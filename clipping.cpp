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

bool inside(Point p, Edge e) {
    switch(e) {
        case Left:
            if(p.x < wMin.x) {
                return false;
            }
            break;
        case Right:
            if(p.x > wMax.x) {
                return false;
            }
            break;
        case Bottom:
            if(p.y < wMin.y) {
                return false;
            }
            break;
        case Top:
            if(p.y > wMax.y) {
                return false;
            }
            break;
    }

    return true;
}

bool cross(Point p1, Point p2, Edge e) {
    if(inside(p1, e) == inside(p2, e)) {
        return false;
    } else {
        return true;
    }
}

Point intersect(Point p1, Point p2, Edge e) {
    Point intersection;
    float m;

    if(p1.x != p2.x) {
        m = (p2.y - p1.y) / (p2.x - p1.x);
    }

    switch(e) {
        case Left:
            intersection.x = wMin.x;
            intersection.y = p2.y + (wMin.x - p2.x) * m;
            break;
        case Right:
            intersection.x = wMax.x;
            intersection.y = p2.y + (wMax.x - p2.x) * m;
            break;
        case Bottom:
            intersection.y = wMin.y;
            if(p1.x != p2.x) {
                intersection.x = p2.x + (wMin.y - p2.y) / m;
            } else {
                intersection.x = p2.x;
            }
            break;
        case Top:
            intersection.y = wMax.y;
            if(p1.x != p2.x) {
                intersection.x = p2.x + (wMax.y - p2.y) / m;
            } else {
                intersection.x = p2.x;
            }
            break;
    }

    return intersection;
}

vector<Point> clip(vector<Point> polygon, Edge e, vector<Point> &out) {
    vector<Point> clipped;
    int n = polygon.size();

    for(int i = 0, j = 1; i < n; i++, j = (++j) % n) {
        if(cross(polygon[i], polygon[j], e)) {
            Point intersection = intersect(polygon[i], polygon[j], e);
            if(inside(polygon[i], e)) {      // Inside -> Outside
                clipped.push_back(intersection);
                out.push_back(intersection);
                out.push_back(polygon[j]);
            } else {                         // Outside -> Inside
                clipped.push_back(intersection);
                clipped.push_back(polygon[j]);
                out.push_back(intersection);
            }
        } else {
            if(inside(polygon[i], e)) {     // Inside -> Inside
                clipped.push_back(polygon[j]);
            } else {                        // Outside -> Outside
                out.push_back(polygon[j]);
            }
        }
    }

    return clipped;
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

void drawPolygon(vector<Point> polygon, FColour c) {
    glColor3f(c.r, c.g, c.b);

    int n = polygon.size();

    glBegin(GL_LINE_LOOP);
        for(int i = 0; i < n; i++) {
            glVertex2f(polygon[i].x, HEIGHT - polygon[i].y);
        }
    glEnd();
}

static void sutherlandHodgeman(int button, int state, int xx, int yy) {
    static vector<Point> polygon;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        drawPoint(xx, HEIGHT - yy);

        Point point(xx, yy);
        polygon.push_back(point);

        glFlush();
    } else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        vector<Point> clipped;
        vector<Point> out;

        clipped = clip(polygon, Left, out);
        clipped = clip(clipped, Right, out);
        clipped = clip(clipped, Bottom, out);
        clipped = clip(clipped, Top, out);

        // drawPolygon(out, 1, 0, 0);
        drawPolygon(clipped, 0, 1, 0);

        glFlush();
        polygon.clear();
    }
}


static void keyboardHandler(unsigned char key, int xx, int yy) {
    switch(key) {
        case '1':
            cout<<"Sutherland - Hodgeman Algorithm\n";

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

            glutMouseFunc(sutherlandHodgeman);
            break;
    }
}

static void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    cout << "1. Press 1 to continue\n";
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
