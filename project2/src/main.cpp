#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <SOIL.h>
#include <math.h>

using namespace std;

bool WireFrame= false;

GLuint tex; // Texture pointer

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Variables Handlers */

// Object selection variables
bool c_flag, t_flag, s_flag, r_flag, scale_flag;

// Object position
GLfloat cubeX, cubeY, cubeZ, teaPotX, teaPotY, teaPotZ, sphereX, sphereY, sphereZ;

// Scaling
GLfloat scaleFactorX, scaleFactorY, scaleFactorZ;

// Position offsets
GLfloat cubeOffset, teaPotOffset, sphereOffset;

// Rotation offsets
GLint cubeAngle, teaPotAngle, sphereAngle;

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,15,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    // Two other operations are also useful for manipulating the matrix stack:
    // glPushMatrix(), glPopMatrix() makes it so that what is made inside does not affect other objects
    // if not used the first obj will be in relation to origin/translatef, and next will be in relation to previous trasnlatef and so on
    // CUBE
    glPushMatrix();
    glColor3f(0.0f,1.0f,0.0f);  // Sets Color
    glTranslatef(cubeX, cubeY, cubeZ+cubeOffset); // Sets new position
    glRotatef(cubeAngle, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX,scaleFactorY,scaleFactorZ); // Scale factors along the (x axis, y axis, z axis)
    glutSolidCube(2); // Draws Cube
    glPopMatrix();

    // TEAPOT
    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);  // Sets Color
    glTranslatef(teaPotX, teaPotY, teaPotZ+teaPotOffset); // Sets new position
    glRotatef(teaPotAngle, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX,scaleFactorY,scaleFactorZ);
    glutSolidTeapot(1.5); // Draws Teacup
    glPopMatrix();

    // SPHERE
    glPushMatrix();
    glColor3f(0.0f,0.0f,1.0f);  // Sets Color
    glTranslatef(sphereX, sphereY, sphereZ+sphereOffset); // Sets new position
    glRotatef(sphereAngle, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX,scaleFactorY,scaleFactorZ);
    glutSolidSphere (1.3, 20, 20); // Draws Sphere (radius, slices, stack)
    glPopMatrix();

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'c': // Cube
            c_flag = true;
            t_flag = false;
            s_flag = false;
            scale_flag = false;
            break;
        case 's': // Sphere
            c_flag = false;
            t_flag = false;
            s_flag = true;
            scale_flag = false;
            break;
        case 't': // Teapot
            c_flag = false;
            t_flag = true;
            s_flag = false;
            scale_flag = false;
            break;
        case 'd': // Unselect all
            s_flag = false;
            c_flag = false;
            t_flag = false;
            scale_flag = false;
            break;
        case '+':
            scale_flag = true;
            break;
        case 'r': // Reset
            r_flag = true;
            break;
        default:
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {   
        // Zoom in
        case GLUT_KEY_DOWN:
            if(c_flag){
                cubeOffset += 0.5;
            }
            if(t_flag){
                teaPotOffset += 0.5;
            }
            if(s_flag){
                sphereOffset += 0.5;
            }
            if(r_flag){
                cubeOffset = teaPotOffset = sphereOffset = 0;
                r_flag = false;
            }
        break;
        // Zoom out
        case GLUT_KEY_UP:
            if(c_flag){
                cubeOffset -= 0.5;
            }
            if(t_flag){
                teaPotOffset -= 0.5;
            }
            if(s_flag){
                sphereOffset -= 0.5;
            }
            if(r_flag){
                cubeOffset = teaPotOffset = sphereOffset = 0;
                r_flag = false;
            }
            break;
        // Rotate left around y-axis
        case GLUT_KEY_LEFT:
            if(c_flag){
                cubeAngle += 5;
            }
            if(t_flag){
                teaPotAngle += 5;
            }
            if(s_flag){
                sphereAngle += 5;
            }
            if(r_flag){
                cubeAngle = teaPotAngle = sphereAngle = 0;
                r_flag = false;
            }
            break;
        // Rotate right around y-axis
        case GLUT_KEY_RIGHT:
            if(c_flag){
                cubeAngle -= 5;
            }
            if(t_flag){
                teaPotAngle -= 5;
            }
            if(s_flag){
                sphereAngle -= 5;
            }
            if(r_flag){
                cubeAngle = teaPotAngle = sphereAngle = 0;
                r_flag = false;
            }
            break;
        case GLUT_KEY_F1: // Scale x-axis
            if(scale_flag){
                scaleFactorX += 0.5;
            }
            if(r_flag){
                scaleFactorX = scaleFactorY = scaleFactorZ = 1;
                r_flag = false;
            }
            break;
        case GLUT_KEY_F2:
            if(scale_flag){
                scaleFactorX -= 0.5;
            }
            if(r_flag){
                scaleFactorX = scaleFactorY = scaleFactorZ = 1;
                r_flag = false;
            }
            break;
        case GLUT_KEY_F3: // Scale y-axis
            if(scale_flag){
                scaleFactorY += 0.5;
            }
            if(r_flag){
                scaleFactorX = scaleFactorY = scaleFactorZ = 1;
                r_flag = false;
            }
            break;
        case GLUT_KEY_F4:
            if(scale_flag){
                scaleFactorY -= 0.5;
            }
            if(r_flag){
                scaleFactorX = scaleFactorY = scaleFactorZ = 1;
                r_flag = false;
            }
            break;
        case GLUT_KEY_F5: // Scale z-axis
            if(scale_flag){
                scaleFactorZ += 0.5;
            }
            if(r_flag){
                scaleFactorX = scaleFactorY = scaleFactorZ = 1;
                r_flag = false;
            }
            break;
        case GLUT_KEY_F6:
            if(scale_flag){
                scaleFactorZ -= 0.5;
            }
            if(r_flag){
                scaleFactorX = scaleFactorY = scaleFactorZ = 1;
                r_flag = false;
            }
            break;
        default:
            break;
   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    // Objects initial position
    cubeX = 4.0;
    teaPotX = 0.0;
    sphereX = -3.5;

    cubeY = 0.0;
    teaPotY = 0.0;
    sphereY = 0.0;

    cubeZ = -2.0;
    teaPotZ = -3.0;
    sphereZ = -1.0;

    // Flags
    c_flag = t_flag = s_flag = r_flag = scale_flag = false;

    // Scaling
    scaleFactorX = scaleFactorY = scaleFactorZ = 1;

    // Position offset
    cubeOffset = teaPotOffset = sphereOffset = 0;

    // Rotation offset
    cubeAngle = teaPotAngle = sphereAngle = 0 % 360;
    
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}