#include <bits/stdc++.h>
#include <GLUT/glut.h>
using namespace std;

float robotX = 0;
float walkAngle = 0;
bool walking = false;

void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1,0.1,0.1,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,100);
}

void drawRobot(){

    glPushMatrix();
    glTranslatef(robotX, -2, -20);

    // Body
    glPushMatrix();
    glScalef(2,3,1);
    glColor3f(0,0,1);
    glutSolidCube(1);
    glPopMatrix();

    // Head
    glPushMatrix();
    glTranslatef(0,2.5,0);
    glColor3f(1,0.8,0.6);
    glutSolidSphere(0.8,30,30);
    glPopMatrix();

    // Left Arm
    glPushMatrix();
    glTranslatef(-1.5,1,0);
    glRotatef(sin(walkAngle)*45,1,0,0);
    glTranslatef(0,-1,0);
    glScalef(0.5,2,0.5);
    glColor3f(1,0,0);
    glutSolidCube(1);
    glPopMatrix();

    // Right Arm
    glPushMatrix();
    glTranslatef(1.5,1,0);
    glRotatef(-sin(walkAngle)*45,1,0,0);
    glTranslatef(0,-1,0);
    glScalef(0.5,2,0.5);
    glColor3f(1,0,0);
    glutSolidCube(1);
    glPopMatrix();

    // Left Leg
    glPushMatrix();
    glTranslatef(-0.7,-2,0);
    glRotatef(-sin(walkAngle)*45,1,0,0);
    glTranslatef(0,-1,0);
    glScalef(0.7,2,0.7);
    glColor3f(0,1,0);
    glutSolidCube(1);
    glPopMatrix();

    // Right Leg
    glPushMatrix();
    glTranslatef(0.7,-2,0);
    glRotatef(sin(walkAngle)*45,1,0,0);
    glTranslatef(0,-1,0);
    glScalef(0.7,2,0.7);
    glColor3f(0,1,0);
    glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawRobot();

    glutSwapBuffers();
}

void timer(int){
    if(walking){
        walkAngle += 0.1;
        robotX += 0.02;
    }

    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}

void keyboard(unsigned char key, int, int){

    if(key == 'w' || key == 'W'){
        walking = true;
    }

    if(key == 's' || key == 'S'){
        walking = false;
    }

    if(key == 'r' || key == 'R'){
        robotX = 0;
        walkAngle = 0;
    }

    if(key == 27) exit(0);
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,800);
    glutCreateWindow("Complex 3D Walking Robot");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16,timer,0);

    glutMainLoop();
}
