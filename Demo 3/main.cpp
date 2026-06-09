#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

// Status Grid dan Obyek
bool mazeGrid[17][17];
int playerX = 7;
int playerY = 0;
double nrpPosX = 5.0;
double nrpPosY = 5.0;

// Status Mode dan Kamera (0 = Orthogonal, 1 = Isometric)
int viewMode = 0;
// Kemiringan Isometrik diperhalus
double mazeRotX = -25.0;
double mazeRotY = 0.0;
double mazeRotZ = 15.0;

// Status Animasi NRP
double nrpSpinAngle = 0.0;
int nrpSpinAxis = 0; // 0 = Sumbu X, 1 = Sumbu Y, 2 = Sumbu Z

// ==========================================
// FUNGSI KHUSUS MENGGAMBAR DINDING (WARNA-WARNI)
// ==========================================
void drawDinding(double x, double y) {
    // Depan (Merah)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glEnd();

    // Belakang (Biru)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glEnd();

    // Atas (Hijau)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y, 0.0);
    glEnd();

    // Bawah (Ungu)
    glColor3f(0.5, 0.0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glEnd();

    // Kiri (Cyan)
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x, y, 1.0);
    glEnd();

    // Kanan (Kuning)
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glEnd();
}

// ==========================================
// FUNGSI KHUSUS MENGGAMBAR PLAYER (WARNA-WARNI)
// ==========================================
void drawPlayer(double x, double y) {
    // Depan (Ungu)
    glColor3f(0.5, 0.0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glEnd();

    // Belakang (Kuning)
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glEnd();

    // Atas (Cyan)
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y, 1.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y, 0.0);
    glEnd();

    // Bawah (Biru)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x, y + 1.0, 1.0);
    glEnd();

    // Kiri (Merah)
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x, y, 0.0);
    glVertex3f(x, y + 1.0, 0.0);
    glVertex3f(x, y + 1.0, 1.0);
    glVertex3f(x, y, 1.0);
    glEnd();

    // Kanan (Hijau)
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex3f(x + 1.0, y, 0.0);
    glVertex3f(x + 1.0, y, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 1.0);
    glVertex3f(x + 1.0, y + 1.0, 0.0);
    glEnd();
}

// ==========================================
// FUNGSI KHUSUS MENGGAMBAR LANTAI (ABU TRANSPARAN)
// ==========================================
void drawLantai() {
    double al = 0.5;

    // Permukaan Atas Lantai
    glColor4f(0.60, 0.60, 0.60, al);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, -0.01);
    glVertex3f(17.0, 0.0, -0.01);
    glVertex3f(17.0, 17.0, -0.01);
    glVertex3f(0.0, 17.0, -0.01);
    glEnd();

    // Permukaan Bawah Lantai
    glColor4f(0.42, 0.42, 0.42, al);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, -0.21);
    glVertex3f(0.0, 17.0, -0.21);
    glVertex3f(17.0, 17.0, -0.21);
    glVertex3f(17.0, 0.0, -0.21);
    glEnd();

    // Sisi Atas
    glColor4f(0.54, 0.54, 0.54, al);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, -0.01);
    glVertex3f(0.0, 0.0, -0.21);
    glVertex3f(17.0, 0.0, -0.21);
    glVertex3f(17.0, 0.0, -0.01);
    glEnd();

    // Sisi Bawah
    glColor4f(0.30, 0.30, 0.30, al);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 17.0, -0.01);
    glVertex3f(17.0, 17.0, -0.01);
    glVertex3f(17.0, 17.0, -0.21);
    glVertex3f(0.0, 17.0, -0.21);
    glEnd();

    // Sisi Kiri
    glColor4f(0.48, 0.48, 0.48, al);
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, -0.01);
    glVertex3f(0.0, 17.0, -0.01);
    glVertex3f(0.0, 17.0, -0.21);
    glVertex3f(0.0, 0.0, -0.21);
    glEnd();

    // Sisi Kanan
    glColor4f(0.48, 0.48, 0.48, al);
    glBegin(GL_POLYGON);
    glVertex3f(17.0, 0.0, -0.01);
    glVertex3f(17.0, 0.0, -0.21);
    glVertex3f(17.0, 17.0, -0.21);
    glVertex3f(17.0, 17.0, -0.01);
    glEnd();
}

// ==========================================
// MENGGAMBAR NRP 103 (MERAH SHADED)
// ==========================================
void drawNRP3D() {
    glPushMatrix();

    // Titik pusat rotasi NRP
    double centerX = nrpPosX + 0.12;
    double centerY = nrpPosY + 0.16;
    double centerZ = 0.25;

    if (viewMode == 1) {
        glTranslated(centerX, centerY, centerZ);
        if (nrpSpinAxis == 0) glRotated(nrpSpinAngle, 1.0, 0.0, 0.0);
        else if (nrpSpinAxis == 1) glRotated(nrpSpinAngle, 0.0, 1.0, 0.0);
        else if (nrpSpinAxis == 2) glRotated(nrpSpinAngle, 0.0, 0.0, 1.0);
        glTranslated(-centerX, -centerY, -centerZ);
    }

    // ----- ANGKA '1' (1 Balok Vertikal Penuh) -----
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

    // ----- ANGKA '0' (Balok Kiri 0) -----
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

    // ----- ANGKA '0' (Balok Kanan 0) -----
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

    // ----- ANGKA '0' (Balok Atas 0) -----
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

    // ----- ANGKA '0' (Balok Bawah 0) -----
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

    // ----- ANGKA '3' (Balok Kanan 3) -----
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

    // ----- ANGKA '3' (Balok Atas 3) -----
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

    // ----- ANGKA '3' (Balok Tengah 3) -----
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

    // ----- ANGKA '3' (Balok Bawah 3) -----
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

    nrpPosX = (double)rx + 0.5;
    nrpPosY = (double)ry + 0.4;
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
                if (rand() % 2 == 0) mazeGrid[y][x + 1] = false;
                else mazeGrid[y + 1][x] = false;
            } else if (canMoveRight) {
                mazeGrid[y][x + 1] = false;
            } else if (canMoveDown) {
                mazeGrid[y + 1][x] = false;
            }
        }
    }
    mazeGrid[0][7] = false;  // Pintu Masuk
    mazeGrid[16][7] = false; // Pintu Keluar
}

// ==========================================
// FUNGSI ANIMASI BARU MENGGUNAKAN ALGORITMA SPINDISPLAY
// ==========================================
void spinDisplay(void) {
    if (viewMode == 1) { // Animasi hanya berjalan/terlihat saat Mode Isometrik
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

    // 1. Gambar Dinding Maze
    for (int y = 0; y < 17; y++) {
        for (int x = 0; x < 17; x++) {
            if (mazeGrid[y][x]) {
                drawDinding((double)x, (double)y);
            }
        }
    }

    // 2. Gambar Player 3D
    drawPlayer((double)playerX, (double)playerY);

    // 3. Gambar NRP 3D
    drawNRP3D();

    // 4. Lantai Transparan
    drawLantai();

    glutSwapBuffers(); // Menggunakan SwapBuffers untuk animasi
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        if (playerY > 0 && !mazeGrid[playerY - 1][playerX]) playerY--;
    }
    else if (key == 's' || key == 'S') {
        if (playerY < 16 && !mazeGrid[playerY + 1][playerX]) playerY++;
    }
    else if (key == 'a' || key == 'A') {
        if (playerX > 0 && !mazeGrid[playerY][playerX - 1]) playerX--;
    }
    else if (key == 'd' || key == 'D') {
        if (playerX < 16 && !mazeGrid[playerY][playerX + 1]) playerX++;
    }
    else if (key == 'c' || key == 'C') {
        generateMaze();
        playerX = 7;
        playerY = 0;
        letakkanNRP();
    }
    else if (key == 'v' || key == 'V') {
        viewMode = (viewMode == 0) ? 1 : 0;
    }
    else if (viewMode == 1) {
        if (key == 'j' || key == 'J') mazeRotZ -= 5.0;
        else if (key == 'l' || key == 'L') mazeRotZ += 5.0;
        else if (key == 'i' || key == 'I') mazeRotX -= 5.0;
        else if (key == 'k' || key == 'K') mazeRotX += 5.0;
    }
    glutPostRedisplay();
}

// ==========================================
// FUNGSI MOUSE BARU MENGGUNAKAN ALGORITMA SPINDISPLAY
// ==========================================
void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                nrpSpinAxis = 0; // Kiri -> Sumbu X
                glutIdleFunc(spinDisplay);
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                nrpSpinAxis = 1; // Kanan -> Sumbu Y
                glutIdleFunc(spinDisplay);
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            if (state == GLUT_DOWN) {
                nrpSpinAxis = 2; // Tengah -> Sumbu Z
                glutIdleFunc(spinDisplay);
            }
            break;
        default:
            break;
    }
}

void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Resolusi Ortho
    glOrtho(-4.0, 21.0, 21.0, -4.0, -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand((unsigned int)time(NULL));
    generateMaze();
    letakkanNRP();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Maze 3D Isometrik - Aslih Maulana Syadat (NRP 103)");

    myinit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    // CATATAN: glutIdleFunc sengaja TIDAK dipanggil di sini.
    // Animasi akan otomatis berhenti di awal, dan baru berputar saat Mouse diklik.

    glutMainLoop();
    return 0;
}