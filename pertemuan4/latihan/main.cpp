#include <windows.h>

#ifdef __APPLE__
// #include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
/t
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(4);

    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(5,-10);
    glVertex2f(5,0);
    glVertex2f(0,5);
    glVertex2f(-5,0);
    glVertex2f(-5,-10);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
    glVertex2f(-3,-10);
    glVertex2f(0,-10);
    glVertex2f(0,-4);
    glVertex2f(-3,-4);
    glEnd();

    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
    glVertex2f(2,-4);
    glVertex2f(4,-4);
    glVertex2f(4,-2);
    glVertex2f(2,-2);
    glEnd();



    glFlush();
}

void myinit(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20.0,20.0,-20.0,20.0);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(1.0,1.0,1.0,1.0);
    // glColor3f(0.0,0.0,8.0);
}

int main(int argc, char* argv[]){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700,700);
    glutInitWindowPosition(-400,100);
    glutCreateWindow("Hello World");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();

    return 0;
}