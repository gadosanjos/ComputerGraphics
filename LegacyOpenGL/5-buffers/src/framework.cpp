#include <string.h>
#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#define PI 3.14159

using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

// VBO: Stores raw vertex data (positions, normals, texture coordinates).
// EBO: Stores indices for vertices only, defining the geometry (e.g., triangles).
GLuint vbo, ibo;

float rotateX = 0;
float rotateY = 0;
float rotateZ = 0;
float Wwidth,Wheight;

GLfloat vertices[] = {
    0, 0, 0, // Vertex 0
    0, 0, 1, // Vertex 1
    0, 1, 0, // Vertex 2
    0, 1, 1, // Vertex 3
    1, 0, 0, // Vertex 4
    1, 0, 1, // Vertex 5
    1, 1, 0, // Vertex 6
    1, 1, 1  // Vertex 7
    };
// face normals
GLfloat normals[] = {
    0, 0, 1,  // Z+ face
    0, 0, -1, // Z- face
    0, 1, 0,  // Y+ face
    0, -1, 0, // Y- face
    1, 0, 0,  // X+ face
    -1, 0, 0  // X- face
    };

int indices[] = {
    0,6,4, // First triangle
    0,2,6, // Second triangle
    0,3,2,
    0,1,3,
    2,7,6,
    2,3,7,
    4,6,7,
    4,7,5,
    0,4,5,
    0,5,1,
    1,5,7,
    1,7,3
    };

int indices1[36];

GLfloat vertices2[] = {
    0, 0, 0,
    5, 0, 0,
    5, 5, 0,
    0, 5, 0
    };

GLfloat normals2[] = {0, 0, -1};

int ind2[] = {
    0,0,
    1,0,
    2,0,
    3,0
    };


// Variables for mouse interaction
int numbOfClicks = 0; // Track the number of clicks
const int MAX_CLICKS = 100; // Maximum allowed clicks
float mouseX = 0.0f; // Mouse X coordinate
float mouseY = 0.0f; // Mouse Y coordinate

void createVBO(){
    /* 
    This function creates and initializes Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs), storing vertex data, normal data, and indices in GPU memory. 
    */

    //Calculate the size of the buffer:
    unsigned int Size = (sizeof(vertices)) +(sizeof(normals)); //This calculates the total memory needed to store the vertex and normal data.
    // Generate buffer objects:
        // glGenBuffers creates a unique identifier for the Element Buffer Object (ibo) and the Vertex Buffer Object (vbo).
        // The ibo will store indices, and the vbo will store vertex and normal data.
    glGenBuffers(1, &ibo); // convetional name is EBO
    glGenBuffersARB(1, &vbo);

    // Bind the VBO and EBO:
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);  // Target for storing vertex data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); // Target for storing index data.

    // Allocate and transfer vertex and normal data to the VBO:
    glBufferDataARB(GL_ARRAY_BUFFER_ARB, Size, 0, GL_DYNAMIC_DRAW); // Allocates GPU memory for the VBO.
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(vertices), vertices); // Copies vertices starting at offset 0 and normals starting at sizeof(vertices)
    glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), sizeof(normals), normals);

    // Transfer index data to the EBO:
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW); // Allocates and uploads index data to the EBO.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0); // Unbinds the buffers to avoid accidental modification.
    glBindBuffer(GL_ARRAY_BUFFER_ARB,0); // Unbinds the buffers to avoid accidental modification.
}

void drawVBO(){
    /* 
    This function renders the object using the buffers created in createVBO. 
    */
    //Bind the VBO and EBO:
        //Rebinds the buffers so OpenGL knows where to find the data.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Enable vertex and normal arrays:
        // Tells OpenGL to expect vertex and normal data in the active VBO.
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // Specify vertex and normal data layout:
    glVertexPointer(3,GL_FLOAT,0,(void*)0); // Specifies that vertex data consists of groups of 3 floats starting from offset 0 in the VBO.
    glNormalPointer(GL_FLOAT,0,(void*)sizeof(vertices)); // Specifies that normal data starts from offset sizeof(vertices) in the VBO.
        //Render the object:
        glDrawElements(GL_TRIANGLES,    // primite type, Render the geometry as triangles.
                        36,             // Number of indices to use for rendering.
                        GL_UNSIGNED_INT,// Specifies the data type of the indices in the EBO.
                        (void*)0);      // Starting offset in the EBO.
    //Disable vertex and normal arrays:
    glDisableClientState(GL_VERTEX_ARRAY); // Resets OpenGL state, preventing unintended effects on subsequent draw calls.
    glDisableClientState(GL_NORMAL_ARRAY); // Resets OpenGL state, preventing unintended effects on subsequent draw calls.
}

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

     Wwidth = (float)width;
     Wheight = (float)height;

     Ratio= (double)width /(double)height;

    glViewport(0,0,(GLsizei) width,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	gluPerspective (45.0f,Ratio,0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10.0,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

 // your code here
    
    glRotatef(rotateY, 1, 0, 0 );
    glRotatef(rotateX, 0, 1, 0 );
    glRotatef(rotateZ, 0, 0, 1 );
    glPushMatrix();
    glTranslatef(-0.5,-0.5,-0.5);
    drawVBO();
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

	  case 'w':
		WireFrame =!WireFrame;
	       break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:
        rotateY += 5;
        break;
    case GLUT_KEY_UP:
        rotateY -= 5;
        break;
    case GLUT_KEY_RIGHT:
        rotateX += 5;
        break;
    case GLUT_KEY_LEFT:
        rotateX -= 5;
        break;
   }
  glutPostRedisplay();
}


static void idle(void)
{
    glutPostRedisplay();
}



void mouse(int btn, int state, int x, int y){

    float scale = 54*(Wwidth/Wheight);

   //  printf(" original %d %d \n", x,y);
    switch(btn){
        case GLUT_LEFT_BUTTON:

        if(state==GLUT_DOWN){

               // get new mouse coordinates for x,y
               // use scale to match right
              if(numbOfClicks < MAX_CLICKS)
                {
                    mouseX = (x - (Wwidth/2))/scale; // Get the mouse x and y coordinate, and translate based on origin.
                    mouseY = ((Wheight/2) - y)/scale;

                }
             }
            break;

        case GLUT_MIDDLE_BUTTON:
        break;

        case GLUT_RIGHT_BUTTON:
        break;
    }
  glutPostRedisplay();
};



static void init(void)
{
    glewInit();
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

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

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    createVBO();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("buffers");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}