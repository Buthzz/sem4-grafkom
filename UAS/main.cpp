#include <windows.h>
#include <cmath>
#include <cstdio>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// Status Grid dan Obyek
bool mazeGrid[17][17];
int playerX = 7;
int playerY = 0;
float nrpPosX = 0.0;
float nrpPosY = 0.0;

int viewMode = 0;
// Kemiringan Isometrik
float mazeRotX = 25.0;
float mazeRotY = 0.0;

// Status Animasi NRP
float nrpSpinAngle = 0.0;
int nrpSpinAxis = 0;

// === FPS Camera ===
float camPosX = 7.5f;
float camPosY = 0.5f;
float camPosZ = 0.5f;
float camYaw = 90.0f; // menghadap ke bawah (arah +Y)
bool camInitialized = false;

int windowW = 700, windowH = 700;

bool ambOn = true;
bool diffOn = true;
bool specOn = true;
bool isNight = false;
bool wallTransparent = false;

bool collisionDetected = false;

void drawDinding(float x, float y) {
    float alpha = wallTransparent ? 0.3f : 1.0f;

    GLfloat mat_ambient[]  = { 0.3f, 0.0f, 0.0f, alpha };
    GLfloat mat_diffuse[]  = { 0.8f, 0.0f, 0.0f, alpha };
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, alpha };
    GLfloat mat_shininess[]= { 32.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Depan (normal ke -Y)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x,       y,       0.0f);
    glVertex3f(x + 1.0, y,       0.0f);
    glVertex3f(x + 1.0, y,       1.0f);
    glVertex3f(x,       y,       1.0f);
    glEnd();

    // Belakang (normal ke +Y)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x,       y + 1.0, 0.0f);
    glVertex3f(x,       y + 1.0, 1.0f);
    glVertex3f(x + 1.0, y + 1.0, 1.0f);
    glVertex3f(x + 1.0, y + 1.0, 0.0f);
    glEnd();

    // Atas (normal ke +Z)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x,       y,       1.0f);
    glVertex3f(x + 1.0, y,       1.0f);
    glVertex3f(x + 1.0, y + 1.0, 1.0f);
    glVertex3f(x,       y + 1.0, 1.0f);
    glEnd();

    // Bawah (normal ke -Z)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x,       y,       0.0f);
    glVertex3f(x,       y + 1.0, 0.0f);
    glVertex3f(x + 1.0, y + 1.0, 0.0f);
    glVertex3f(x + 1.0, y,       0.0f);
    glEnd();

    // Kiri (normal ke -X)
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x,       y,       0.0f);
    glVertex3f(x,       y + 1.0, 0.0f);
    glVertex3f(x,       y + 1.0, 1.0f);
    glVertex3f(x,       y,       1.0f);
    glEnd();

    // Kanan (normal ke +X)
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x + 1.0, y,       0.0f);
    glVertex3f(x + 1.0, y,       1.0f);
    glVertex3f(x + 1.0, y + 1.0, 1.0f);
    glVertex3f(x + 1.0, y + 1.0, 0.0f);
    glEnd();
}

void drawPlayer(float x, float y) {
    // Set material player (biru)
    GLfloat mat_ambient[]  = { 0.0f, 0.0f, 0.3f, 1.0f };
    GLfloat mat_diffuse[]  = { 0.0f, 0.0f, 0.9f, 1.0f };
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_shininess[]= { 64.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Depan (normal ke -Y)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x,       y,       0.0f);
    glVertex3f(x + 1.0, y,       0.0f);
    glVertex3f(x + 1.0, y,       1.0f);
    glVertex3f(x,       y,       1.0f);
    glEnd();

    // Belakang (normal ke +Y)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x,       y + 1.0, 0.0f);
    glVertex3f(x,       y + 1.0, 1.0f);
    glVertex3f(x + 1.0, y + 1.0, 1.0f);
    glVertex3f(x + 1.0, y + 1.0, 0.0f);
    glEnd();

    // Atas (normal ke +Z)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x,       y,       1.0f);
    glVertex3f(x + 1.0, y,       1.0f);
    glVertex3f(x + 1.0, y + 1.0, 1.0f);
    glVertex3f(x,       y + 1.0, 1.0f);
    glEnd();

    // Bawah (normal ke -Z)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x,       y,       0.0f);
    glVertex3f(x,       y + 1.0, 0.0f);
    glVertex3f(x + 1.0, y + 1.0, 0.0f);
    glVertex3f(x + 1.0, y,       0.0f);
    glEnd();

    // Kiri (normal ke -X)
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(x,       y,       0.0f);
    glVertex3f(x,       y + 1.0, 0.0f);
    glVertex3f(x,       y + 1.0, 1.0f);
    glVertex3f(x,       y,       1.0f);
    glEnd();

    // Kanan (normal ke +X)
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x + 1.0, y,       0.0f);
    glVertex3f(x + 1.0, y,       1.0f);
    glVertex3f(x + 1.0, y + 1.0, 1.0f);
    glVertex3f(x + 1.0, y + 1.0, 0.0f);
    glEnd();
}

void drawLantai() {
    float al = 0.5f;

    GLfloat mat_ambient[]  = { 0.2f, 0.2f, 0.2f, al };
    GLfloat mat_diffuse[]  = { 0.6f, 0.6f, 0.6f, al };
    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, al };
    GLfloat mat_shininess[]= { 8.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // Permukaan Atas Lantai (normal ke +Z)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0,  0.0,  -0.01f);
    glVertex3f(17.0, 0.0,  -0.01f);
    glVertex3f(17.0, 17.0, -0.01f);
    glVertex3f(0.0,  17.0, -0.01f);
    glEnd();

    // Permukaan Bawah Lantai (normal ke -Z)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0,  0.0,  -0.21f);
    glVertex3f(0.0,  17.0, -0.21f);
    glVertex3f(17.0, 17.0, -0.21f);
    glVertex3f(17.0, 0.0,  -0.21f);
    glEnd();

    // Sisi Atas (normal ke -Y)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0,  0.0, -0.01f);
    glVertex3f(0.0,  0.0, -0.21f);
    glVertex3f(17.0, 0.0, -0.21f);
    glVertex3f(17.0, 0.0, -0.01f);
    glEnd();

    // Sisi Bawah (normal ke +Y)
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0,  17.0, -0.01f);
    glVertex3f(17.0, 17.0, -0.01f);
    glVertex3f(17.0, 17.0, -0.21f);
    glVertex3f(0.0,  17.0, -0.21f);
    glEnd();

    // Sisi Kiri (normal ke -X)
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0, 0.0,  -0.01f);
    glVertex3f(0.0, 17.0, -0.01f);
    glVertex3f(0.0, 17.0, -0.21f);
    glVertex3f(0.0, 0.0,  -0.21f);
    glEnd();

    // Sisi Kanan (normal ke +X)
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(17.0, 0.0,  -0.01f);
    glVertex3f(17.0, 0.0,  -0.21f);
    glVertex3f(17.0, 17.0, -0.21f);
    glVertex3f(17.0, 17.0, -0.01f);
    glEnd();
}


void drawNRP3D() {
    glPushMatrix();

    float centerX = nrpPosX + 0.12;
    float centerY = nrpPosY + 0.16;
    float centerZ = 0.25;

    if (viewMode == 1) {
        glTranslated(centerX, centerY, centerZ);
        if (nrpSpinAxis == 0)
            glRotated(nrpSpinAngle, 1.0, 0.0, 0.0);
        else if (nrpSpinAxis == 1)
            glRotated(nrpSpinAngle, 0.0, 1.0, 0.0);
        else if (nrpSpinAxis == 2)
            glRotated(nrpSpinAngle, 0.0, 0.0, 1.0);
        glTranslated(-centerX, -centerY, -centerZ);
    }

    // Material NRP (hijau)
    GLfloat mat_ambient[]  = { 0.0f, 0.3f, 0.0f, 1.0f };
    GLfloat mat_diffuse[]  = { 0.0f, 0.8f, 0.0f, 1.0f };
    GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat mat_shininess[]= { 32.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // =====================
    // ANGKA 1
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.0);
    glEnd();

    // =====================
    // ANGKA 0 - Balok Kiri
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glEnd();

    // =====================
    // ANGKA 0 - Balok Kanan
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.0);
    glEnd();

    // =====================
    // ANGKA 0 - Balok Atas
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glEnd();

    // =====================
    // ANGKA 0 - Balok Bawah
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.0);
    glEnd();

    // =====================
    // ANGKA 3 - Balok Kanan
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.0);
    glEnd();

    // =====================
    // ANGKA 3 - Balok Atas
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glEnd();

    // =====================
    // ANGKA 3 - Balok Tengah
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.0);
    glEnd();

    // =====================
    // ANGKA 3 - Balok Bawah
    // =====================
    // Depan
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.0);
    glEnd();
    // Belakang
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.5);
    glEnd();
    // Kiri
    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.0);
    glEnd();

    glPopMatrix();
}

void letakkanNRP() {
    int rx, ry;
    do {
        rx = 1 + rand() % 15;
        ry = 1 + rand() % 15;
    } while (mazeGrid[ry][rx] == true);

    nrpPosX = (float)rx + 0.5;
    nrpPosY = (float)ry + 0.4;
}

void generateMaze() {
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            mazeGrid[y][x] = true;
        }
    }
    for (int y = 1; y < 16; y += 2) {
        for (int x = 1; x < 16; x += 2) {
            mazeGrid[y][x] = false;
            bool canMoveRight = (x + 2 < 16);
            bool canMoveDown = (y + 2 < 16);
            if (canMoveRight && canMoveDown) {
                if (rand() % 2 == 0) {
                        mazeGrid[y][x + 1] = false;
                } else  {
                    mazeGrid[y + 1][x] = false;
                }
            } else if (canMoveRight) {
                mazeGrid[y][x + 1] = false;
            } else if (canMoveDown) {
                mazeGrid[y + 1][x] = false;
            }
        }
    }
    mazeGrid[0][7] = false;
    mazeGrid[16][7] = false;
}

void spinDisplay(void) {
    if (viewMode == 1) {
        nrpSpinAngle = nrpSpinAngle + 0.8;
        if (nrpSpinAngle > 360.0)
            nrpSpinAngle = nrpSpinAngle - 360.0;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    windowW = w;
    windowH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (viewMode == 2) {
        gluPerspective(65.0, (float)w / (float)h, 0.1, 100.0);
    } else {
        glOrtho(-4.0, 21.0, 21.0, -4.0, -50.0, 50.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void updateLighting() {
    // Ambient
    GLfloat amb[4];
    if (ambOn) {
        if (isNight) {
            amb[0]=0.05f; amb[1]=0.05f; amb[2]=0.1f; amb[3]=1.0f;
        } else {
            amb[0]=0.4f; amb[1]=0.4f; amb[2]=0.4f; amb[3]=1.0f;
        }
    } else {
        amb[0]=0.0f; amb[1]=0.0f; amb[2]=0.0f; amb[3]=1.0f;
    }
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    // Diffuse (LIGHT0)
    GLfloat diff[4];
    if (diffOn) {
        if (isNight) {
            diff[0]=0.1f; diff[1]=0.1f; diff[2]=0.3f; diff[3]=1.0f;
        } else {
            diff[0]=1.0f; diff[1]=1.0f; diff[2]=1.0f; diff[3]=1.0f;
        }
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
    }

    // Specular (LIGHT1)
    GLfloat spec[4];
    if (specOn) {
        if (isNight) {
            spec[0]=0.2f; spec[1]=0.2f; spec[2]=0.5f; spec[3]=1.0f;
        } else {
            spec[0]=1.0f; spec[1]=1.0f; spec[2]=1.0f; spec[3]=1.0f;
        }
        glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
        glEnable(GL_LIGHT1);
    } else {
        glDisable(GL_LIGHT1);
    }

    // Background siang/malam
    if (isNight) {
        glClearColor(0.0f, 0.0f, 0.05f, 1.0f);
    } else {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
}

void cekTabrakan() {
    if (collisionDetected) return;

    int nrpTileX = (int)(nrpPosX);
    int nrpTileY = (int)(nrpPosY);

    if (playerX == nrpTileX && playerY == nrpTileY) {
        collisionDetected = true;
        printf("Dapat NRP!\n");
        glutPostRedisplay();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (viewMode == 1) {
        glTranslated(8.5, 8.5, 0.0);
        glRotated(mazeRotX, 1.0, 0.0, 0.0);
        glRotated(mazeRotY, 0.0, 1.0, 0.0);
        glTranslated(-8.5, -8.5, 0.0);
    } else if (viewMode == 2) {
        float radYaw = camYaw * 3.14159f / 180.0f;
        float lookX = camPosX + cos(radYaw);
        float lookY = camPosY + sin(radYaw);
        gluLookAt(camPosX, camPosY, camPosZ,
                  lookX,   lookY,   camPosZ,
                  0.0,     0.0,     1.0);
    }

    // Gambar obyek solid dulu
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    drawPlayer((float)playerX, (float)playerY);
    drawNRP3D();
    drawLantai();

    // Gambar dinding   jika transparan, aktifkan blend
    if (wallTransparent) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    }
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            if (mazeGrid[y][x] == true) {
                drawDinding((float)x, (float)y);
            }
        }
    }
    if (wallTransparent) {
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        if (playerY > 0) {
            if (mazeGrid[playerY - 1][playerX] == false) {
                playerY--;
                cekTabrakan();
            }
        }
    }
    else if (key == 's' || key == 'S') {
        if (playerY < 16) {
            if (mazeGrid[playerY + 1][playerX] == false) {
                playerY++;
                cekTabrakan();
            }
        }
    }
    else if (key == 'a' || key == 'A') {
        if (playerX > 0) {
            if (mazeGrid[playerY][playerX - 1] == false) {
                playerX--;
                cekTabrakan();
            }
        }
    }
    else if (key == 'd' || key == 'D') {
        if (playerX < 16) {
            if (mazeGrid[playerY][playerX + 1] == false) {
                playerX++;
                cekTabrakan();
            }
        }
    }
    else if (key == 'c' || key == 'C') {
        generateMaze();
        playerX = 7;
        playerY = 0;
        collisionDetected = false;
        letakkanNRP();
    }
    else if (key == 'v' || key == 'V') {
    viewMode = (viewMode + 1) % 3;
    if (viewMode == 2 && !camInitialized) {
        camPosX = 7.5f;
        camPosY = 1.5f;   // <-- ubah dari 0.5 ke 1.5 agar tidak di tepi
        camPosZ = 0.5f;
        camYaw = 90.0f;
        camInitialized = true;
    }
    // Update proyeksi
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (viewMode == 2) {
        gluPerspective(65.0, (float)windowW / (float)windowH, 0.1, 100.0);
    } else {
        glOrtho(-4.0, 21.0, 21.0, -4.0, -50.0, 50.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
    }
    else if (viewMode == 1) {
        if (key == 'j' || key == 'J') mazeRotY -= 5.0;
        else if (key == 'l' || key == 'L') mazeRotY += 5.0;
        else if (key == 'i' || key == 'I') mazeRotX -= 5.0;
        else if (key == 'k' || key == 'K') mazeRotX += 5.0;
    }
    // === TOGGLE LAMPU & FITUR ===
    // Dipisah dari else if agar selalu terpanggil
    if (key == '1') {
        ambOn = !ambOn;
        updateLighting();
    }
    else if (key == '2') {
        diffOn = !diffOn;
        updateLighting();
    }
    else if (key == '3') {
        specOn = !specOn;
        updateLighting();
    }
    else if (key == '4') {
        isNight = !isNight;
        updateLighting();
    }
    else if (key == '5') {
        wallTransparent = !wallTransparent;
    }

    glutPostRedisplay();
}

void specialKey(int key, int x, int y) {
    if (viewMode == 2) {
        float radYaw = camYaw * 3.14159f / 180.0f;
        float moveSpeed = 0.3f;
        float turnSpeed = 5.0f;

        float nextX = camPosX;
        float nextY = camPosY;

        if (key == GLUT_KEY_UP) {
            // Maju ke arah pandang
            nextX = camPosX + cos(radYaw) * moveSpeed;
            nextY = camPosY + sin(radYaw) * moveSpeed;
        }
        else if (key == GLUT_KEY_DOWN) {
            // Mundur
            nextX = camPosX - cos(radYaw) * moveSpeed;
            nextY = camPosY - sin(radYaw) * moveSpeed;
        }
        else if (key == GLUT_KEY_LEFT) {
            // Putar kamera ke kiri
            camYaw -= turnSpeed;
        }
        else if (key == GLUT_KEY_RIGHT) {
            // Putar kamera ke kanan
            camYaw += turnSpeed;
        }

        // Cek tabrakan dengan dinding sebelum pindah
        int tileX = (int)nextX;
        int tileY = (int)nextY;

        if (tileX >= 0 && tileX < 17 && tileY >= 0 && tileY < 17) {
            if (mazeGrid[tileY][tileX] == false) {
                camPosX = nextX;
                camPosY = nextY;
            }
        }

        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) nrpSpinAxis = 0;      // Sumbu X
        else if (button == GLUT_RIGHT_BUTTON) nrpSpinAxis = 1; // Sumbu Y
        else if (button == GLUT_MIDDLE_BUTTON) nrpSpinAxis = 2; // Sumbu Z
    }
}

void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4.0, 21.0, 21.0, -4.0, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glShadeModel(GL_SMOOTH); // ganti dari GL_FLAT ke GL_SMOOTH
    glEnable(GL_DEPTH_TEST);

    // === PENCAHAYAAN ===
    // Ambient global
    GLfloat ambient_global[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_global);

    // Light 0 = Diffuse (dari atas)
    GLfloat light0_pos[]     = { 8.5f, 8.5f, 10.0f, 1.0f };
    GLfloat light0_diffuse[] = { 1.0f, 1.0f,  1.0f, 1.0f };
    GLfloat light0_ambient[] = { 0.0f, 0.0f,  0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);

    // Light 1 = Specular (dari samping)
    GLfloat light1_pos[]      = { 0.0f, 0.0f, 5.0f, 1.0f };
    GLfloat light1_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light1_diffuse[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    generateMaze();
    letakkanNRP();
    updateLighting();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);
    glutInitWindowSize(700, 700);
    glutCreateWindow("DEMO 3 | Kelompok 4");

    myinit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutSpecialFunc(specialKey);
    glutIdleFunc(spinDisplay);

    glutMainLoop();
    return 0;
}
