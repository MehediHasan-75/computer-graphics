#include <bits/stdc++.h>
#include <GLUT/glut.h>
using namespace std;

float earthAngle = 0;
float moonAngle = 0;

void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,100);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0,0,-20);

    // Sun
    glColor3f(1,1,0);
    glutSolidSphere(2,50,50);

    // Earth revolution
    glRotatef(earthAngle,0,1,0);
    glTranslatef(8,0,0);

    glColor3f(0,0,1);
    glutSolidSphere(1,50,50);

    // Moon revolution
    glRotatef(moonAngle,0,1,0);
    glTranslatef(2,0,0);

    glColor3f(1,1,1);
    glutSolidSphere(0.4,50,50);

    glutSwapBuffers();
}

void timer(int){
    earthAngle += 1;
    moonAngle += 3;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("3D Solar System");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16, timer, 0);
    glutMainLoop();
}