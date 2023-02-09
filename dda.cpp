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

static void DDA(int button, int state, int xx, int yy) {
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

        int dx = x2 - x1, dy = y2 - y1;
        int steps, k;
        float xInc, yInc;
        float x = x1, y = y1;

        if(abs(dx) > abs(dy)) {
            steps = abs(x);
        } else {
            steps = abs(y);
        }

        xInc = dx / (float)steps;
        yInc = dy / (float)steps;

        drawPoint(x, HEIGHT - y);

        for(k = 0; k < steps; k++) {
            x += xInc;
            y += yInc;
            drawPoint(x, HEIGHT - y);
        }
    }
    glFlush();
}

static void keyboardHandler(unsigned char key, int xx, int yy)
{
    switch(key) {
    case '1':
        cout<<"DDA Line Algorithm\n";
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0, 1.0, 1.0);
        glFlush();
        glutMouseFunc(DDA);
        break;
    }
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    cout<<"1. DDA Line\n\n";
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
