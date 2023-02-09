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

int xwmin = WIDTH / 2 - 200, xwmax = WIDTH / 2 + 200, ywmin = HEIGHT / 2 - 100, ywmax = HEIGHT / 2 + 100;

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


static bool pointClipping(int x, int y) {
    if(x >= xwmin && x <= xwmax and y >= ywmin && y <= ywmax) {
        return true;
    } else {
        return false;
    }
}

static void getRegionCode(int x, int y, vector<int>& r) {
    if(x - xwmin < 0) {
        r[3] = 1;
    }
    if(xwmax - x < 0) {
        r[2] = 1;
    }
    if(y - ywmin < 0) {
        r[1] = 1;
    }
    if(ywmax - y < 0) {
        r[0] = 1;
    }
}

static bool completelyInside(vector<int> r1, vector<int> r2) {
    if(r1[0] == 0 && r1[1] == 0 && r1[2] == 0 && r1[3] == 0 && r2[0] == 0 && r2[1] == 0 && r2[2] == 0 && r2[3] == 0) {
        return true;
    } else {
        return false;
    }
}

static bool completelyOutside(vector<int> r1, vector<int> r2) {
    if((r1[0] == 1 && r2[0] == 1) || (r1[1] == 1 && r2[1] == 1) || (r1[2] == 1 && r2[2] == 1) || (r1[3] == 1 && r2[3] == 1)) {
        return true;
    } else {
        return false;
    }
}

static void cohenSutherland(int button, int state, int xx, int yy)
{
    static vector<vector<int> > points;

    if(points.size() < 2) {
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            drawPoint(xx, HEIGHT - yy);
            // cout<<"("<<xx<<", "<<yy<<")\n\n";

            vector<int> point;
            point.push_back(xx);
            point.push_back(yy);
            points.push_back(point);

            glFlush();
        }
    } else {
        int x1 = points[0][0], y1 = points[0][1], x2 = points[1][0], y2 = points[1][1];
        points.clear();

        // Setting region codes for the end points
        vector<int> r1(4, 0);
        getRegionCode(x1, y1 ,r1);
        vector<int> r2(4, 0);
        getRegionCode(x2, y2, r2);

        // Check if the line is completely inside
        if(r1[0] == 0 && r1[1] == 0 && r1[2] == 0 && r1[3] == 0 && r2[0] == 0 && r2[1] == 0 && r2[2] == 0 && r2[3] == 0) {
            glColor3d(0, 1, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
            return;
        }

        // Check if line is completely outside
        if((r1[0] == 1 && r2[0] == 1) || (r1[1] == 1 && r2[1] == 1) || (r1[2] == 1 && r2[2] == 1) || (r1[3] == 1 && r2[3] == 1)) {
            glColor3d(1, 0, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
            return;
        }

        float m = (y2 - y1) / (float)(x2 - x1);
        float c = y1 - m * x1;
        // Line might pass through the clip window
        while(!completelyInside(r1, r2) && !completelyOutside(r1, r2)) {
           
            // If P1 is outside the clip window
            if(!pointClipping(x1, y1)) {
                // Outside left boundary
                int xOld = x1, yOld = y1;
                if(r1[3] == 1) {
                    x1 = xwmin;
                    y1 = m * x1 + c;
                    r1[3] = 0;
                } else if(r1[2] == 1) {
                    x1 = xwmax;
                    y1 = m * x1 + c;
                    r1[2] = 0;
                } else if(r1[1] == 1) {
                    y1 = ywmin;
                    x1 = (y1 - c) / m;
                    r1[1] = 0;
                } else if(r1[0] == 1) {
                    y1 = ywmax;
                    x1 = (y1 - c) / m;
                    r1[0] = 0;
                }

                glColor3d(1, 0, 0);
                glBegin(GL_LINES);
                    glVertex2f(x1, HEIGHT - y1);
                    glVertex2f(xOld, HEIGHT - yOld);
                glEnd();
                glFlush();
            }
            // If P2 is outside the clip window
            else if(!pointClipping(x2, y2)) {
                int xOld = x2, yOld = y2;
                // Outside left boundary
                if(r2[3] == 1) {
                    x2 = xwmin;
                    y2 = m * x2 + c;
                    r2[3] = 0;
                } else if(r2[2] == 1) {
                    x2 = xwmax;
                    y2 = m * x2 + c;
                    r2[2] = 0;
                } else if(r2[1] == 1) {
                    y2 = ywmin;
                    x2 = (y2 - c) / m;
                    r2[1] = 0;
                } else if(r2[0] == 1) {
                    y2 = ywmax;
                    x2 = (y2 - c) / m;
                    r2[0] = 0;
                }

                glColor3d(1, 0, 0);
                glBegin(GL_LINES);
                    glVertex2f(xOld, HEIGHT - yOld);
                    glVertex2f(x2, HEIGHT - y2);
                glEnd();
                glFlush();
            }
        }
        if(completelyInside(r1, r2)) {
            glColor3d(0, 1, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
        } else {
            glColor3d(1, 0, 0);
            glBegin(GL_LINES);
                glVertex2f(x1, HEIGHT - y1);
                glVertex2f(x2, HEIGHT - y2);
            glEnd();
            glFlush();
        }
    }

    return;
}

static void keyboardHandler(unsigned char key, int xx, int yy)
{
    switch(key) {
    case '1':
        cout<<"Cohen - Sutherland Line Clipping\n";

        // Draw a fresh clip window
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0, 1.0, 1.0);
        glLineWidth(3);

        glBegin(GL_LINE_LOOP);
            glVertex2i(xwmin, ywmin);
            glVertex2i(xwmax, ywmin);
            glVertex2i(xwmax, ywmax);
            glVertex2i(xwmin, ywmax);
        glEnd();

        glFlush();

        glutMouseFunc(cohenSutherland);
        break;
    }
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    cout<<"1. Cohen-Sutherland Clipping\n\n";
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
