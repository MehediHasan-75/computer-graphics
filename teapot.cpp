#include <bits/stdc++.h>
#include <GLUT/glut.h>
using namespace std;

float angleX = 0, angleY = 0;
float zoom = -10;

void init(){
    glEnable(GL_DEPTH_TEST);
    glClearColor(0,0,0,1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60,1,1,100);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,zoom);
    glRotatef(angleX,1,0,0);
    glRotatef(angleY,0,1,0);

    glColor3f(1,0,0);
    glutSolidTeapot(2);

    glutSwapBuffers();
}

void specialKey(int key, int, int){
    if(key == GLUT_KEY_LEFT)  angleY -= 5;
    if(key == GLUT_KEY_RIGHT) angleY += 5;
    if(key == GLUT_KEY_UP)    angleX -= 5;
    if(key == GLUT_KEY_DOWN)  angleX += 5;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int){
    if(key == '+') zoom += 1;
    if(key == '-') zoom -= 1;

    glutPostRedisplay();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600,600);
    glutCreateWindow("3D Teapot Control");

    init();
    glutDisplayFunc(display);
    glutSpecialFunc(specialKey);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
}