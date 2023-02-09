#include <windows.h>
#include <math.h>

#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

#define WIDTH 640
#define HEIGHT 480

using namespace std;

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glClearColor(0, 0, 0, 1);
}

static void drawPoint(int x, int y)
{
    glPointSize(2.0f);
    glColor3d(1,1,1);

    glBegin(GL_POINTS);
        glVertex2f(x, y);
    glEnd();
}

static void circlePlotPoints(int xc, int yc, int x, int y) {
    drawPoint(xc + x, HEIGHT - (yc + y));
    drawPoint(xc - x, HEIGHT - (yc + y));
    drawPoint(xc + x, HEIGHT - (yc - y));
    drawPoint(xc - x, HEIGHT - (yc - y));
    drawPoint(xc + y, HEIGHT - (yc + x));
    drawPoint(xc - y, HEIGHT - (yc + x));
    drawPoint(xc + y, HEIGHT - (yc - x));
    drawPoint(xc - y, HEIGHT - (yc - x));
    glFlush();
}

static void midpointCircle(int button, int state, int xx, int yy)
{
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);

            glFlush();
        }
    } else {
        int xc = points[0][0], yc = points[0][1], xr = points[1][0], yr = points[1][1];
        points.clear();

        int r = sqrt((xc - xr) * (xc - xr) + (yc - yr) * (yc - yr));
        int p = 1 - r;
        int x = 0, y = r;

        circlePlotPoints(xc, yc, x, y);

        while(x < y) {
            cout<<"x = "<<x<<"  y = "<<y<<"\n";
            x += 1;
            if(p < 0) {
                p += 2 * x + 1;
            } else {
                y -= 1;
                p += 2 * (x - y) + 1;
            }
            circlePlotPoints(xc, yc, x, y);
        }
    }
}

static void keyboardHandler(unsigned char key, int xx, int yy)
{
    switch(key) {

    case '1':
        cout<<"Mid - Point Circle Algorithm\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glFlush();
        glutMouseFunc(midpointCircle);
        break;
    }
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    cout<<"1. Mid-Point Circle\n\n";
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(WIDTH / 4, HEIGHT / 4);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("CA1");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);

    glutMainLoop();

    return EXIT_SUCCESS;
}
