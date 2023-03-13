#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <bits/stdc++.h>
#include <vector>
#include <windows.h>

#ifdef __APPLE__
#else
#include <GL/glut.h>
#endif

#define WIDTH 640
#define HEIGHT 480

using namespace std;

float xwmin = WIDTH / 2 - 200, xwmax = WIDTH / 2 + 200, ywmin = HEIGHT / 2 - 100, ywmax = HEIGHT / 2 + 100;
enum Edge {Left, Right, Bottom, Top};

class Point {
    public:
        float x;
        float y;

        Point() {
            x = 0;
            y = 0;
        }

        Point(float xx, float yy) {
            x = xx;
            y = yy;
        }
};

class Colour {
    public:
        int r;
        int g;
        int b;

        Colour() {
            r = 0;
            g = 0;
            b = 0;
        }

        Colour(int rr, int gg, int bb) {
            r = rr;
            g = gg;
            b = bb;
        }

        void display() {
            cout<<"Red:"<<r<<"  Green:"<<g<<"  Blue:"<<b<<"\n";
        }
};

class FColour {
    public:
    float r;
    float g;
    float b;

    FColour() {
        r = 0;
        g = 0;
        b = 0;
    }

    FColour(float rr, float gg, float bb) {
        r = rr;
        g = gg;
        b = bb;
    }

    FColour(float pixel[3]) {
        r = pixel[0];
        g = pixel[1];
        b = pixel[2];
    }

    void display() {
        cout<<"Red:"<<r<<"  Green:"<<g<<"  Blue:"<<b<<"\n";
    }
};

Point wMin(xwmin, ywmin);
Point wMax(xwmax, ywmax);

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glClearColor(0, 0, 0, 1);
}

static void drawPoint(float x, float y)
{
    glPointSize(2.0f);
    glColor3d(1, 1, 1);

    glBegin(GL_POINTS);
    glVertex2f(x, y);
    glEnd();
}

void drawPolygon(vector<Point> polygon, int r, int g, int b) {
    glColor3d(r, g, b);
    glLineWidth(2);

    int n = polygon.size();

    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < n; i++) {
            glVertex2f(polygon[i].x, HEIGHT - polygon[i].y);
        }
    glEnd();
}


static void yx(int button, int state, int xx, int yy) {
    static vector<Point> polygon;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        Point point(xx, yy);
        polygon.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        drawPolygon(polygon, 1, 1, 1);
        glFlush();

        FColour background(0.0f, 0.0f, 0.0f);

        for(int y = 0; y <= HEIGHT; y++) {      // For each scan line
            vector<int> intersections;
            int n = polygon.size();

            for(int i = 0, j = 1; i < n; i++, j = (++j) % n) {      // For each edge
                /*
                if(polygon[i].x != polygon[j].x) {
                    int x = scanLineIntersect(y, polygon[i], polygon[j]);
                    intersections.push_back(x);
                }
                */

                int y1 = polygon[i].y;
                int y2 = polygon[j].y;

                if(y1 < y2) {
                    if(y >= y1 && y < y2) {
                        int x = (y - y1) * (polygon[j].x - polygon[i].x) / (y2 - y1) + polygon[i].x;
                        intersections.push_back(x);
                    }
                } else {
                    if (y >= y2 && y < y1) {
                        int x = (y - y1) * (polygon[j].x - polygon[i].x) / (y2 - y1) + polygon[i].x;
                        intersections.push_back(x);
                    }
                }
            }

            sort(intersections.begin(), intersections.end());
            int m = intersections.size();

            glColor3f(0, 1, 0);
            for(int i = 0; i < m; i += 2) {
                int x1 = intersections[i];
                int x2 = intersections[i + 1];
                glBegin(GL_LINES);
                    glVertex2i(x1, HEIGHT - y);
                    glVertex2i(x2, HEIGHT - y);
                glEnd();

                glFlush();
            }
        }

        cout<<"Finished filling!\n";

        glFlush();
        polygon.clear();
    }
}

static void keyboardHandler(unsigned char key, int xx, int yy) {
    switch(key) {

        case '1':
            cout<<"YX Algorithm\n";

            glLineWidth(1);
            glPointSize(1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            glFlush();

            glutMouseFunc(yx);
            break;
    }
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    cout << "Press 1 to continue\n";
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(WIDTH / 4, HEIGHT / 4);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("CA 2");
    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardHandler);

    glutMainLoop();

    return EXIT_SUCCESS;
}
