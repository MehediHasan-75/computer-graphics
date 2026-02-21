#include <bits/stdc++.h>
#include <cstdlib>
#include <GLUT/glut.h>
using namespace std;

float heliX = 0, heliZ = -30;
float rotorAngle = 0;
float tailAngle = 0;

void initLighting(){
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    float lightPos[] = {0,20,20,1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.2,0.25,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,200);

    initLighting();
}

void drawHelicopter(){

    glPushMatrix();
    glTranslatef(heliX,0,heliZ);

    // Body
    glPushMatrix();
    glScalef(4,1.5,1.5);
    glColor3f(0.1,0.4,0.8);
    glutSolidCube(2);
    glPopMatrix();

    // Cabin
    glPushMatrix();
    glTranslatef(2.5,0,0);
    glColor3f(0.8,0.8,0.9);
    glutSolidSphere(1.2,30,30);
    glPopMatrix();

    // Tail
    glPushMatrix();
    glTranslatef(-5,0,0);
    glScalef(6,0.3,0.3);
    glColor3f(0.1,0.4,0.8);
    glutSolidCube(1);
    glPopMatrix();

    // Main Rotor
    glPushMatrix();
    glTranslatef(0,2,0);
    glRotatef(rotorAngle,0,1,0);
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
        glVertex3f(-6,0,0.1);
        glVertex3f(6,0,0.1);
        glVertex3f(6,0,-0.1);
        glVertex3f(-6,0,-0.1);
    glEnd();
    glPopMatrix();

    // Tail Rotor
    glPushMatrix();
    glTranslatef(-8,0,0);
    glRotatef(tailAngle,1,0,0);
    glBegin(GL_QUADS);
        glVertex3f(0,-1,0.1);
        glVertex3f(0,1,0.1);
        glVertex3f(0,1,-0.1);
        glVertex3f(0,-1,-0.1);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera follows helicopter
    gluLookAt(heliX,10,heliZ+25,
              heliX,0,heliZ,
              0,1,0);

    drawHelicopter();

    glutSwapBuffers();
}

void timer(int){
    rotorAngle += 15;
    tailAngle += 20;

    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}

void keyboard(unsigned char key,int,int){
    if(key=='w') heliZ -= 2;
    if(key=='s') heliZ += 2;
    if(key=='a') heliX -= 2;
    if(key=='d') heliX += 2;

    if(key=='r'){
        heliX=0;
        heliZ=-30;
    }

    if(key==27) exit(0);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutCreateWindow("Advanced 3D Helicopter Animation");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16,timer,0);

    glutMainLoop();
}
