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

static void bresenham(int button, int state, int xx, int yy) {
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);
            cout<<"("<<xx<<", "<<yy<<")\n\n";

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);
        }
    }
    if(points.size() == 2) {
        int x1 = points[0][0], y1 = points[0][1], x2 = points[1][0], y2 = points[1][1];
        points.clear();

        int dx, dy, x, y, s1, s2, p;
        bool interchange = false;

        dx = abs(x2 - x1);
        dy = abs(y2 - y1);
        if((x2 - x1) < 0) {
            s1 = -1;
        } else if((x2 - x1) > 0) {
            s1 = 1;
        } else {
            s1 = 0;
        }
        if((y2 - y1) < 0) {
            s2 = -1;
        } else if((y2 - y1) > 0) {
            s2 = 1;
        } else {
        s2 = 0;
        }

        if(dy > dx) {
            int temp = dy;
            dy = dx;
            dx = temp;
            interchange = true;
        }

        p = 2 * dy - dx;
        x = x1;
        y = y1;

        for(int i = 1; i < dx; i++) {
            if(p < 0) {
                if(interchange) {
                    y += s2;
                } else {
                    x += s1;
                }
                p += 2 * dy;
            }
            else {
                y += s2;
                x += s1;
                p += 2 * dy - 2 * dx;
            }
            drawPoint(x, HEIGHT - y);
        }
    }
    glFlush();
}

static void keyboardHandler(unsigned char key, int xx, int yy)
{
    switch(key) {
    case '1':
        cout<<"Bresenham Line Algorithm\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glFlush();
        glutMouseFunc(bresenham);
        break;
    }
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    cout<<"1. Bresenham Line\n\n";
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
