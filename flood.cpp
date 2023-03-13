#include<windows.h>
#include <GL/glut.h>
#include<iostream>
#include<vector>
#include<cmath>
#include <stdlib.h>

using namespace std;

static int slices = 16;
static int stacks = 16;

static int x_length=720;
static int y_length=640;

class Point
{
    public:
        double X;
        double Y;

        Point()
        {
            X=0;
            Y=0;
        }
        Point(double x,double y)
        {
            this->X=x;
            this->Y=y;
        }

        void xy(double x,double y)
        {
            this->X=x;
            this->Y=y;
        }
        double x()
        {
            return X;
        }
        double y()
        {
            return Y;
        }
};

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,x_length,0,y_length);
    glClearColor(0,0,0,0);
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void boundaryFill(int x, int y, float* fillColor, float* borderColor) {
    cout << "i boun\n";
    float interiorColor[3];
    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, interiorColor);
    if (interiorColor[0] != borderColor[0] || interiorColor[1] != borderColor[1] || interiorColor[2] != borderColor[2]) {
        if (interiorColor[0] != fillColor[0] || interiorColor[1] != fillColor[1] || interiorColor[2] != fillColor[2]) {
            glColor3fv(fillColor);
            setPixel(x, y);
            boundaryFill(x+1, y, fillColor, borderColor);
            boundaryFill(x-1, y, fillColor, borderColor);
            boundaryFill(x, y+1, fillColor, borderColor);
            boundaryFill(x, y-1, fillColor, borderColor);
        }
    }
}

void floodFill(int x, int y, float* fillColor, float* interiorColor) {
    float currentColor[3];
    glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, currentColor);
    if (currentColor[0] == interiorColor[0] && currentColor[1] == interiorColor[1] && currentColor[2] == interiorColor[2]) {
        setPixel(x, y);
        glColor3fv(fillColor);
        floodFill(x+1, y, fillColor, interiorColor);
        floodFill(x-1, y, fillColor, interiorColor);
        floodFill(x, y+1, fillColor, interiorColor);
        floodFill(x, y-1, fillColor, interiorColor);
    }
}


static void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(100, 100);
    glVertex2i(200, 100);
    glVertex2i(200, 200);
    glVertex2i(100, 200);
    glEnd();
    float fillColor[] = {0.0, 0.0, 0.0};
    float boundaryColor[] = {1.0, 0.0, 0.0};
    float interiorColor[] = {1.0, 1.0, 1.0};
    floodFill(150, 150, fillColor, interiorColor);
//    boundaryFill(150, 150, fillColor1, boundaryColor);
    glFlush();
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(x_length,y_length);
    glutInitWindowPosition(250,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Fill color");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return EXIT_SUCCESS;
}
