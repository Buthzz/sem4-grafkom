#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

// Diletakkan di luar agar bisa dipakai di display() dan input()
float postX = 0.0;
float postY = 0.0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity(); // Reset transformasi sebelumnya

    // Geser seluruh objek berdasarkan nilai postX dan postY
    glTranslatef(postX, postY, 0.0f); // ini jika

    // Gambar Batang Panah
    // glBegin(GL_QUADS);
    // glVertex2f(-3.0 , 3.0 + postY);
    // glVertex2f(-3.0, -3.0 + postY);
    // glVertex2f(15.0, -3.0 + postY);
    // glVertex2f(15.0, 3.0);
    // glEnd();

    // Gambar Kepala Panah
    glBegin(GL_LINES);
    glVertex2i(10,0);
    glVertex2i(-10,0);
    glVertex2i(0,10);
    glVertex2i(0,-10);
    glEnd();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for(float t =-10.0; t<=10.0; t+=0.1){
        glVertex2f(t,2*t*t-t+1);
    }
    glEnd();

    glFlush();
}

void input(unsigned char key, int x, int y) {
    if (key == 'q' || key == 27) { exit(0); } // 27 adalah tombol ESC

    // Logika pergerakan (Sesuaikan arah +/- sesuai keinginan)
    if (key == 'w') postY += 0.5; // Atas
    if (key == 's') postY -= 0.5; // Bawah
    if (key == 'a') postX -= 0.5; // Kiri
    if (key == 'd') postX += 0.5; // Kanan

    glutPostRedisplay();
}

void myinit() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-25.0, 25.0, -25.0, 25.0); // Diperlebar agar ruang gerak luas
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Gerakan Panah");

    myinit();

    glutDisplayFunc(display);
    // Agar bisa menggunakan keyboard
    glutKeyboardFunc(input);

    glutMainLoop();
    return 0;

    cout << "gua ganteng";
}