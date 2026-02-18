#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <algorithm>

// Cross-platform GLUT include
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

// ------------------- CONFIG / GLOBAL -------------------
int W = 600, H = 600;     // window size
int L, R, B, T;           // region line boundary

int ra;                   // red radius
int rb;                   // blue radius

int redcx, redcy;         // red center
int bluecx, bluecy;       // blue center

int redV = 6;             // red speed
int blueVx, blueVy;       // blue velocity

bool gameOver = false;


// ------------------- UTILS -------------------
int rnd(int a, int b){
    return a + rand() % (b - a + 1);
}

// Distance check for collision between two circles
bool collide(int x1, int y1, int r1, int x2, int y2, int r2){
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    long long dist2 = dx*dx + dy*dy;
    long long rr = (long long)(r1 + r2);
    return dist2 <= rr*rr;
}

// ------------------- DRAW CIRCLE -------------------
// It iterates through a square box around the center and draws pixels inside the radius.
void drawAndBuildCircle(int cx, int cy, int r){
    
    glBegin(GL_POINTS);
    
    // Iterate from left-bound to right-bound of the circle
    for(int x = cx - r; x <= cx + r; x++) {
        // Iterate from bottom-bound to top-bound
        for(int y = cy - r; y <= cy + r; y++) {
            
            // 1. Check if pixel is inside the allowed Region (L, R, B, T)
            if(x < L || x > R || y < B || y > T) continue;

            // 2. Check if pixel is inside the Circle: dx^2 + dy^2 <= r^2
            long long dx = x - cx;
            long long dy = y - cy;
            
            if(dx*dx + dy*dy <= (long long)r*r) {
                glVertex2i(x, y);       // Draw the pixel
             }
        }
    }
    glEnd();
}

// ------------------- DISPLAY -------------------
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Red Circle
    glColor3f(1,0,0);
    drawAndBuildCircle(redcx, redcy, ra);

    // Draw Blue Circle
    glColor3f(0,0,1);
    drawAndBuildCircle(bluecx, bluecy, rb);

    // Draw Text
    glColor3f(1,1,0); // Yellow text
    glRasterPos2i(L + 20, T - 30);
    
    string s;
    if(gameOver){
        s = "GAME OVER! Press R to Restart";
    } else {
        s = "ra=" + to_string(ra) + " rb=" + to_string(rb) +
            " Speed=" + to_string(redV) + " (+/- to change)";
    }
    
    for(char c: s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glFlush();
 }

// ------------------- INIT / RESET GAME -------------------
void resetGame(){
    gameOver = false;

    // Radius setup
    ra = rnd(6, 12);
    rb = 5 * ra;

    // Determine boundaries based on circle sizes
    int minPlane = 5 * rb;
    W = max(600, minPlane);
    H = max(600, minPlane);

    L = -W/2; R = W/2;
    B = -H/2; T = H/2;

    // Set positions
    redcx  = L + rb + 30;
    redcy  = 0;

    bluecx = R - rb - 30;
    bluecy = 0;

    // Random velocity for blue
    int sp = rnd(2, 4);
    blueVx = (rnd(0,1) ? 1 : -1) * sp;
    blueVy = (rnd(0,1) ? 1 : -1) * sp;

    // Clamp red speed
    redV = max(4, min(10, redV));

    // Apply reshape to ensure coordinate system matches new W/H
    glutReshapeWindow(W, H);
}

// ------------------- ANIMATION / TICK -------------------
void tick(int){
    if(!gameOver){
        // Move Blue
        bluecx += blueVx;
        bluecy += blueVy;

        // Bounce Blue (checking radius so it stays inside)
        if(bluecx - rb <= L){ bluecx = L + rb; blueVx *= -1; }
        if(bluecx + rb >= R){ bluecx = R - rb; blueVx *= -1; }
        if(bluecy - rb <= B){ bluecy = B + rb; blueVy *= -1; }
        if(bluecy + rb >= T){ bluecy = T - rb; blueVy *= -1; }

        // Collision Check
        if(collide(redcx, redcy, ra, bluecx, bluecy, rb)){
            gameOver = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, tick, 0); // ~60 FPS
}

// ------------------- INPUT HANDLING -------------------
void spe_key(int key, int, int){
    if(gameOver) return;

    if(key == GLUT_KEY_LEFT)  redcx -= redV;
    if(key == GLUT_KEY_RIGHT) redcx += redV;
    if(key == GLUT_KEY_UP)    redcy += redV;
    if(key == GLUT_KEY_DOWN)  redcy -= redV;

    // Keep Red inside boundaries
    redcx = max(L + ra, min(R - ra, redcx));
    redcy = max(B + ra, min(T - ra, redcy));

    // Check collision immediately after move
    if(collide(redcx, redcy, ra, bluecx, bluecy, rb)){
        gameOver = true;
    }
}

void keyboard(unsigned char key, int, int){
    if(key == 27) exit(0); // ESC to quit

    if(key == 'r' || key == 'R'){
        resetGame();
    }

    if(key == '+') redV++;
    if(key == '-') redV--;
    redV = max(4, min(10, redV));
}

// ------------------- OPENGL SETUP -------------------
void init(){
    glClearColor(0,0,0,1); // Black background
}

void reshape(int w, int h){
    // Prevent divide by zero
    if (h == 0) h = 1;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set coordinate system to match L, R, B, T
    gluOrtho2D(L, R, B, T);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ------------------- MAIN -------------------
int main(int argc, char** argv){
    srand((unsigned)time(nullptr));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    
    glutInitWindowSize(W, H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Circle Game.");

    init();
    resetGame(); // Sets up coordinates and radii

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, tick, 0);

    glutMainLoop();
    return 0;
}