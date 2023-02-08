#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <bits/stdc++.h>
#include <stdlib.h>
using namespace std;

class Point{
public:
    int x;
    int y;

    Point(int a=0,int b=0){
        x=a;
        y=b;
    }
};

vector <Point> points;
vector <Point> window;
vector <vector<Point>> lines;
vector <vector<Point>> clipped_lines;

//top bottom right left
const int TOP = 8;
const int BOTTOM = 4;
const int RIGHT = 2;
const int LEFT = 1;

int x_min,x_max,y_min,y_max;

void plotPoint(Point pt){
    glPointSize(2);
    glColor3d(0,1,0);
    glBegin(GL_POINTS);
    glVertex2f(pt.x,pt.y);
    glEnd();
    glFlush();
}

bool flag = false;

void drawWindowOrLine(){
    if(flag == false){
        Point p1 = points[0];
        Point p2 = points[1];
        glBegin(GL_LINES);
        glVertex2d(p1.x,p1.y);
        glVertex2d(p1.x,p2.y);
        glVertex2d(p2.x,p2.y);
        glVertex2d(p2.x,p1.y);
        glVertex2d(p1.x,p2.y);
        glVertex2d(p2.x,p2.y);
        glVertex2d(p2.x,p1.y);
        glVertex2d(p1.x,p1.y);
        glEnd();
        glFlush();
        window.push_back(p1);
        window.push_back(p2);
        x_min = window[0].x;
        y_min = window[0].y;
        x_max = window[1].x;
        y_max = window[1].y;
    }
    else{
        Point p1 = points[points.size()-2];
        Point p2 = points[points.size()-1];
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2d(p1.x,p1.y);
        glVertex2d(p2.x,p2.y);
        glEnd();
        glFlush();
        vector <Point> line;
        line.push_back(p1);
        line.push_back(p2);
        lines.push_back(line);
    }
}



int findCode(Point p){

    int code=0;

    if(p.x<x_min)
        code+=LEFT;

    if(p.y<y_min)
        code+=BOTTOM;

    if(p.x>x_max)
        code+=RIGHT;

    if(p.y>y_max)
        code+=TOP;

    return code;
}

void changeAcceptColour(Point p1,Point p2){
    glColor3d(1,1,0);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2d(p1.x,p1.y);
    glVertex2d(p2.x,p2.y);
    glEnd();
    glFlush();

}


void cohenSutherland(){
    for(int i=0;i<lines.size();i++){
        Point pnt1 = lines[i][0];
        Point pnt2 = lines[i][1];
        int code1 = findCode(pnt1);
        int code2 = findCode(pnt2);

        while(true){
            if(!(code1|code2)){
                cout<<"Trivial accept."<<endl;
                changeAcceptColour(pnt1,pnt2);
                break;
            }
            else if(code1&code2){
                cout<<"Trivial reject."<<endl;
                break;
            }
            else{
                int code_out;
                double x, y;

                if (code1 != 0)
                    code_out = code1;
                else
                    code_out = code2;

                if(code_out & TOP){
                    //x = x1+(x2-x1)*y_max-y1/y2-y1
                    x = pnt1.x + (pnt2.x-pnt1.x)*(y_max-pnt1.y)/(pnt2.y-pnt1.y);
                    y = y_max;
                }
                if(code_out & BOTTOM){
                    x = pnt1.x + (pnt2.x-pnt1.x)*(y_min-pnt1.y)/(pnt2.y-pnt1.y);
                    y = y_min;
                }
                if(code_out & LEFT) {
                    y = pnt1.y + (pnt2.y-pnt1.y) * (x_min - pnt1.x)/(pnt2.x-pnt1.x);
                    x = x_min;
                }
                if(code_out & RIGHT) {
                    y = pnt1.y + (pnt2.y-pnt1.y) * (x_max - pnt1.x)/(pnt2.x-pnt1.x);
                    x = x_max;
                }

                if (code_out == code1)
                {
                    pnt1.x = x;
                    pnt1.y = y;
                    code1 = findCode(pnt1);
                }
                else
                {
                    pnt2.x = x;
                    pnt2.y = y;
                    code2 = findCode(pnt2);
                }
            }
        }


    }
}

void liangBarsky(){
   
}


static void mouse(int button,int state,int x,int y){
    if(button == GLUT_LEFT_BUTTON and state == GLUT_DOWN){
        Point p(x,480-y);
        plotPoint(p);
        points.push_back(p);
        if(points.size() == 2){
            drawWindowOrLine();
            flag = true;
        }
        if(points.size()>2 && points.size()%2 == 0){
            drawWindowOrLine();
        }
    }

}

static void keyboard(unsigned char key,int x,int y){
    if(key == 'c'){
        cohenSutherland();
    }
    if(key == 'l'){
        liangBarsky();
    }
}

static void display(void)
{
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}



int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    glutCreateWindow("GLUT Shapes");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}
