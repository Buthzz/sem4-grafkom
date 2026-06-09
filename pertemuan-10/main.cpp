#include <windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
// #include <stdlib.h>
#include <iostream>

using namespace std;

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
float mazeRotZ = 0.0;

// Status Animasi NRP
float nrpSpinAngle = 0.2;
int nrpSpinAxis = 0;

void drawDinding(float x, float y) {
    // Depan
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glEnd();

    // Belakang
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glEnd();

    // Atas
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y, 0.0);
    glEnd();

    // Bawah
    glColor3f(0.5, 0.0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glEnd();

    // Kiri
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x, y, 1.0);
    glEnd();

    // Kanan
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glEnd();
}

void drawPlayer(float x, float y) {
    // Depan
    glColor3f(0.5, 0.0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glEnd();

    // Belakang
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glEnd();

    // Atas
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y, 0.0);
    glEnd();

    // Bawah
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glEnd();

    // Kiri
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x, y, 1.0);
    glEnd();

    // Kanan
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glEnd();
}

void drawLantai() {
    float al = 0.5;

    glColor4f(0.60, 0.60, 0.60, al);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, -0.01);
    glVertex3f(17.0, 0.0, -0.01);
    glVertex3f(17.0, 17.0, -0.01);
    glVertex3f(0.0, 17.0, -0.01);
    glEnd();

}


void drawNRP3D() {
    glPushMatrix();

    // Titik pusat rotasi NRP
    float centerX = nrpPosX + 0.12;
    float centerY = nrpPosY + 0.16;
    float centerZ = 0.25;

    if (viewMode == 1) {
        glTranslated(centerX, centerY, centerZ);
        if (nrpSpinAxis == 0) {
                glRotated(nrpSpinAngle, 1.0, 0.0, 0.0);
        }
        else if (nrpSpinAxis == 1) {
                glRotated(nrpSpinAngle, 0.0, 1.0, 0.0);
        }
        else if (nrpSpinAxis == 2) {
                glRotated(nrpSpinAngle, 0.0, 0.0, 1.0);
        }
        glTranslated(-centerX, -centerY, -centerZ);
    }

    // ANGKA 1
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.40, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.32, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.32, nrpPosY + 0.32, 0.0);
    glEnd();

    // ANGKA 0 Balok Kiri
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.16, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.16, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glEnd();

    // ANGKA 0 Balok Kanan
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.08, nrpPosY + 0.32, 0.0);
    glEnd();

    // ANGKA 0 Balok Atas
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.24, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.32, 0.0);
    glEnd();

    // ANGKA 0 Balok Bawah
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX - 0.08, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX - 0.08, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.00, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.00, nrpPosY + 0.00, 0.0);
    glEnd();

    // ANGKA 3 Balok Kanan
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.48, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.48, nrpPosY + 0.32, 0.0);
    glEnd();

    // ANGKA 3 Balok Atas
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.24, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.24, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.32, 0.0);
    glEnd();

    // ANGKA 3 Balok Tengah
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.08, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.16, 0.0);
    glEnd();

    // ANGKA 3 Balok Bawah
    // Depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.0);
    glEnd();
    // Belakang
    glColor4f(0.7, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glEnd();
    // Atas
    glColor4f(0.9, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.5);
    glVertex3f(nrpPosX + 0.40, nrpPosY - 0.08, 0.0);
    glEnd();
    // Bawah
    glColor4f(0.5, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.40, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.5);
    glEnd();
    // Kiri
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.0);
    glVertex3f(nrpPosX + 0.24, nrpPosY + 0.00, 0.5);
    glVertex3f(nrpPosX + 0.24, nrpPosY - 0.08, 0.5);
    glEnd();
    // Kanan
    glColor4f(0.8, 0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
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
    nrpPosY = (float)ry + 0.5;

    // cout << nrpPosX << " " << nrpPosY << endl;
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
        glRotated(mazeRotZ, 0.0, 0.0, 1.0);
        glTranslated(-8.5, -8.5, 0.0);
    }

    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            if (mazeGrid[y][x] == true) {
                drawDinding((float)x, (float)y);
            }
        }
    }



    drawPlayer((float)playerX, (float)playerY);

    drawNRP3D();

    drawLantai();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        if (playerY > 0) {
            if (mazeGrid[playerY - 1][playerX] == false) {
                playerY--;
            }
        }
    }
    else if (key == 's' || key == 'S') {
        if (playerY < 16) {
            if (mazeGrid[playerY + 1][playerX] == false) {
                playerY++;
            }
        }
    }
    else if (key == 'a' || key == 'A') {
        if (playerX > 0) {
            if (mazeGrid[playerY][playerX - 1] == false) {
                playerX--;
            }
        }
    }
    else if (key == 'd' || key == 'D') {
        if (playerX < 16) {
            if (mazeGrid[playerY][playerX + 1] == false) {
                playerX++;
            }
        }
    }
    else if (key == 'c' || key == 'C') {
        generateMaze();
        playerX = 7;
        playerY = 0;
        letakkanNRP();
    }
    else if (key == 'v' || key == 'V') {
        if (viewMode == 0) {
            viewMode = 1;
        } else {
            viewMode = 0;
        }
    }
    else if (viewMode == 1) {
        if (key == 'j' || key == 'J') mazeRotZ -= 5.0;
        else if (key == 'l' || key == 'L') mazeRotZ += 5.0;
        else if (key == 'i' || key == 'I') mazeRotX -= 5.0;
        else if (key == 'k' || key == 'K') mazeRotX += 5.0;
    }
    glutPostRedisplay();

    cout << mazeRotZ << endl << mazeRotX<< endl;
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

    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    generateMaze();
    letakkanNRP();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("DEMO 3 | Kelompok 4");

    myinit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutIdleFunc(spinDisplay);

    glutMainLoop();
    return 0;
}