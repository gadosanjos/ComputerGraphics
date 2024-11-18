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

GLuint Tex;

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
bool s_flag, r_flag, scale_flag;

GLfloat xOffset, yOffset, zOffset = 0.0f;

// Scaling
GLfloat scaleFactorX, scaleFactorY, scaleFactorZ;

// rotation angle variables for glRotatef
GLfloat cubeAngleX, cubeAngleY = 0.0f;
GLfloat skyBoxAngleX, skyBoxAngleY;
GLfloat sbMulC;//for skyBox

// Texture Handling
bool skyboxTextureLoaded = false;

// **_________________________CAMERA

// Camera position
GLfloat baseAngleX = 1.5708f, baseAngleY = 0.0f, baseAngleZ = 0.0f,radius = 7.0f;
GLfloat eyeXprime = radius*cosf(baseAngleX), eyeYprime = radius*sinf(baseAngleY), eyeZprime = 7.0f;
GLfloat rotationAngleX = 0.0f,rotationAngleY = 0.0f, eyeX = eyeXprime, eyeY = eyeYprime, eyeZ = eyeZprime;


/* GLUT callback Handlers */

// Modify the TextureLoader function to return a bool indicating whether the texture was loaded successfully. 
bool TextureLoader(GLuint textureID, const char* fileName){
    unsigned char* image;              // Place your file name
    int width, height;                 // width & height for the image reader

    glBindTexture(GL_TEXTURE_2D, textureID); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.

    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

    if(!image){
        cout << "SOIL loading error: " << SOIL_last_result() << "' (" << fileName << ")" << endl;
        return false;  // Return false if loading fails
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // biding image data
    SOIL_free_image_data(image);

    // Set texture parameters for wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    return true;  // Return true if loading is successful
}

void createSkyBox() {
    //DRAW THE SKYBOX
    glBindTexture(GL_TEXTURE_2D, Tex);//bind skyBox texture to skyBox
    glPushMatrix();

    glRotatef(skyBoxAngleX, 1, 0, 0);      //rotates around X axis
    glRotatef(skyBoxAngleY, 0, 1, 0);      //rotates around Y axis  
    glScalef(sbMulC, sbMulC, sbMulC);      //zoom in/out depending on user input

    //had to draw these inverse transpose 
    // 0,0 tex(bottom-left of current face)
    // 1,0 tex(bottom-right of current face)
    // 1,1 tex(top-right of current face)
    // 0,1 tex(top-left of current face)
    // vertex values
    // -1, -1, -1 = v1 = back lower left   = BLL 
    // -1, -1,  1 = v2 = front lower left  = FLL
    // -1,  1, -1 = v3 = back upper left   = BUL
    // -1,  1,  1 = v4 = front upper left  = FUL
    //  1, -1, -1 = v5 = back lower right  = BLR
    //  1, -1,  1 = v6 = front lower right = FLR
    //  1,  1, -1 = v7 = back upper right  = BUR
    //  1,  1,  1 = v8 = front upper right = FUR

    //Draw the back face of the skybox    
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(.75, .667);glVertex3f(-1, -1, -1);
    glTexCoord2f(1, .667); glVertex3f(1, -1, -1);
    glTexCoord2f(1, .333); glVertex3f(1, 1, -1);
    glTexCoord2f(.75, .333); glVertex3f(-1, 1, -1);

    //Draw the left face of the skybox    
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, .667);      glVertex3f(1, -1, -1);
    glTexCoord2f(.25, .667);    glVertex3f(1, -1, 1);
    glTexCoord2f(.25, .333);    glVertex3f(1, 1, 1);
    glTexCoord2f(0, .333);      glVertex3f(1, 1, -1);

    //Draw the top face of the skybox
    glNormal3f(0, -1, 0);
    glTexCoord2f(.25, .333);    glVertex3f(1, 1, 1);
    glTexCoord2f(.5, .333);     glVertex3f(-1, 1, 1);
    glTexCoord2f(.5, 0);        glVertex3f(-1, 1, -1);
    glTexCoord2f(.25, 0);       glVertex3f(1, 1, -1);

    //Draw the right face of the skybox    
    glNormal3f(1, 0, 0);
    glTexCoord2f(.5, .667);    glVertex3f(-1, -1, 1);
    glTexCoord2f(.75, .667);   glVertex3f(-1, -1, -1);
    glTexCoord2f(.75, .333);   glVertex3f(-1, 1, -1);
    glTexCoord2f(.5, .333);    glVertex3f(-1, 1, 1);

    //Draw the bottom face of the skybox
    glNormal3f(0, 1, 0);
    glTexCoord2f(.25, 1);    glVertex3f(1, -1, -1);
    glTexCoord2f(.5, 1);     glVertex3f(-1, -1, -1);
    glTexCoord2f(.5, .667);  glVertex3f(-1, -1, 1);
    glTexCoord2f(.25, .667); glVertex3f(1, -1, 1);

    //Draw the front face of the skybox
    glNormal3f(0, 0, -1);
    glTexCoord2f(.25, .667);    glVertex3f(1, -1, 1);
    glTexCoord2f(.5, .667);     glVertex3f(-1, -1, 1);
    glTexCoord2f(.5, .333);     glVertex3f(-1, 1, 1);
    glTexCoord2f(.25, .333);    glVertex3f(1, 1, 1);

    glEnd();
    glPopMatrix();
}

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // (eyeX,,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ)  
    // eye = Specifies the position of the eye point 
    // center = Specifies the position where you are looking at
    // up = Specifies the direction of the up vector
    gluLookAt(eyeX+xOffset,eyeY+yOffset,eyeZ+zOffset,0,0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    // Two other operations are also useful for manipulating the matrix stack:
    // glPushMatrix(), glPopMatrix() makes it so that what is made inside does not affect other objects
    // if not used the first obj will be in relation to origin/translatef, and next will be in relation to previous trasnlatef and so on
    // REFERENCE CUBE FOR CAMERA DEBUGGING
    glPushMatrix();
    glColor3f(1.0f,0.0f,0.0f);  // Sets Color
    glTranslatef(0.0, 0.0, 0.0); // Sets new position
    glRotatef(cubeAngleX, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glRotatef(cubeAngleY, 1, 0, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX,scaleFactorY,scaleFactorZ); // Scale factors along the (x axis, y axis, z axis)
    glutSolidCube(1); // Draws Cube
    glPopMatrix();

    // Draw RGB color cube
    float cubeSize = 1.0f;

    // Color coordinates (Red, Green, Blue)
    struct Cube {
        float x, y, z;   // Position
        float r, g, b;   // Color
    };

    // Positions and colors for each vertex in the RGB cube
    Cube cubes[] = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},  // Black
        {1.0, 0.0, 0.0, 1.0, 0.0, 0.0},  // Red
        {0.0, 1.0, 0.0, 0.0, 1.0, 0.0},  // Green
        {0.0, 0.0, 1.0, 0.0, 0.0, 1.0},  // Blue
        {1.0, 1.0, 0.0, 1.0, 1.0, 0.0},  // Yellow
        {1.0, 0.0, 1.0, 1.0, 0.0, 1.0},  // Magenta
        {0.0, 1.0, 1.0, 0.0, 1.0, 1.0},  // Cyan
        {1.0, 1.0, 1.0, 1.0, 1.0, 1.0}   // White
    };

    // CUBE
    glBindTexture(GL_TEXTURE_2D, 0);  // Bind no texture (default to color)
    for (int i = 0; i < 8; ++i){
        glPushMatrix();
        glColor3f(cubes[i].r, cubes[i].g, cubes[i].b);      // Set cube color
        glTranslatef(cubes[i].x, cubes[i].y, cubes[i].z);   // Sets new position
        glRotatef(cubeAngleX, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
        glRotatef(cubeAngleY, 1, 0, 0 ); // Rotation (angle of rotation, xyz coord of vector)
        glScalef(scaleFactorX,scaleFactorY,scaleFactorZ); // Scale factors along the (x axis, y axis, z axis)
        glutSolidCube(cubeSize); // Draws Cube
        glPopMatrix(); 
    }
    
    glColor3f(1.0f,1.0f,1.0f);
    // SKYBOX
    createSkyBox();

    glutSwapBuffers(); // Swap from rendering buffer to viewing buffer made available in glutInitDisplayMode(GLUT_DOUBLE)
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 'Q':
            exit(0);
            break;
        case 'u': // Unselect all
            s_flag = false;
            scale_flag = false;
            break;
        case ' ': // Start
            s_flag = true;
            scale_flag = false;
            break;
        case '+':
            scale_flag = true;
            break;
        case 'a': // Reset
            cubeAngleX += 1.5;
            if(r_flag){
                cubeAngleX = 0;
                r_flag = false;
            }
            break;
        case 'w': // Reset
            cubeAngleY -= 1.5;
            if(r_flag){
                cubeAngleY = 0;
                r_flag = false;
            }
            break;
        case 's': // Reset
            cubeAngleY += 1.5;
            if(r_flag){
                cubeAngleY = 0;
                r_flag = false;
            }
            break;
        case 'd': // Reset
            cubeAngleX -= 1.5;
            if(r_flag){
                cubeAngleX = 0;
                r_flag = false;
            }
            break;
        case 'j': // Reset
            xOffset -= 1.5;
            if(r_flag){
                xOffset = 0;
                r_flag = false;
            }
            break;
        case 'k': // Reset
            yOffset -= 1.5;
            if(r_flag){
                yOffset = 0;
                r_flag = false;
            }
            break;
        case 'i': // Reset
            yOffset += 1.5;
            if(r_flag){
                yOffset = 0;
                r_flag = false;
            }
            break;
        case 'l': // Reset
            xOffset += 1.5;
            if(r_flag){
                xOffset = 0;
                r_flag = false;
            }
            break;
        case 'z': // Reset
            zOffset -= 1.5;
            if(r_flag){
                zOffset = 0;
                r_flag = false;
            }
            break;
        case 'x': // Reset
            zOffset += 1.5;
            if(r_flag){
                zOffset = 0;
                r_flag = false;
            }
            break;
        case 'o': // Reset
            r_flag = true;
            break;
        case 'O': // Reset
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
        // Rotate down around x-axis
        case GLUT_KEY_DOWN:
            rotationAngleX = 0.0f;
            eyeX = eyeXprime;
            eyeY = eyeYprime;
            eyeZ = eyeZprime;
            if(r_flag){
                rotationAngleX = 0.0f;
                rotationAngleY = 0;
                eyeX = eyeXprime;
                eyeY = eyeYprime;
                eyeZ = eyeZprime;
                r_flag = false;
            } else {
                rotationAngleY += 0.1;

                eyeY = cosf(rotationAngleY)*eyeYprime - sinf(rotationAngleY)*eyeZprime; // Horizontal movement (X-axis)
                eyeZ = sinf(rotationAngleY)*eyeYprime + cosf(rotationAngleY)*eyeZprime; // Depth movement (Z-axis)

                // Wrapping logic to prevent rotationAngle from becoming excessively large or negative
                if (rotationAngleY > 2 *  3.14159) rotationAngleY -= 2 *  3.14159;
                if (rotationAngleY < 0) rotationAngleY += 2 * 3.14159;
            }
            break;
        // Rotate up around x-axis
        case GLUT_KEY_UP:
        
            eyeX = eyeXprime;
            eyeY = eyeYprime;
            eyeZ = eyeZprime;
            if(r_flag){
                rotationAngleX = 0.0f;
                rotationAngleY = 0;
                eyeX = eyeXprime;
                eyeY = eyeYprime;
                eyeZ = eyeZprime;
                r_flag = false;
            } else {
                rotationAngleY -= 0.1;
                eyeX = eyeXprime;
                eyeY = eyeYprime;

                eyeY = cosf(rotationAngleY)*eyeYprime - sinf(rotationAngleY)*eyeZprime; // Horizontal movement (X-axis)
                eyeZ = sinf(rotationAngleY)*eyeYprime + cosf(rotationAngleY)*eyeZprime; // Depth movement (Z-axis)

                // Wrapping logic to prevent rotationAngleY from becoming excessively large or negative
                if (rotationAngleY > 2 *  3.14159) rotationAngleY -= 2 *  3.14159;
                if (rotationAngleY < 0) rotationAngleY += 2 * 3.14159;
            }
            break;
        // Rotate left around y-axis
        case GLUT_KEY_LEFT:
            if(r_flag){
                rotationAngleX = 0;
                rotationAngleY = 0;
                eyeX = eyeXprime;
                eyeY = eyeYprime;
                eyeZ = eyeZprime;
                r_flag = false;
            } else {
                rotationAngleX += 0.1;
                cout << rotationAngleX;

                eyeX = cosf(rotationAngleX)*eyeXprime + sinf(rotationAngleX)*eyeZprime; // Horizontal movement (X-axis)
                eyeZ = cosf(rotationAngleX)*eyeZprime - sinf(rotationAngleX)*eyeXprime; // Depth movement (Z-axis)

                // Wrapping logic to prevent rotationAngleX from becoming excessively large or negative
                if (rotationAngleX > 2 *  3.14159) rotationAngleX -= 2 *  3.14159;
                if (rotationAngleX < 0) rotationAngleX += 2 * 3.14159;
            }
            break;
        // Rotate right around y-axis
        case GLUT_KEY_RIGHT:
            if(r_flag){
                rotationAngleX = 0;
                rotationAngleY = 0;
                eyeX = eyeXprime;
                eyeY = eyeYprime;
                eyeZ = eyeZprime;
                r_flag = false;
            } else {
                rotationAngleX -= 0.1;

                eyeX = cosf(rotationAngleX)*eyeXprime + sinf(rotationAngleX)*eyeZprime; // Horizontal movement (X-axis)
                eyeZ = cosf(rotationAngleX)*eyeZprime - sinf(rotationAngleX)*eyeXprime; // Depth movement (Z-axis)
                
                // Wrapping logic to prevent rotationAngleX from becoming excessively large or negative
                if (rotationAngleX > 2 *  3.14159) rotationAngleX -= 2 *  3.14159;
                if (rotationAngleX < 0) rotationAngleX += 2 * 3.14159;
            }
            break;
        // Scale x-axis
        case GLUT_KEY_F1: 
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
        // Scale y-axis
        case GLUT_KEY_F3: 
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
        // Scale z-axis
        case GLUT_KEY_F5: 
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
    glutPostRedisplay(); // used to keep events going, since glut usually wont keep look going if no events happen, this will can render again
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

    // Flags
    s_flag = r_flag = scale_flag = false;

    // Scaling
    scaleFactorX = scaleFactorY = scaleFactorZ = 1;

    // Texture handling
    glEnable(GL_TEXTURE_2D);    // Enable 2D texture mapping
    //glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D

    // Generate texture IDs for each skybox
    glGenTextures(1, &Tex);

    skyboxTextureLoaded = TextureLoader(Tex, "../images/skybox_image.jpg");

    //scale multiplier (set to 1 when not in use)
    sbMulC = 20;  //for skyBox

    //rotation degree variables for glRotatef
    cubeAngleX = 0, cubeAngleY = 0;
    skyBoxAngleX = 0, skyBoxAngleY = 180;
}           

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1280,720);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Sky Box");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}