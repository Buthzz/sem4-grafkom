#include <GL/glut.h>

void myinit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(4);
    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_POINTS);
    glVertex2f(-1.0, -1.0);
    glVertex2f( 1.0, -1.0);
    glVertex2f( 0.0,  1.0);
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Segitiga Warna");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
    return 0;
}