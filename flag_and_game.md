# Computer Graphics Projects — Structured Notes

## Overview

This repository contains simple computer graphics projects implemented in C++ using OpenGL and GLUT. Each project demonstrates basic graphics programming concepts and interactive features.

---

## Projects

### 1. `2_circle_game_easy.cpp` — Two-Circle Game (Easy)

A simple two-circle game where you control a red circle to avoid collision with a moving blue circle.

**Controls:**
- Arrow keys — Move the red circle
- `R` — Restart the game
- `+` / `-` — Increase / decrease speed
- `ESC` — Exit

**Build & Run:**
```bash
g++ 2_circle_game_easy.cpp -o 2_circle_game_easy -framework OpenGL -framework GLUT
./2_circle_game_easy
```

---

### 2. `national_flag_bd.cpp` — National Flag of Bangladesh

Draws the national flag of Bangladesh using OpenGL pixel plotting. Maintains the official 10:6 aspect ratio. Clicking inside the circle toggles its visibility.

**Build & Run:**
```bash
g++ national_flag_bd.cpp -o national_flag_bd -framework OpenGL -framework GLUT
./national_flag_bd
```

---

## Code Breakdown

### Code 1: `2_circle_game_easy.cpp` (DFS-based Fill)

This version uses a **recursive DFS flood-fill** approach to draw and store circle pixels.

#### Key Global Variables

```cpp
int W = 600, H = 600;         // Window size (resized dynamically)
int L, R, B, T;               // World coordinate boundaries
int ra, rb;                   // Red radius (random), Blue radius = 5*ra
int redcx, redcy;             // Red circle center
int bluecx, bluecy;           // Blue circle center
int redV = 6;                 // Red speed (range: 4..10)
int blueVx, blueVy;           // Blue velocity
bool gameOver = false;

vector<pair<int,int>> redc, bluec;          // Stores pixels of each circle
vector<pair<int,int>> mv = {{-1,0},{0,-1},{1,0},{0,1}};  // DFS directions
map<pair<int,int>, bool> vis;               // Visited map for DFS
```

#### Core Functions

**`fillCircle()` — Recursive DFS Fill**
```cpp
void fillCircle(int x, int y, int xc, int yc, int r, vector<pair<int,int>>& store){
    if(!valid(x,y)) return;
    if((x-xc)*(x-xc) + (y-yc)*(y-yc) > r*r) return;
    if(vis[{x,y}]) return;
    vis[{x,y}] = true;
    store.push_back({x,y});
    setColor(x,y);
    for(auto [dx,dy] : mv) fillCircle(x+dx, y+dy, xc, yc, r, store);
}
```
- Starts from center, recursively visits all 4 neighbors
- Stops at boundary or outside circle equation
- Stores every pixel in `store` vector for later use

**`buildCircleFromCenter()` — Rebuild on Every Frame**
```cpp
void buildCircleFromCenter(int cx, int cy, int r, vector<pair<int,int>>& store){
    store.clear();
    vis.clear();
    glBegin(GL_POINTS);
    fillCircle(cx, cy, cx, cy, r, store);
    glEnd();
}
```

**`moveCircle()` — Translate Stored Pixels**
```cpp
void moveCircle(vector<pair<int,int>>& circle, int dx, int dy){
    for(auto [x,y] : circle)
        temp.push_back(Translate(dx, dy, x, y));
    circle = temp;
}
```

**`collide()` — Center-Based Collision Detection**
```cpp
bool collide(int x1,int y1,int r1,int x2,int y2,int r2){
    long long dx = x1-x2, dy = y1-y2;
    long long dist2 = dx*dx + dy*dy;
    return dist2 <= (long long)(r1+r2)*(r1+r2);
}
```

**`resetGame()`**
- Randomizes `ra` (6–12), sets `rb = 5*ra`
- Calculates minimum window size: `minPlane = 5*rb`
- Places red on left side, blue on right side
- Assigns random velocity to blue (speed 2–4)
- Calls `glutReshapeWindow()` to resize dynamically

**`tick()` — Game Loop (~60 FPS)**
- Moves blue circle by velocity each frame
- Bounces blue off all 4 walls (with radius offset)
- Checks collision after every move

**`display()`**
- Clears screen → draws boundary → draws red → draws blue → draws HUD text

**`spe_key()` — Arrow Key Handler**
- Moves red circle, clamps it inside boundaries, checks collision after move

**`init()` / `reshape()`**
- `init()`: Black background, default ortho projection
- `reshape()`: Sets viewport and calls `gluOrtho2D(L, R, B, T)` to match dynamic window

**Display Mode:** `GLUT_DOUBLE | GLUT_RGB` (double buffering)

---

### Code 2: `2_circle_game_easy.cpp` (Bounding Box Scan)

This version uses a simpler **nested loop scan** over a bounding square to draw circles.

#### Key Difference — `drawAndBuildCircle()`

```cpp
void drawAndBuildCircle(int cx, int cy, int r){
    glBegin(GL_POINTS);
    for(int x = cx - r; x <= cx + r; x++) {
        for(int y = cy - r; y <= cy + r; y++) {
            if(x < L || x > R || y < B || y > T) continue;   // Region check
            long long dx = x - cx, dy = y - cy;
            if(dx*dx + dy*dy <= (long long)r*r)               // Circle equation
                glVertex2i(x, y);
        }
    }
    glEnd();
}
```
- No recursion, no stored pixel vectors, no visited map
- Simpler and more memory-efficient
- Redraws from scratch every frame using the current center

**Display Mode:** `GLUT_SINGLE | GLUT_RGB` (single buffer, uses `glFlush()`)

#### Comparison: DFS Fill vs Bounding Box Scan

| Feature | Code 1 (DFS) | Code 2 (Bounding Box) |
|---|---|---|
| Fill method | Recursive DFS | Nested loop scan |
| Pixel storage | `vector<pair<int,int>>` | None |
| Memory use | Higher | Lower |
| Complexity | Higher | Simpler |
| Display mode | `GLUT_DOUBLE` | `GLUT_SINGLE` |
| Rebuild each frame | Yes (clears vis map) | Yes (loop runs fresh) |

---

### Code 3: `national_flag_bd.cpp`

#### Key Variables

```cpp
int W = 600, H = 360;    // 10:6 aspect ratio
int L = -W/2, R = W/2;
int B = -H/2, T = H/2;
int r = 120;             // 20% of flag width
int cx = -30, cy = 0;   // Circle center (shifted left per spec)
bool flag = false;       // Controls circle visibility
```

#### Drawing Strategy

The flag is drawn using **pixel plotting** — no `GL_POLYGON`, just `GL_POINTS`.

**Green background with circular hole:**
```cpp
void drawRectangleWithHole(){
    // Iterates all pixels in the window
    // Plots only those OUTSIDE the circle equation
    if(dx*dx + dy*dy > (long long)r*r) glVertex2i(x, y);
}
```

**Red circle:**
```cpp
void drawCircle(){
    // Iterates bounding box of circle
    // Plots only those INSIDE the circle equation
    if(dx*dx + dy*dy <= (long long)r*r) glVertex2i(x, y);
}
```

The circle is drawn separately on top only when `flag == true`, which creates a clean hole in the green background.

**Mouse Interaction:**
```cpp
void mouse(int button, int state, int x, int y){
    int winX = x - W/2;
    int winY = H/2 - y;   // Y-axis inversion (GLUT origin: top-left; OpenGL: bottom-left)
    if(checkInsideCircle(winX, winY)){
        flag = !flag;
        glutPostRedisplay();
    }
}
```
Clicking inside the circle toggles `flag`, which shows/hides the red circle.

---

## FreeGLUT / OpenGL Quick Reference

### Typical `main()` Structure

```cpp
int main(int argc, char** argv){
    srand((unsigned)time(nullptr));     // Seed RNG
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Title");

    init();         // OpenGL setup
    resetGame();    // App state setup

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(spe_key);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, tick, 0);

    glutMainLoop();
    return 0;
}
```

### Initialization

```cpp
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(800, 600);
glutCreateWindow("Window Title");
```

**Display Mode Flags:**

| Flag | Description |
|---|---|
| `GLUT_SINGLE` | Single buffer — draws directly to screen |
| `GLUT_DOUBLE` | Double buffer — reduces flicker, preferred for animation |
| `GLUT_RGB` | RGB color mode |
| `GLUT_DEPTH` | Enable depth buffering (3D) |
| `GLUT_STENCIL` | Enable stencil buffering |

### Display Callback

```cpp
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    // drawing code
    glFlush();   // for GLUT_SINGLE
    // glutSwapBuffers(); // for GLUT_DOUBLE
}
glutDisplayFunc(display);
```

### Drawing Primitives

```cpp
glColor3f(1.0, 0.0, 0.0);  // Set color (red)

glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
glEnd();
```

**Common `glBegin` Modes:**

| Mode | Description |
|---|---|
| `GL_POINTS` | Each vertex is a point |
| `GL_LINES` | Pairs of vertices = line segments |
| `GL_LINE_STRIP` | Connected lines |
| `GL_LINE_LOOP` | Connected lines, closes back to start |
| `GL_TRIANGLES` | Every 3 vertices = triangle |
| `GL_TRIANGLE_FAN` | Fan of triangles sharing first vertex |
| `GL_QUADS` | Every 4 vertices = quad (deprecated) |
| `GL_POLYGON` | Single filled convex polygon |

### Keyboard & Special Keys

```cpp
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);  // ESC
}
glutKeyboardFunc(keyboard);

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) { /* ... */ }
}
glutSpecialFunc(specialKeys);
```

### Window Reshape

```cpp
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(L, R, B, T);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
glutReshapeFunc(reshape);
```

> Always call `glMatrixMode(GL_PROJECTION); glLoadIdentity();` before `gluOrtho2D` — otherwise old transforms stack up and corrupt the projection.

> `gluOrtho2D(left, right, bottom, top)` maps world coordinates to the window. Origin is wherever you define it (typically center for these projects).

### Init Function Example

```cpp
void init(){
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(L, R, B, T);
}
```

### Mouse Events

```cpp
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // x, y are in window coords (top-left origin, y down)
        // Convert to OpenGL coords (center origin, y up):
        int winX = x - W/2;
        int winY = H/2 - y;
    }
}
glutMouseFunc(mouse);
```

### Timer Events (Game Loop)

```cpp
void tick(int value) {
    // update logic here
    glutPostRedisplay();
    glutTimerFunc(16, tick, 0);  // reschedule (~60 FPS)
}
glutTimerFunc(0, tick, 0);  // start immediately
```

> `glutTimerFunc` fires only once — you must reschedule inside the callback to create a continuous loop.

---

## Prerequisites

- C++ compiler (e.g., `g++`)
- OpenGL and GLUT libraries (pre-installed on most macOS systems)

---

*This project is for educational purposes.*