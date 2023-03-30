//  Bezier Curve



#include<iostream>
#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<math.h>
#include <stdlib.h>
#include<vector>
using namespace std;

static int slices = 16;
static int stacks = 16;

struct pnt{
    float x, y;
};

vector<pnt> c_pnts;

void init()
{
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,640,0,480);
}

static void display(void)
{
    //glClear(GL_COLOR_BUFFER_BIT );
    glColor3d(1,0,0);
    glFlush();
}

float fact(float n){
    if(n <= 1){
        return 1;
    }
    return fact(n-1);
}

vector<pnt> b_curve(){
    int n = c_pnts.size() - 1;
    float x, y;
    vector<pnt> fin;
    for(float t =0 ; t<=1 ; t+=0.001){
        x =0;
        y =0;

        for(int i=0;i< c_pnts.size();i++){
            x += c_pnts[i].x*pow(t,i)*pow((1-t),(n-i))*(fact(n)/fact(i)*fact(n-i));
            y += c_pnts[i].y*pow(t,i)*pow((1-t),(n-i))*(fact(n)/fact(i)*fact(n-i));
        }
        //fin.push_back((pnt){x,y});

        x = pow(t,3)*c_pnts[0].x + 3*pow(t,2)*pow(1-t,1)*c_pnts[1].x + 3*pow(1-t,2)*pow(t,1)*c_pnts[2].x + pow(1-t,3)*c_pnts[3].x;
        y = pow(t,3)*c_pnts[0].y + 3*pow(t,2)*pow(1-t,1)*c_pnts[1].y + 3*pow(1-t,2)*pow(t,1)*c_pnts[2].y + pow(1-t,3)*c_pnts[3].y;

        fin.push_back((pnt){x,y});
    }
    return fin;
}

void drw_pnt(pnt p){
    glColor3d(1,0,0);
    glBegin(GL_POINTS);
        glVertex2f(p.x,p.y);
    glEnd();
}

void mou(int btn, int st, int x, int y){
    y = 480 - y;
    if(btn == GLUT_LEFT_BUTTON && st == GLUT_DOWN){
        c_pnts.push_back((pnt){x,y});
        cout<<x<<"   "<<y<<"\n";
        glColor3d(1,0,1);
        drw_pnt((pnt){x,y});
    }
    else if(btn == GLUT_RIGHT_BUTTON && st == GLUT_DOWN){
        glColor3d(0.3,0.3,0.3);
        glBegin(GL_LINES);
            for(int j=1; j<c_pnts.size();j++){
                glVertex2f(c_pnts[j-1].x,c_pnts[j-1].y);
                glVertex2f(c_pnts[j].x,c_pnts[j].y);
            }
        glEnd();

        glColor3d(1,1,0);
        vector<pnt> tmp = b_curve();
        cout<<"points :\n";
        for(int i = 0 ; i < tmp.size();i++){
            drw_pnt(tmp[i]);
            cout<<tmp[i].x<<"    "<<tmp[i].y<<"\n";
        }
        tmp.clear();
        c_pnts.clear();
    }
    glutPostRedisplay();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);


    glutCreateWindow("GLUT Shapes");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mou);
    //glutKeyboardFunc(key);
    glutMainLoop();

    return EXIT_SUCCESS;
}
