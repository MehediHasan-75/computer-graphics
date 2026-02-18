#include <bits/stdc++.h>
#include <GLUT/glut.h>
using namespace std;

// ------------------- CONFIG / GLOBAL -------------------
int W = 600, H = 600;     // window (will be resized based on ra)
int L, R, B, T;           // region line boundary in world coords

int ra;                   // red radius (random)
int rb;                   // blue radius = 5*ra

int redcx, redcy;         // red center
int bluecx, bluecy;       // blue center

int redV = 6;             // red speed (4..10) (controlled)
int blueVx, blueVy;       // blue velocity

bool gameOver = false;

// store pixels of circles
vector<pair<int,int>> redc, bluec;

// movement directions for DFS fill
vector<pair<int,int>> mv = { {-1,0}, {0,-1}, {1,0}, {0,1} };

// visited map used inside fillCircle
map<pair<int,int>, bool> vis;

// ------------------- UTILS -------------------
int rnd(int a, int b){
    return a + rand() % (b - a + 1);
}

bool valid(int x, int y){
    return (x >= L && x <= R && y >= B && y <= T);
}

void setColor(int x, int y){
    glVertex2i(x, y);
}

pair<int,int> Translate(int dx, int dy, int x, int y){
    return {x + dx, y + dy};
}

// distance check for collision between two circles
bool collide(int x1,int y1,int r1,int x2,int y2,int r2){
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    long long dist2 = dx*dx + dy*dy;
    long long rr = (long long)(r1 + r2);
    return dist2 <= rr*rr;
}

// ------------------- FILL CIRCLE-------------------
void fillCircle(int x, int y, int xc, int yc, int r, vector<pair<int,int>>& store){
    if(!valid(x,y)) return;

    // outside circle
    if( (x-xc)*(x-xc) + (y-yc)*(y-yc) > r*r ) return;

    if(vis[{x,y}]) return;
    vis[{x,y}] = true;

    store.push_back({x,y});
    setColor(x,y);

    for(auto [dx,dy] : mv){
        fillCircle(x+dx, y+dy, xc, yc, r, store);
    }
}

// ------------------- MOVE CIRCLE PIXELS -------------------
void moveCircle(vector<pair<int,int>>& circle, int dx, int dy){
    vector<pair<int,int>> temp;
    temp.reserve(circle.size());
    for(auto [x,y] : circle){
        auto it = Translate(dx, dy, x, y);
        temp.push_back(it);
    }
    circle = temp;
}

// ------------------- REBUILD CIRCLE FROM CENTER -------------------
void buildCircleFromCenter(int cx, int cy, int r, vector<pair<int,int>>& store){
    store.clear();
    vis.clear();
    // start fill from center
    glBegin(GL_POINTS);
    fillCircle(cx, cy, cx, cy, r, store);
    glEnd();
}

// ------------------- INIT / RESET GAME -------------------
void resetGame(){
    gameOver = false;

    // ra randomized (you can change range)
    ra = rnd(6, 12);         // example: 6..12
    rb = 5 * ra;             // rb = 5ra

    // plane min = 5*rb = 25*ra
    int minPlane = 5 * rb;

    // choose window at least that
    W = max(600, minPlane);
    H = max(600, minPlane);

    // set region line boundary (world coords centered)
    L = -W/2; R = W/2;
    B = -H/2; T = H/2;

    // place red and blue centers inside region
    redcx  = L + rb + 30;   // keep far from blue
    redcy  = 0;

    bluecx = R - rb - 30;
    bluecy = 0;

    // random blue velocity (2..4)
    int sp = rnd(2, 4);
    int sx = (rnd(0,1) ? 1 : -1);
    int sy = (rnd(0,1) ? 1 : -1);
    blueVx = sx * sp;
    blueVy = sy * sp;

    // clamp red speed 4..10
    redV = max(4, min(10, redV));

    glutReshapeWindow(W, H);
    glutPostRedisplay();
}

// ------------------- DRAW BOUNDARY (REGION LINE) -------------------
void drawRegion(){
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
        glVertex2i(L,B);
        glVertex2i(R,B);
        glVertex2i(R,T);
        glVertex2i(L,T);
    glEnd();
}

// ------------------- DISPLAY -------------------
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    // boundary
    drawRegion();

    // draw red circle
    glPointSize(2);
    glColor3f(1,0,0);
    buildCircleFromCenter(redcx, redcy, ra, redc);

    // draw blue circle
    glPointSize(2);
    glColor3f(0,0,1);
    buildCircleFromCenter(bluecx, bluecy, rb, bluec);

    // game over text (simple)
    if(gameOver){
        glColor3f(1,1,0);
        glRasterPos2i(L + 30, T - 40);
        string s = "GAME OVER! Press R to Restart";
        for(char c: s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }else{
        glColor3f(1,1,0);
        glRasterPos2i(L + 30, T - 40);
        string s = "ra=" + to_string(ra) + " rb=" + to_string(rb) +
                   " redV=" + to_string(redV) + " (2..4)";
        for(char c: s) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    glFlush();
}

// ------------------- BLUE MOTION + BOUNCE -------------------
void tick(int){
    if(!gameOver){
        // move blue center
        bluecx += blueVx;
        bluecy += blueVy;

        // bounce with radius check
        if(bluecx - rb <= L){
            bluecx = L + rb;
            blueVx *= -1;
        }
        if(bluecx + rb >= R){
            bluecx = R - rb;
            blueVx *= -1;
        }
        if(bluecy - rb <= B){
            bluecy = B + rb;
            blueVy *= -1;
        }
        if(bluecy + rb >= T){
            bluecy = T - rb;
            blueVy *= -1;
        }

        // collision check (center-based)
        if(collide(redcx, redcy, ra, bluecx, bluecy, rb)){
            gameOver = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, tick, 0); // ~60 fps
}

// ------------------- KEYS (RED MOVE + SPEED CLAMP) -------------------
void spe_key(int key, int, int){
    if(gameOver) return;

    if(key == GLUT_KEY_LEFT)  redcx -= redV;
    if(key == GLUT_KEY_RIGHT) redcx += redV;
    if(key == GLUT_KEY_UP)    redcy += redV;
    if(key == GLUT_KEY_DOWN)  redcy -= redV;

    // keep red inside region (with radius)
    redcx = max(L + ra, min(R - ra, redcx));
    redcy = max(B + ra, min(T - ra, redcy));

    // collision check after move
    if(collide(redcx, redcy, ra, bluecx, bluecy, rb)){
        gameOver = true;
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int){
    if(key == 27) exit(0);

    // restart
    if(key == 'r' || key == 'R'){
        resetGame();
        return;
    }


    // change red speed (min 4 max 10)
    if(key == '+') redV++;
    if(key == '-') redV--;
    redV = max(4, min(10, redV));

    glutPostRedisplay();
}

// ------------------- INIT -------------------
void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Ortho will be set by reshape (but keep default)
    gluOrtho2D(-300,300,-300,300);
}

void reshape(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
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
    glutInitWindowPosition(0,0);
    glutCreateWindow("Circle Game - Your Style");

    init();
    resetGame();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(keyboard);

    glutTimerFunc(16, tick, 0);

    glutMainLoop();
    return 0;
}