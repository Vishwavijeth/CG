#include <windows.h>
#include <math.h>

#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <vector>

#define WIDTH 640
#define HEIGHT 480
#define DELTA 5

using namespace std;

float x = WIDTH / 2, y = HEIGHT / 2;

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glClearColor(0, 0, 0, 1);
}

static void drawPoint(int x, int y)
{
    // glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
        glPointSize(5.0f);
        glColor3d(1,1,1);
        glVertex2f(x, y);
    glEnd();

    glFlush();
}

static void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int r, int g, int b) {
    glColor3ub(r, g, b);

    glBegin(GL_TRIANGLES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
    glEnd();
}

static void drawLine(int x1, int y1, int x2, int y2)
{
    glPointSize(5.0f);
    glColor3d(1,1,1);

    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
}

static void drawBall(float cx, float cy, float r) {
    int num_segments = 100;

    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float xx = r * cosf(theta);//calculate the x component
        float yy = r * sinf(theta);//calculate the y component
        glVertex2f(xx + cx, yy + cy);//output vertex
    }
    glEnd();
}

static void drawQuad(float x1, float y1, float x2, float y2, int r, int g, int b) {
    glColor3ub(r, g, b);

    glBegin(GL_POLYGON);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

static void drawPolygon(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int r, int g, int b) {
    glColor3ub(r, g, b);

    glBegin(GL_POLYGON);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glVertex2f(x3, y3);
        glVertex2f(x4, y4);
    glEnd();
}

static void drawHouse() {
    glClear(GL_COLOR_BUFFER_BIT);

    float x = WIDTH / 2, y = HEIGHT / 2;

    // Sky
    drawQuad(0, 0, WIDTH, HEIGHT, 110, 229, 225);
    // Ground
    drawQuad(0, y - 100, WIDTH, 0, 48, 218, 57);

    // Building
    drawQuad(x - 150, y - 100, x + 150, y + 75, 255, 200, 0);
    // Roof
    drawPolygon(x - 175, y + 75, x + 175, y + 75, x + 150, y + 160, x - 150, y + 160, 255, 0, 0);
    // Chimney
    drawQuad(x - 140, y + 180, x - 110, y + 160, 255, 200, 0);
    drawQuad(x - 143, y + 190 , x - 107, y + 180, 255, 200, 0);
    // First Floor
    drawQuad(x, y + 170, x + 120, y + 105, 255, 200, 0);
    drawTriangle(x - 10, y + 170, x + 130, y + 170, x + 60, y + 210, 255, 0, 0);
    drawTriangle(x, y + 170, x + 120, y + 170, x + 60, y + 200, 255, 200, 0);
    // Door
    drawQuad(x + 60, y - 100, x + 110, y - 20, 155, 53, 27);
    // Steps
    drawQuad(x + 55, y - 100, x + 115, y - 105, 255, 53, 27);
    drawQuad(x + 50, y - 105, x + 120, y - 110, 255, 60, 30);
    // Windows
    //52 219 213
    drawQuad(x - 130, y + 40, x - 70, y - 20, 52, 219, 213);
    drawQuad(x - 50, y + 40, x + 10, y - 20, 52, 219, 213);
    drawQuad(x + 40, y + 160, x + 80, y + 120, 52, 219, 213);

    glFlush();
}

static void mouseHandler(int button, int state, int xx, int yy)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, 480 - yy);
        cout<<"("<<xx<<", "<<yy<<")\n";
    }
}

static void keyboardHandler(unsigned char key, int xx, int yy)
{
    switch(key) {
    case 'a':
        cout<<"a\n";
        break;
    }

    cout<<key<<"\n";
}

static void specialKeyHandler(int key, int xx, int yy)
{
    // None
}

static void display(void)
{
    drawHouse();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("House");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouseHandler);
    glutKeyboardFunc(keyboardHandler);
    glutSpecialFunc(specialKeyHandler);

    glutMainLoop();

    return EXIT_SUCCESS;
}
