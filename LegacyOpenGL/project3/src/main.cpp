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

#include <chrono>
#include <thread>
#include <atomic> // for atomic flag

using namespace std;

bool WireFrame= false;

GLuint earthTex, moonTex, marsTex, sunTex; // Texture pointer

// Set the position of the light to be at the sun's location
const GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Sun at the origin

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

// **_________________________PLANETS

// global variable
GLUquadricObj *sphere= NULL; 

// Object position
GLfloat earthX, earthY, earthZ, moonX, moonY, moonZ, marsX, marsY, marsZ;

// Orbital angles for the planets
GLfloat earthAngle = 0.0f;
GLfloat moonAngle = 0.0f;
GLfloat marsAngle = 0.0f;

// Orbital radii (distances from the sun)
GLfloat earthOrbitRadius, moonOrbitRadius, marsOrbitRadius;

// Scaling
GLfloat scaleFactorX, scaleFactorY, scaleFactorZ;

// Position offsets
GLfloat sphereOffset;

// Rotation offsets
GLint planetAngle;

// Texture Handling
bool earthTextureLoaded, moonTextureLoaded, marsTextureLoaded,sunTextureLoaded = false;

// **_________________________CAMERA

// Camera position
GLfloat rotationAngle = 1.5f, radius = 35.0f, eyeX = 0.0f, eyeY = 20.0f, eyeZ = 35.0f;

// Camera offsets
GLfloat eyeZOffset;

// **_________________________THREADS

// Thread Handling
thread planetThread;
std::atomic<bool> isThreadRunning(false);  // Track if a thread is running

/* GLUT callback Handlers */

// Calculations done first in desmos https://www.desmos.com/geometry/v6ezottir7
void planetRotations() {
    while (s_flag) {
        // Reduce sphere angle for rotation
        planetAngle -= 15;
        earthAngle += 0.1f; // Earth's angular velocity
        moonAngle += 0.5f;  // Moon's angular velocity (faster orbit)
        marsAngle += 0.09f; // Mars' angular velocity (slower orbit)

        // Oribiting rotation
        earthX = earthOrbitRadius * cos(earthAngle);
        earthZ = earthOrbitRadius * sin(earthAngle);

        moonX = earthX + moonOrbitRadius * cos(moonAngle);
        moonZ = earthZ + moonOrbitRadius * sin(moonAngle);

        marsX = marsOrbitRadius * cos(marsAngle);
        marsZ = marsOrbitRadius * sin(marsAngle);

        // Add a small sleep to avoid excessive CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Check for a full rotation reset
        if (planetAngle < 0) {
            planetAngle += 360; // Keep it within 0-360 degrees
        }

    }
    // When s_flag becomes false, the loop ends, and the thread terminates
    isThreadRunning = false;  // Mark that the thread has finished
}

bool TextureLoader(GLuint textureID, const char* fileName);

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

    // (eyeX,,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ)  
    // eye = Specifies the position of the eye point 
    // center = Specifies the position where you are looking at
    // up = Specifies the direction of the up vector
    gluLookAt(eyeX,eyeY,eyeZ+eyeZOffset,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    // Two other operations are also useful for manipulating the matrix stack:
    // glPushMatrix(), glPopMatrix() makes it so that what is made inside does not affect other objects
    // if not used the first obj will be in relation to origin/translatef, and next will be in relation to previous trasnlatef and so on

    // REFERENCE CUBE FOR CAMERA DEBUGGING
    /* glPushMatrix();
    glColor3f(1.0f,1.0f,1.0f);  // Sets Color
    glTranslatef(0.0, 0.0, 0.0); // Sets new position
    glRotatef(0 % 360, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX,scaleFactorY,scaleFactorZ); // Scale factors along the (x axis, y axis, z axis)
    glutSolidCube(5); // Draws Cube
    glPopMatrix(); */

    // SUN
    glPushMatrix();
    if(sunTextureLoaded) {
        glBindTexture(GL_TEXTURE_2D, sunTex);  // Bind sun texture if loaded
        glColor3f(1.0f, 1.0f, 1.0f);  // Ensure no color tinting
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);  // Bind no texture (default to color)
        glColor3f(1.0f, 1.0f, 0.0f);  // Fallback yellow color for the Sun
    }
    glTranslatef(0, 0, 0); // Sets new position
    glRotatef(planetAngle, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX,scaleFactorY,scaleFactorZ); // Scale factors along the (x axis, y axis, z axis)
    gluSphere(sphere,3.0, 40,40); // Draws Sphere (radius, slices, stack)
    glPopMatrix();

    // Earth
    glPushMatrix();
    if (earthTextureLoaded) {
        glBindTexture(GL_TEXTURE_2D, earthTex);  // Bind earth texture if loaded
        glColor3f(1.0f, 1.0f, 1.0f);  // Ensure no color tinting
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);  // Bind no texture (default to color)
        glColor3f(0.0f, 0.0f, 1.0f);  // Fallback blue color for the Earth
    }
    glTranslatef(earthX, earthY, earthZ+sphereOffset); // Sets new position
    glRotatef(planetAngle, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX/2,scaleFactorY/2,scaleFactorZ/2); // Scale factors along the (x axis, y axis, z axis)
    gluSphere(sphere,3.0, 40,40); // Draws Sphere (radius, slices, stack)
    glPopMatrix();

    // Moon
    glPushMatrix();
    if (moonTextureLoaded) {
        glBindTexture(GL_TEXTURE_2D, moonTex);  // Bind moon texture if loaded
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);  // Bind no texture (default to color)
        glColor3f(0.5f, 0.5f, 0.5f);  // Fallback gray color for the Moon
    }
    glTranslatef(moonX, moonY, moonZ+sphereOffset); // Sets new position
    glRotatef(planetAngle, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX/7,scaleFactorY/7,scaleFactorZ/7); // Scale factors along the (x axis, y axis, z axis)
    gluSphere(sphere,3.0, 40,40); // Draws Sphere (radius, slices, stack)
    glPopMatrix();

    // Mars
    glPushMatrix();
    if (marsTextureLoaded) {
        glBindTexture(GL_TEXTURE_2D, marsTex);  // Bind mars texture if loaded
        glColor3f(1.0f, 1.0f, 1.0f);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);  // Bind no texture (default to color)
        glColor3f(1.0f, 0.0f, 0.0f);  // Fallback red color for Mars
    }
    glTranslatef(marsX, marsY, marsZ+sphereOffset); // Sets new position
    glRotatef(planetAngle, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glScalef(scaleFactorX/3,scaleFactorY/3,scaleFactorZ/3); // Scale factors along the (x axis, y axis, z axis)
    gluSphere(sphere,3.0, 40,40); // Draws Sphere (radius, slices, stack)
    glPopMatrix();

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
        case 'd': // Unselect all
            s_flag = false;
            scale_flag = false;
            break;
        case ' ': // Sphere
            s_flag = true;
            if (!isThreadRunning) {
                cout << "Round and round we go" << endl;
                isThreadRunning = true;  // Mark that a thread is running
                planetThread = std::thread(planetRotations); // Start a new thread
                planetThread.detach(); // Detach to let it run independently
            }
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
            eyeZOffset += 0.5;
            if(r_flag){
                sphereOffset = 0;
                eyeZOffset = 0;
                s_flag = false;
                r_flag = false;
            }
        break;
        // Zoom out
        case GLUT_KEY_UP:
            eyeZOffset -= 0.5;
            if(r_flag){
                sphereOffset = 0;
                eyeZOffset = 0;
                r_flag = false;
            }
            break;
        // Rotate left around y-axis
        case GLUT_KEY_LEFT:
            if(r_flag){
                planetAngle = 0;
                rotationAngle = 1.5f;
                r_flag = false;
            } else {
                rotationAngle += 0.1;

                eyeX = radius * cosf(rotationAngle); // Horizontal movement (X-axis)
                eyeZ = radius * sinf(rotationAngle); // Depth movement (Z-axis)

                // Wrapping logic to prevent rotationAngle from becoming excessively large or negative
                if (rotationAngle > 2 *  3.14159) rotationAngle -= 2 *  3.14159;
                if (rotationAngle < 0) rotationAngle += 2 * 3.14159;
            }
            break;
        // Rotate right around y-axis
        case GLUT_KEY_RIGHT:
            if(r_flag){
                planetAngle = 0;
                rotationAngle = 1.5f;
                r_flag = false;
            } else {
                rotationAngle -= 0.1;

                eyeX = radius * cosf(rotationAngle); // Horizontal movement (X-axis)
                eyeZ = radius * sinf(rotationAngle); // Depth movement (Z-axis)

                // Wrapping logic to prevent rotationAngle from becoming excessively large or negative
                if (rotationAngle > 2 *  3.14159) rotationAngle -= 2 *  3.14159;
                if (rotationAngle < 0) rotationAngle += 2 * 3.14159;
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

    // Planets initialization sphere
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    // Objects initial position
    earthX = -8;
    earthY = 0.0;
    earthZ = 0.0;

    moonX = -10;
    moonY = 0.0;
    moonZ = 0.0;

    marsX = -15;
    marsY = 0.0;
    marsZ = 0.0;

    earthOrbitRadius = -8.0f; // Negative to match initial position
    moonOrbitRadius = -2.0f;
    marsOrbitRadius = -15.0f;

    // Flags
    s_flag = r_flag = scale_flag = false;

    // Scaling
    scaleFactorX = scaleFactorY = scaleFactorZ = 1;

    // Position offset
    sphereOffset = 0;

    // Rotation offset
    planetAngle = 0 % 360;

    // Texture handling
    glEnable(GL_TEXTURE_2D);    // Enable 2D texture mapping
    //glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D

    // Generate texture IDs for each planet
    glGenTextures(1, &earthTex);
    glGenTextures(1, &moonTex);
    glGenTextures(1, &marsTex);
    glGenTextures(1, &sunTex);

    // Load each texture
    earthTextureLoaded = TextureLoader(earthTex, "../images/2k_earth_daymap.jpg");
    moonTextureLoaded = TextureLoader(moonTex, "../images/2k_moon.jpg");
    marsTextureLoaded = TextureLoader(marsTex, "../images/2k_mars.jpg");
    sunTextureLoaded = TextureLoader(sunTex, "../images/2k_sun.jpg");
    
}           

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Planets Orbitating the Sun");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}

// Modify the TextureLoader function to return a bool indicating whether the texture was loaded successfully. 
// If the texture loading fails, we skip binding the texture and use the fallback color instead.
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
