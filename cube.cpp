#include <bits/stdc++.h>
#include <GLUT/glut.h>   // use <GL/glut.h> in Windows/Ubuntu
using namespace std;

float angleX = 0, angleY = 0;
bool rotateX = true, rotateY = true;

void init(){
    glEnable(GL_DEPTH_TEST);   // IMPORTANT for 3D
    glClearColor(0,0,0,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 100);   // perspective view
}

void drawCube(){
    glColor3f(0,1,1);
    glutSolidCube(2);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glTranslatef(0,0,-8);   // move back
    glRotatef(angleX,1,0,0);
    glRotatef(angleY,0,1,0);

    drawCube();

    glutSwapBuffers();
}

void timer(int){
    if(rotateX) angleX += 1;
    if(rotateY) angleY += 1;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void specialKey(int key, int, int){
    if(key == GLUT_KEY_LEFT)  rotateY = false;
    if(key == GLUT_KEY_RIGHT) rotateY = true;
    if(key == GLUT_KEY_UP)    rotateX = true;
    if(key == GLUT_KEY_DOWN)  rotateX = false;
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("3D Rotating Cube");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
}
