#ifdef __APPLE__
#include <GLUT/glut.h>   // macOS
#else
#include <GL/glut.h>     // Windows / Linux
#endif

/* =========================================================
   Program: National Flag of Bangladesh (Interactive)
   Description:
   - Draws the Bangladesh flag using pixel plotting.
   - Maintains official 10:6 aspect ratio.
   - Allows user interaction: clicking inside the circle
     toggles its visibility.
   ========================================================= */

// ---------------- Window Configuration ----------------

// Window dimensions (10:6 ratio)
int W = 600;
int H = 360;

// Orthographic coordinate boundaries
int L = -W/2, R = W/2;   // Left and Right limits
int B = -H/2, T = H/2;   // Bottom and Top limits

// ---------------- Circle Configuration ----------------

// Circle radius (20% of flag width)
int r = 120;

// Circle center (slightly shifted left as per flag specification)
int cx = -30;
int cy = 0;

// Boolean flag to control circle visibility
bool flag = false;

/* =========================================================
   Function: drawCircle()
   Purpose : Draws a filled circle using pixel plotting.
   Method  : Scans bounding square and applies circle equation:
             (x - cx)^2 + (y - cy)^2 <= r^2
   ========================================================= */
void drawCircle(){
    glBegin(GL_POINTS);

    // Iterate through bounding box of circle
    for(int x = cx - r; x <= cx + r; x++){
        for(int y = cy - r; y <= cy + r; y++){

            long long dx = x - cx;
            long long dy = y - cy;

            // Plot point only if it lies inside circle boundary
            if(dx*dx + dy*dy <= (long long)r*r){
                glVertex2i(x, y);
            }
        }
    }

    glEnd();
}

/* =========================================================
   Function: drawRectangleWithHole()
   Purpose : Draws the green flag background while leaving
             the circular region empty.
   Method  : Plots points only outside the circle equation.
   ========================================================= */
void drawRectangleWithHole(){
    glBegin(GL_POINTS);

    // Iterate through entire rectangular area
    for(int x = L; x <= R; x++){
        for(int y = B; y <= T; y++){

            long long dx = x - cx;
            long long dy = y - cy;

            // Plot only if point lies outside circle
            if(dx*dx + dy*dy > (long long)r*r){
                glVertex2i(x, y);
            }
        }
    }

    glEnd();
}

/* =========================================================
   Function: checkInsideCircle()
   Purpose : Determines whether a given point lies inside
             the circle.
   Returns : true  -> point inside circle
             false -> point outside circle
   ========================================================= */
bool checkInsideCircle(int x, int y){
    long long dx = x - cx;
    long long dy = y - cy;

    return dx*dx + dy*dy <= (long long)r*r;
}

/* =========================================================
   Function: display()
   Purpose : Rendering callback function.
             Clears screen and redraws entire scene.
   ========================================================= */
void display(){
    // Clear the screen using background color
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw green background (with circular hole)
    glColor3f(0.0f, 0.42f, 0.31f);  // Official bottle green
    drawRectangleWithHole();

    // Draw red circle only if flag is true
    if(flag){
        glColor3f(0.96f, 0.16f, 0.25f);  // Official flag red
        drawCircle();
    }

    // Ensure all drawing commands are executed
    glFlush();
}

/* =========================================================
   Function: mouse()
   Purpose : Handles mouse click events.
             Toggles circle visibility when user clicks
             inside the circular region.
   ========================================================= */
void mouse(int button, int state, int x, int y){

    // Check for left mouse button press
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        // Convert window coordinates (top-left origin)
        // to OpenGL coordinates (center origin)
        int winX = x - W/2;
        int winY = H/2 - y;   // Y-axis inversion

        // If click occurred inside circle, toggle flag
        if(checkInsideCircle(winX, winY)){
            flag = !flag;

            // Request screen redraw
            glutPostRedisplay();
        }
    }
}

/* =========================================================
   Function: init()
   Purpose : Performs one-time OpenGL setup.
   ========================================================= */
void init(){

    // Set background (clear) color to white
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Select projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set orthographic 2D projection
    // Defines coordinate system with (0,0) at center
    gluOrtho2D(L, R, B, T);
}

/* =========================================================
   Function: main()
   Purpose : Program entry point.
             Initializes GLUT and starts event loop.
   ========================================================= */
int main(int argc, char** argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(W, H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("National Flag of Bangladesh");

    init();

    // Register callback functions
    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    // Enter GLUT event-processing loop
    glutMainLoop();

    return 0;
}
