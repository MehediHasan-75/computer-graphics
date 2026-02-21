#include <GL/glut.h>
#include <iostream>
using namespace std;

// Clipping window
float xmin = -100, xmax = 100, ymin = -100, ymax = 100;

bool clipEnabled = false;

// Line structure
struct Line {
    float x1, y1, x2, y2;
};

Line lines[] = {
    {-150, 50, 150, 50},
    {-50, -150, 50, 150},
    {-200, -200, -50, -50},
    {0, 0, 80, 80}
};

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

int computeCode(float x, float y) {
    int code = INSIDE;

    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;

    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;

    return code;
}

bool cohenSutherland(float &x1, float &y1, float &x2, float &y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    while (true) {
        if (!(code1 | code2)) {
            return true; // Accept
        }
        else if (code1 & code2) {
            return false; // Reject
        }
        else {
            float x, y;
            int codeOut = code1 ? code1 : code2;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
                y = ymax;
            }
            else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
                y = ymin;
            }
            else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
                x = xmax;
            }
            else if (codeOut & LEFT) {
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
                x = xmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
}

void drawWindow() {
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2) {
    glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawWindow();

    for (int i = 0; i < 4; i++) {
        float x1 = lines[i].x1;
        float y1 = lines[i].y1;
        float x2 = lines[i].x2;
        float y2 = lines[i].y2;

        if (!clipEnabled) {
            glColor3f(1, 0, 0); // red
            drawLine(x1, y1, x2, y2);
        }
        else {
            if (cohenSutherland(x1, y1, x2, y2)) {
                glColor3f(0, 1, 0); // green
                drawLine(x1, y1, x2, y2);
            }
        }
    }

    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c') {
        clipEnabled = true;
        glutPostRedisplay();
    }
    if (key == 'r') {
        clipEnabled = false;
        glutPostRedisplay();
    }
}

void init() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-300, 300, -300, 300);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}