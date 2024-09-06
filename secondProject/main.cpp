/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

float a, b, c, d, e, f, x, y, x0, y_init; //changed y0 to y_init since it caused conflict with standard namespace libraries
int i = 0;
using namespace std;

void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport
    x0 = 0.5;
    y_init = 0.5;
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
    // your code here
    // 0 --- 7 --- 15---------99
    // |  \/    \/       \/
    //f1  f3    f4       f2
    //
    // 0 < 85 < 92 < 99
    //   |    |    |    |
    //   f2   f3   f4   f1
    for(int j=0; j < 200000; j++){
        i = rand()%100;
        if(i < 85){ // Transform 2 is selected for numbers between 0 and 85.
            a= 0.85;
            b = -0.04;
            c =0.04;
            d =0.85;
            e =0;
            f=1.6;
        }
        else if(i < 90){ // Transform 3 is for numbers between 85 and 90
            a= 0.2;
            b =0.23;
            c =-0.26;
            d =0.22;
            e =0;
            f=1.6;
        } 
        else if (i < 99){ // Transform 4 is for numbers between 90 and 99
            a= -0.15;
            b =0.26;
            c =0.28;
            d =0.24;
            e =0;
            f=0.44;

        } else { // Transform 1 is selected for numbers between 99 and 100
            a= 0;
            b =0;
            c =0;
            d =0.16;
            e =0;
            f=0;
        }

        x = a*x0 + c*y_init + e;
        y = b*x0 + d*y_init + f;

        glColor3f(0.0f,1.0f,0.0f);
        glBegin(GL_POINTS);
        glVertex3f(x,y,0);
        glEnd();

        x0 = x;
        y_init = y;
    }
    glFlush();                           // clear buffer
    
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600, 800);                //window screen
   glutInitWindowPosition (0, 0);            //window position
   glutCreateWindow ("second Project");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}