#include <bits/stdc++.h>
#include <GLUT/glut.h>
using namespace std;

float angleY = 0;

void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,100);
}

void drawPyramid(){
    glBegin(GL_TRIANGLES);

    // Front
    glColor3f(1,0,0);
    glVertex3f(0,2,0);
    glVertex3f(-2,-2,2);
    glVertex3f(2,-2,2);

    // Right
    glColor3f(0,1,0);
    glVertex3f(0,2,0);
    glVertex3f(2,-2,2);
    glVertex3f(2,-2,-2);

    // Back
    glColor3f(0,0,1);
    glVertex3f(0,2,0);
    glVertex3f(2,-2,-2);
    glVertex3f(-2,-2,-2);

    // Left
    glColor3f(1,1,0);
    glVertex3f(0,2,0);
    glVertex3f(-2,-2,-2);
    glVertex3f(-2,-2,2);

    glEnd();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,-10);
    glRotatef(angleY,0,1,0);

    drawPyramid();

    glutSwapBuffers();
}

void timer(int){
    angleY += 1;
    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("3D Pyramid");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(16,timer,0);
    glutMainLoop();
}
