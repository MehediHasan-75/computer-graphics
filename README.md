# FreeGLUT Basic Syntax Reference

## Typical `main()` Structure
```cpp
int main(int argc, char** argv){
    srand((unsigned)time(nullptr)); // Seed random number generator

    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode (single buffer, RGB color)

    glutInitWindowSize(W, H); // Set initial window size
    glutInitWindowPosition(100, 100); // Set initial window position
    glutCreateWindow("Circle Game."); // Create window with title

    init(); // Custom function: OpenGL/GLUT setup (background, projection, etc.)
    resetGame(); // Custom function: Initialize or reset game state/variables

    glutDisplayFunc(display); // Register display callback (draws the scene)
    glutReshapeFunc(reshape); // Register reshape callback (handles window resize)
    glutSpecialFunc(spe_key); // Register special keys callback (arrow keys, etc.)
    glutKeyboardFunc(keyboard); // Register keyboard callback (ASCII keys)
    glutTimerFunc(0, tick, 0); // Set up timer callback for animation/game loop

    glutMainLoop(); // Enter the main event-processing loop
    return 0;
}
```

- `srand((unsigned)time(nullptr));` — Seeds the random number generator for any random operations.
- `glutInit(&argc, argv);` — Initializes the GLUT library.
- `glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);` — Sets the display mode to single buffering and RGB color.
- `glutInitWindowSize(W, H);` — Sets the initial window size.
- `glutInitWindowPosition(100, 100);` — Sets the initial window position on the screen.
- `glutCreateWindow("...");` — Creates a window with the specified title.
- `init();` — User-defined function to set up OpenGL state (background color, projection, etc.).
- `resetGame();` — User-defined function to initialize or reset game/application state.
- `glutDisplayFunc(display);` — Registers the display callback function, which draws the scene.
- `glutReshapeFunc(reshape);` — Registers the reshape callback, which handles window resizing.
- `glutSpecialFunc(spe_key);` — Registers the special keys callback (for arrow keys, function keys, etc.).
- `glutKeyboardFunc(keyboard);` — Registers the keyboard callback (for standard ASCII keys).
- `glutTimerFunc(0, tick, 0);` — Sets a timer to call the tick function after 0 ms (starts animation/game loop).
- `glutMainLoop();` — Starts the main event-processing loop, handling all input and rendering events.

---

## Initialization
```cpp
glutInit(&argc, argv);                         // Initialize GLUT
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);   // Set display mode
glutInitWindowSize(800, 600);                  // Set window size
glutCreateWindow("Window Title");              // Create window
```

*`glutInitDisplayMode` sets the display mode for the window. You can combine multiple flags using the bitwise OR operator (`|`).*

**Common Flags:**
- `GLUT_SINGLE` — Use a single buffer for drawing (draws directly to the screen).
- `GLUT_DOUBLE` — Use double buffering (draws to a back buffer, then swaps to the screen; reduces flicker, preferred for animation).
- `GLUT_RGB` — Use RGB color mode.
- `GLUT_RGBA` — Same as `GLUT_RGB` (alias).
- `GLUT_INDEX` — Use color index mode (rarely used).
- `GLUT_DEPTH` — Enable depth buffering (for 3D rendering).
- `GLUT_STENCIL` — Enable stencil buffering.

---

## Main Loop
```cpp
glutMainLoop();                       // Enter the event-processing loop
```

*`glutMainLoop()` starts an infinite event-processing loop. It waits for events (like window redraw, keyboard, or mouse input) and dispatches them to the appropriate callback functions (e.g., display, keyboard, mouse).*

*On program start, the OS emits an initial event that triggers the display function. Afterwards, any user input (key or mouse) is tracked by GLUT and the corresponding callback is executed. If you want to manually request a redraw (for example, after updating your scene), call `glutPostRedisplay();` to schedule the display function to run again.*

---

## Display Callback
```cpp
void display() {
    glClear(GL_COLOR_BUFFER_BIT);     // Clear the screen
    // Drawing code here
    glFlush();                        // Render now
}
glutDisplayFunc(display);             // Register display callback
```

---

## Drawing Primitives
```cpp
glBegin(GL_POLYGON);                  // Start drawing a polygon
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
glEnd();

glColor3f(1.0, 0.0, 0.0);            // Set color (red)
```

*`glBegin(mode)` and `glEnd()` are used to specify the start and end of a sequence of vertices that define a geometric primitive (such as points, lines, triangles, or polygons). All vertex commands between these calls are grouped together to form the specified shape. For example, `GL_POLYGON` groups the vertices into a filled polygon. Always pair each `glBegin` with a corresponding `glEnd`.*

**Common `glBegin` Primitive Types:**
- `GL_POINTS` — Each vertex is a single point.
- `GL_LINES` — Every pair of vertices forms a separate line segment.
- `GL_LINE_STRIP` — Connected group of line segments from the first vertex to the last.
- `GL_LINE_LOOP` — Same as `GL_LINE_STRIP`, but also connects the last vertex back to the first.
- `GL_TRIANGLES` — Every group of three vertices forms a separate triangle.
- `GL_TRIANGLE_STRIP` — Connected strip of triangles sharing vertices.
- `GL_TRIANGLE_FAN` — Connected fan of triangles sharing the first vertex.
- `GL_QUADS` — Every group of four vertices forms a separate quadrilateral (deprecated in modern OpenGL).
- `GL_POLYGON` — All vertices form a single filled polygon (convex only).

---

## Keyboard and Special Keys
```cpp
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);           // ESC to exit
}
glutKeyboardFunc(keyboard);           // Register keyboard callback

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) { /* ... */ }
}
glutSpecialFunc(specialKeys);         // Register special keys callback
```

---

## Window Reshape
```cpp
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
}
glutReshapeFunc(reshape);
```

*Window reshaping logic ensures your graphics adapt to changes in window size. When the window is resized, GLUT calls your reshape function so you can update the viewport and projection matrix to match the new dimensions. Typically, you use:*

- `glViewport(0, 0, w, h);` — Sets the drawing area to the new window size.
- `glMatrixMode(GL_PROJECTION); glLoadIdentity();` — Resets the projection matrix.
- `gluOrtho2D(0, w, 0, h);` — Sets up a new 2D coordinate system matching the window.

*This prevents distortion or clipping and ensures your drawing scales or adapts correctly when the user resizes the window. If you don't update the viewport and projection, your graphics may appear stretched, squished, or clipped after resizing.*

*If you do not call `glMatrixMode(GL_PROJECTION); glLoadIdentity();` before setting up your projection (like `gluOrtho2D`), your projection matrix may not be set correctly. This can cause the coordinate system to be incorrect, old transformations to remain, and your drawing to appear distorted, misplaced, or not at all. These calls ensure you are modifying the correct matrix and starting from a known state before applying your custom projection.*

*`gluOrtho2D(left, right, bottom, top)` sets up a 2D orthographic projection matrix for OpenGL rendering. It defines a coordinate system where (left, bottom) is the lower-left corner and (right, top) is the upper-right corner of the window. All drawing will be mapped to this coordinate system, making it ideal for 2D graphics.*

---

## Init Function
```cpp
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
```

---

## Mouse Events
```cpp
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Left mouse button pressed at (x, y)
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Right mouse button pressed at (x, y)
    }
}
glutMouseFunc(mouse); // Register mouse callback

// button: GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, etc.
// state: GLUT_DOWN (pressed), GLUT_UP (released)
// x, y: Mouse position in window coordinates
```

*When handling mouse events, GLUT provides (x, y) in window coordinates, where (0, 0) is the top-left and y increases downward. If your OpenGL scene uses a centered coordinate system (e.g., (0, 0) at the center, y up), you must convert coordinates:*

```cpp
int winX = x - W/2;      // Shift origin to center (x)
int winY = H/2 - y;      // Invert y-axis and shift origin to center (y)
```

*This ensures mouse clicks correspond to your OpenGL drawing coordinates, so you can accurately detect clicks inside shapes (like a circle at the center).*

---

## Timer Events
```cpp
// Sets a one-time timer to call the function 'tick' after 0 milliseconds (immediately)
glutTimerFunc(0, tick, 0);

// Usage:
// void tick(int value) {
//     // Code to execute on timer event
// }
// glutTimerFunc(milliseconds, tick, value);
```

*`glutTimerFunc` schedules a callback to the given function after the specified number of milliseconds. The callback is called only once unless rescheduled inside the function (commonly used for animation loops).*