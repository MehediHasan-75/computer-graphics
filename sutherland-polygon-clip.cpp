#include <GLUT/glut.h>
#include <iostream>
#include <vector>
using namespace std;

// Clipping window
float xmin = -100, xmax = 100, ymin = -100, ymax = 100;

bool clipEnabled = false;

// Point structure
struct Point {
    float x, y;
};

// Original polygon
vector<Point> polygon = {
    {-150, 50},
    {150, 50},
    {120, 150},
    {-120, 150}
};

// Additional test polygons (can switch between them)
vector<Point> polygon2 = {
    {-50, -150},
    {50, -150},
    {80, 0},
    {0, 120},
    {-80, 0}
};

vector<Point> polygon3 = {
    {-200, -50},
    {-50, -200},
    {50, -100},
    {0, -50}
};

vector<Point> currentPolygon;
int polygonIndex = 0;

// Edge codes
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

// Check if point is inside edge
bool isInside(Point p, int edge) {
    switch(edge) {
        case LEFT:
            return p.x >= xmin;
        case RIGHT:
            return p.x <= xmax;
        case BOTTOM:
            return p.y >= ymin;
        case TOP:
            return p.y <= ymax;
    }
    return false;
}

// Calculate intersection point with edge
Point getIntersection(Point p1, Point p2, int edge) {
    Point intersection;
    float m;

    if (p2.x != p1.x)
        m = (p2.y - p1.y) / (p2.x - p1.x);

    switch(edge) {
        case LEFT:
            intersection.x = xmin;
            intersection.y = p1.y + m * (xmin - p1.x);
            break;
        case RIGHT:
            intersection.x = xmax;
            intersection.y = p1.y + m * (xmax - p1.x);
            break;
        case BOTTOM:
            intersection.y = ymin;
            if (p2.x != p1.x)
                intersection.x = p1.x + (ymin - p1.y) / m;
            else
                intersection.x = p1.x;
            break;
        case TOP:
            intersection.y = ymax;
            if (p2.x != p1.x)
                intersection.x = p1.x + (ymax - p1.y) / m;
            else
                intersection.x = p1.x;
            break;
    }

    return intersection;
}

// Clip polygon against one edge
vector<Point> clipAgainstEdge(vector<Point> input, int edge) {
    vector<Point> output;

    if (input.size() == 0) return output;

    Point prevPoint = input[input.size() - 1];

    for (int i = 0; i < input.size(); i++) {
        Point currentPoint = input[i];

        bool prevInside = isInside(prevPoint, edge);
        bool currInside = isInside(currentPoint, edge);

        if (currInside) {
            if (!prevInside) {
                // Entering: add intersection then current
                output.push_back(getIntersection(prevPoint, currentPoint, edge));
            }
            output.push_back(currentPoint);
        }
        else if (prevInside) {
            // Leaving: add intersection only
            output.push_back(getIntersection(prevPoint, currentPoint, edge));
        }

        prevPoint = currentPoint;
    }

    return output;
}

// Sutherland-Hodgman polygon clipping
vector<Point> sutherlandHodgman(vector<Point> polygon) {
    vector<Point> output = polygon;

    // Clip against all four edges
    output = clipAgainstEdge(output, LEFT);
    output = clipAgainstEdge(output, RIGHT);
    output = clipAgainstEdge(output, BOTTOM);
    output = clipAgainstEdge(output, TOP);

    return output;
}

void drawWindow() {
    glColor3f(0, 0, 1);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
    glLineWidth(1.0);
}

void drawPolygon(vector<Point> poly, bool fill = false) {
    if (poly.size() < 3) return;

    if (fill) {
        glBegin(GL_POLYGON);
    } else {
        glBegin(GL_LINE_LOOP);
    }

    for (int i = 0; i < poly.size(); i++) {
        glVertex2f(poly[i].x, poly[i].y);
    }

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawWindow();

    if (!clipEnabled) {
        // Draw original polygon in red
        glColor3f(1, 0, 0); // red
        drawPolygon(currentPolygon);
    }
    else {
        // Draw clipped polygon in green (filled)
        vector<Point> clipped = sutherlandHodgman(currentPolygon);
        
        if (clipped.size() >= 3) {
            glColor3f(0.7, 1, 0.7); // light green fill
            drawPolygon(clipped, true);
            
            glColor3f(0, 1, 0); // green outline
            glLineWidth(2.0);
            drawPolygon(clipped, false);
            glLineWidth(1.0);
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
    if (key == '1') {
        currentPolygon = polygon;
        polygonIndex = 0;
        glutPostRedisplay();
    }
    if (key == '2') {
        currentPolygon = polygon2;
        polygonIndex = 1;
        glutPostRedisplay();
    }
    if (key == '3') {
        currentPolygon = polygon3;
        polygonIndex = 2;
        glutPostRedisplay();
    }
    if (key == 27) { // ESC key
        exit(0);
    }
}

void init() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-300, 300, -300, 300);
    currentPolygon = polygon;
}

void printInstructions() {
    cout << "\n=== Sutherland-Hodgman Polygon Clipping ===" << endl;
    cout << "Controls:" << endl;
    cout << "  'c' - Enable clipping" << endl;
    cout << "  'r' - Reset (show original polygon)" << endl;
    cout << "  '1' - Switch to polygon 1" << endl;
    cout << "  '2' - Switch to polygon 2" << endl;
    cout << "  '3' - Switch to polygon 3" << endl;
    cout << "  ESC - Exit" << endl;
    cout << "==========================================\n" << endl;
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping");

    init();
    printInstructions();
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
