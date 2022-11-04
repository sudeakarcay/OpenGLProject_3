/*

There is a problem in flame effect.

While it is reflecting depending on the angle it is reflecting from different place.

----------
ADDITIONAL FEATURES: 
1- Menu function is additional when you press F1 the game starts.
2- There is a different function to draw flame while rocket flying.
3- When you press c or C, the color of the rocket is changed however just once.
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  4 // Period for the timer.
#define TIMER_ON      1 // 0:disable timer, 1:enable timer



#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, menuDraw=false;
int  winWidth, winHeight; bool game = false, color;
bool pointerOn = false;
// current Window width and height

typedef struct rocket_t {
    vec_t v; int angle;
} rocket_t;
typedef struct fire_t {
    vec_t v; int timeout;
} fire_t;

vec_t mouse;

rocket_t rocket;


vec_t reflect;
fire_t flames[10] = { {0,0} ,0 };

int vrtxX(int x, int y, int a) { return x * cos(a * D2R) - y * sin(a * D2R); }
int vrtxY(int x, int y, int a) { return y * cos(a * D2R) + x * sin(a * D2R); }


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

int oRotateX(int x, int y, int starAngle) { return x * cos(starAngle * D2R) - y * sin(starAngle * D2R); }
int oRotateY(int x, int y, int starAngle) { return y * cos(starAngle * D2R) + x * sin(starAngle * D2R); }




void flameEffect(vec_t v, int a, fire_t* tg) {  //draw flame of the rocket while flying but there is a problem in that function
    tg->v.x = vrtxX(-40, 0, a) + v.x;
    tg->v.y = vrtxY(-40, 0, a) + v.y;
    tg->timeout = 30;
}

void drawRocket(vec_t v, int a) {  //draw player
    glColor3ub(color + 187, color + 255, color + 255);
    glBegin(GL_TRIANGLES);
    glVertex2f(vrtxX(120, 20, a) + v.x, vrtxY(120, 20, a) + v.y);
    glVertex2f(vrtxX(140, 0, a) + v.x, vrtxY(140, 0, a) + v.y);
    glVertex2f(vrtxX(120, -20, a) + v.x, vrtxY(120, -20, a) + v.y);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(vrtxX(120, -20, a) + v.x, vrtxY(120, -20, a) + v.y);
    glVertex2f(vrtxX(0, 0, a) + v.x, vrtxY(0, 0, a) + v.y);
    glVertex2f(vrtxX(0, 0, a) + v.x, vrtxY(0, 0, a) + v.y);
    glVertex2f(vrtxX(120, 20, a) + v.x, vrtxY(120, 20, a) + v.y);
    glEnd();


    glColor3f(color + 255, color + 165, color + 0);

    circle(vrtxX(120, 0, a) + v.x, vrtxY(120, 0, a) + v.y, 10);


    circle(vrtxX(20, 0, a) + v.x, vrtxY(20, 0, a) + v.y, 6);


    circle(vrtxX(40, 0, a) + v.x, vrtxY(40, 0, a) + v.y, 6);


    circle(vrtxX(60, 0, a) + v.x, vrtxY(60, 0, a) + v.y, 6);


    circle(vrtxX(80, 0, a) + v.x, vrtxY(80, 0, a) + v.y, 6);


    circle(vrtxX(100, 0, a) + v.x, vrtxY(100, 0, a) + v.y, 6);


    glBegin(GL_TRIANGLES);
    glVertex2f(vrtxX(0, 0, a) + v.x, vrtxY(0, 0, a) + v.y);
    glVertex2f(vrtxX(-40, 40, a) + v.x, vrtxY(-40, 40, a) + v.y);
    glVertex2f(vrtxX(-40, -40, a) + v.x, vrtxY(-40, -40, a) + v.y);
    glEnd();

    glColor3ub(193, 255, 193);
    glBegin(GL_TRIANGLES);
    glVertex2f(vrtxX(-60, 20, a) + v.x, vrtxY(-60, 20, a) + v.y);
    glVertex2f(vrtxX(-40, 0, a) + v.x, vrtxY(-40, 0, a) + v.y);
    glVertex2f(vrtxX(-60, -20, a) + v.x, vrtxY(-60, -20, a) + v.y);
    glEnd();
}

//vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

//
// To display onto window using OpenGL commands


void menu() //Menu is extra
{
    glColor3ub(135 ,206 ,250);
    glRectf(-700, 350, 700, -350);
    glColor3ub(132 ,112 ,255);
    glRectf(-700, 150, 700, -150);
    glColor3ub(255 ,105 ,180);
    glRectf(-700, 50, 700, -50);

    glColor3ub(0 ,0 ,139);
    vprint(-50, 0, GLUT_BITMAP_HELVETICA_18, "WELCOME");
    vprint(-120, -40, GLUT_BITMAP_HELVETICA_18, "<Press F1 to start the program>");

    
    
}

void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if(!menuDraw)  //to display menu 
    menu();

    else {

        glColor3ub(255 ,130 ,171);
        glRectf(-600, 300, 600, -300);


        glColor3ub(238, 174, 238);
        vprint2(-100, 310, 0.3, "Sude Akarcay");

        vprint2(-600, -340, 0.2, "Spacebar to start/stop");

        vprint2(-200, -340, 0.2, "Mouse %s", pointerOn ? "On" : "Off");

        vprint2(100, -340, 0.2, "Angle : %d", rocket.angle);

        vprint2(400, -340, 0.2, "To change color: c/C");

        drawRocket(rocket.v, rocket.angle);

        if (pointerOn)
        {
            glColor4f(0.5, 0, 0.5, 0.6);
            circle(mouse.x, mouse.y, 5);
        }

        for (int i = 0; i < 10; i++) {   //flame
            glColor4f(0.7, 0, 0, flames[i].timeout * 4 / 100.);
            circle(flames[i].v.x, flames[i].v.y, 20);
        }

    }
    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ') 
        game = !game;

    if (key == 'c' || key == 'C')
        color++;
    

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    }

    if (key == GLUT_KEY_F1)
        menuDraw = true;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.

    

    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.


    int glx = x - winWidth / 2;
    int gly = winHeight / 2 - y;

    mouse.x = glx;
    mouse.y = gly;


    pointerOn = fabs(mouse.x) <= 600 && fabs(mouse.y) <= 300;


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    

    if (game) {


        for (int i = 0; i < 10; i++) {

        
            if (flames[i].timeout == 0) {
                flameEffect(rocket.v, rocket.angle, &flames[i]);
            }
            else {
                flames[i].timeout -= 5;
            }
        }


        if (pointerOn) {

       
            vec_t subtv = subV(mouse, rocket.v);
            vec_t unitv = unitV(subtv);
            reflect = unitv;
            rocket.v = addV(rocket.v, reflect);
            polar_t subang = rec2pol(unitv);
            rocket.angle = subang.angle;
          

        }

        else {

            //reflection

            rocket.v = addV(rocket.v, reflect);
            if (fabs(rocket.v.x) > 480) {
                reflect.x *= -1;
                rocket.angle = (180 - rocket.angle) % 360;
            }
            if (fabs(rocket.v.y) > 175) {
                rocket.angle = (360 - rocket.angle) % 360;
                reflect.y *= -1;

            }
        }
    }




    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Flying Rocket-HW3- Sude Akarcay");
    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}