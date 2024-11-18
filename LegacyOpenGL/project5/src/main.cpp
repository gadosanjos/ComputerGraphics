#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <vector>

#include <SOIL.h>
#include <math.h>

#include <string>
#include "fileRead.h"

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

/* Variables Handlers */

/* Model variables */
int selection = 6;

const char* objFiles[6] = {
    "../models/cube.obj",
    "../models/cow.obj",
    "../models/ele.obj",
    "../models/trunk.obj",
    "../models/ven-2.obj",
    "../models/ateneam-2.obj"
};

std::vector<vec3> vertices[6];         // Arrays of vectors to store vertices
std::vector<vec3Tex> textureCoords[6]; // Arrays of vectors to store texture coordinates
std::vector<vec3> normals[6];          // Arrays of vectors to store normals
std::vector<face> faces[6];            // Arrays of vectors to store faces (v/t/n)
std::vector<face2> faces2[6];          // Arrays of vectors to store faces (v//n)

/* Object selection variables */
bool s_flag, r_flag, scale_flag;

// Matrix Calculations
const float PI = 3.14159;

// Scaling

GLfloat scaleFactorX, scaleFactorY, scaleFactorZ;

// rotation angle variables for glRotatef
GLfloat modelAngleX, modelAngleY;
GLfloat skyBoxAngleX, skyBoxAngleY;
GLfloat sbMulC;//for skyBox

// Texture Handling
GLuint Tex[4], SkyTex[4];
int TexIndex = 0;
bool objTextureLoaded = false;
bool skyboxTextureLoaded = false;

/* Camera position */
GLfloat radius = 10.0f;
GLfloat rotationSpeed = 0.1f; // Adjust rotation speed
GLfloat rotationAngleX = 0.0f;
GLfloat rotationAngleY = 0.0f; 
GLfloat centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f; // Look-at point
GLfloat upX = 0.0f, upY = 1.0f, upZ = 0.0f; // Camera's look-at point

// Initialize eye position using spherical coordinates
GLfloat eyeX = centerX + radius * cosf(rotationAngleY) * sinf(rotationAngleX);
GLfloat eyeY = centerY + radius * sinf(rotationAngleY);
GLfloat eyeZ = centerZ + radius * cosf(rotationAngleY) * cosf(rotationAngleX);

/* GLUT callback Handlers */

// Function to render models with v/t/n
void renderModel(const std::vector<vec3>& vertices, const std::vector<vec3Tex>& textureCoords, const std::vector<vec3>& normals, const std::vector<face>& faces, int TexIndex) {
    glBindTexture(GL_TEXTURE_2D, Tex[TexIndex]);
    
    glPushMatrix();
    glRotatef(modelAngleX, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glRotatef(modelAngleY, 1, 0, 0 ); // Rotation (angle of rotation, xyz coord of vector)

    glBegin(GL_TRIANGLES);
    for (const auto& f : faces) {
        for (int i = 0; i < 3; ++i) { // Each face has 3 vertices
            // Get indices
            int vIndex = f.vertices[i].vIndex - 1;
            int tIndex = f.vertices[i].tIndex - 1;
            int nIndex = f.vertices[i].nIndex - 1;

            // Set normal
            if (nIndex >= 0 && nIndex && normals.size()) {
                glNormal3f(normals[nIndex].x, normals[nIndex].y, normals[nIndex].z);
            }

            // Set texture coordinate
            if (tIndex >= 0 && tIndex < textureCoords.size()) {
                glTexCoord3f(textureCoords[tIndex].u, textureCoords[tIndex].v, textureCoords[tIndex].w);
            }

            // Set vertex position
            if (vIndex >= 0 && vIndex < vertices.size()) {
                glVertex3f(vertices[vIndex].x, vertices[vIndex].y, vertices[vIndex].z);
            }
        }
    }
    glEnd();
    glPopMatrix();
}

// Function to render models with v//n
void renderModel2(const std::vector<vec3>& vertices, const std::vector<vec3>& normals, const std::vector<face2>& faces){
    glBindTexture(GL_TEXTURE_2D, 0); // Disable textures for now

    glPushMatrix();
    glRotatef(modelAngleX, 0, 1, 0 ); // Rotation (angle of rotation, xyz coord of vector)
    glRotatef(modelAngleY, 1, 0, 0 ); // Rotation (angle of rotation, xyz coord of vector)

    glBegin(GL_TRIANGLES);
    for (const auto& f : faces) {
        for (int i = 0; i < 3; ++i) { // Each face has 3 vertices
            // Get indices
            int vIndex = f.vertices[i].vIndex - 1;
            int nIndex = f.vertices[i].nIndex - 1;

            // Set normal
            if (nIndex >= 0 && nIndex < normals.size()) {
                glNormal3f(normals[nIndex].x, normals[nIndex].y, normals[nIndex].z);
            }

            // Set vertex position
            if (vIndex >= 0 && vIndex < vertices.size()) {
                glVertex3f(vertices[vIndex].x, vertices[vIndex].y, vertices[vIndex].z);
            }
        }
    }
    glEnd();
    glPopMatrix();
}

void loadModel(int selection, int TexIndex){
    switch(selection) {
        case 1:
            glPushMatrix();
            glTranslatef(0,0,0);
            renderModel(vertices[0], textureCoords[0], normals[0], faces[0], TexIndex); //"../models/cube.obj", works
            glPopMatrix();
            break;
        case 2:
            glPushMatrix();
            glTranslatef(0,0,0);
            glTranslatef(0,0,0);
            renderModel2(vertices[1], normals[1], faces2[1]); //"../models/cow.obj", works
            glPopMatrix();
            break;
        case 3:
            glPushMatrix();
            glTranslatef(0,-4,0);
            glRotatef(90,0,1,0);
            glScalef(0.01,0.01,0.01);
            renderModel2(vertices[2], normals[2], faces2[2]);//"../models/ele.obj",
            glPopMatrix();
            break;
        case 4:
            glPushMatrix();
            glTranslatef(0,-3,0);
            glRotatef(90,0,1,0);
            glScalef(0.002,0.002,0.002);
            renderModel2(vertices[4], normals[4], faces2[4]);//"../models/ven-2.obj",
            glPopMatrix();
            break;
        case 5:
            glPushMatrix();
            glTranslatef(0,-6,0);
            glTranslatef(0,0,0);
            renderModel2(vertices[3], normals[3], faces2[3]);//"../models/trunk.obj", works
            glPopMatrix();
            break;
        case 6:
            glPushMatrix();
            glScalef(0.002,0.002,0.002);
            renderModel(vertices[5], textureCoords[5], normals[5], faces[5], TexIndex);//"../models/ateneam-2.obj"
            glPopMatrix();
            break;
        default:
            break;        
    }
}

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

void createSkyBox(int TextIndex) {
    //DRAW THE SKYBOX
    glBindTexture(GL_TEXTURE_2D, SkyTex[TextIndex]);//bind skyBox texture to skyBox
    glDisable(GL_DEPTH_TEST);                      // Disable depth test

    glPushMatrix();
    
    glTranslatef(eyeX, eyeY, eyeZ); // Move skybox to the camera position
    glRotatef(skyBoxAngleX, 1, 0, 0);      //rotates around X axis
    glRotatef(skyBoxAngleY, 0, 1, 0);      //rotates around Y axis
    glScalef(sbMulC, sbMulC, sbMulC);      //zoom in/out depending

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
    glTexCoord2f(.75, .667);    glVertex3f(-1, -1, -1); // Bottom-left
    glTexCoord2f(1, .667);      glVertex3f(1, -1, -1);  // Bottom-right
    glTexCoord2f(1, .333);      glVertex3f(1, 1, -1);   // Top-right
    glTexCoord2f(.75, .333);    glVertex3f(-1, 1, -1);  // Top-left

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

    glEnable(GL_DEPTH_TEST);  // Re-enable depth test
}

static void resize(int width, int height)
{
    double Ratio = (double)width / (double)height;;

   if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60.0f,Ratio,0.1f, 1500.0f);
 }

void updateCameraPosition() {
    // Calculate eye position using spherical coordinates
    eyeX = centerX + radius * cosf(rotationAngleY) * sinf(rotationAngleX);
    eyeY = centerY + radius * sinf(rotationAngleY);
    eyeZ = centerZ + radius * cosf(rotationAngleY) * cosf(rotationAngleX);

    // Update the view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    updateCameraPosition();

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // SKYBOX
    createSkyBox(TexIndex);

    // Model
    loadModel(selection, TexIndex);

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
        case ' ': // RESET
            r_flag = true;
            r_flag = true;
            scale_flag = false;
            break;
        case 'u': // Unselect all
            s_flag = false;
            scale_flag = false;
            break;
        case '+':
            scale_flag = true;
            break;
        case '1': // Model Selection
            selection = 1;
            break;
        case '2': // Model Selection
            selection = 2;
            break;
        case '3': // Model Selection
            selection = 3;
            break;
        case '4': // Model Selection
            selection = 4;
            break;
        case '5': // Model Selection
            selection = 5;
            break;
        case '6': // Model Selection
            selection = 6;
            break;
        case '7': // Texture Selection
            TexIndex = 0;
            break;
        case '8': // Texture Selection
            TexIndex = 1;
            break;
        case '9': // Texture Selection
            TexIndex = 2;
            break;
        case '0': // Texture Selection
            TexIndex = 3;
            break;
        case 'a': // Rotate Model Left
            modelAngleX -= 1.5;
            break;
        case 'w': // Rotate Model UP
            modelAngleY -= 1.5;
            break;
        case 's': // Rotate Model Down
            modelAngleY += 1.5;
            break;
        case 'd': // Rotate Model Right
            modelAngleX += 1.5;
            break;
        case 'j': // Move Camera Left
            eyeX -= 0.5;
            centerX -= 0.5;
            break;
        case 'k': // Move Camera Down
            eyeY -= 0.5;
            centerY -= 0.5;
            break;
        case 'i': // Move Camera Up
            eyeY += 0.5;
            centerY += 0.5;
            break;
        case 'l': // Move Camera Right
            eyeX += 0.5;
            centerX += 0.5;
            break;
        case 'z': // Zoom IN
            radius -= 0.5;
            if (radius < 2.0) radius = 2.0; // Prevent zooming too close
            break;
        case 'x': // Zoom OUT
            radius += 0.5;
            break;
        case 'o': // Reset Camera
            radius = 10.0f;
            rotationAngleX = 0.0f;
            rotationAngleY = 0.0f;
            centerX = 0.0f;
            centerY = 0.0f;
            centerZ = 0.0f;
            break;
        case 'O': // Reset Camera
            radius = 10.0f;
            rotationAngleX = 0.0f;
            rotationAngleY = 0.0f;
            centerX = 0.0f;
            centerY = 0.0f;
            centerZ = 0.0f;
            break;
        default:
            break;
    }
    updateCameraPosition();
    glutPostRedisplay();
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {   
        case GLUT_KEY_RIGHT:  // Rotate right
            rotationAngleX += rotationSpeed;
            if (rotationAngleX > 2 * PI) rotationAngleX -= 2 * PI;
            break;

        case GLUT_KEY_LEFT:   // Rotate left
            rotationAngleX -= rotationSpeed;
            if (rotationAngleX < 0) rotationAngleX += 2 * PI;
            break;

        case GLUT_KEY_UP:     // Rotate up
            rotationAngleY += rotationSpeed;
            if (rotationAngleY > PI / 2) rotationAngleY = PI / 2; // Limit vertical rotation
            break;

        case GLUT_KEY_DOWN:   // Rotate down
            rotationAngleY -= rotationSpeed;
            if (rotationAngleY < -PI / 2) rotationAngleY = -PI / 2;
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
    updateCameraPosition();
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

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    // Flags
    s_flag = r_flag = scale_flag = false;

    // Scaling
    scaleFactorX = scaleFactorY = scaleFactorZ = 1;

    // Texture handling
    glEnable(GL_TEXTURE_2D);    // Enable 2D texture mapping
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D

    // Generate texture IDs for each skybox
    glGenTextures(4, SkyTex);

    skyboxTextureLoaded = TextureLoader(SkyTex[0], "../images/skybox_debugger.jpg");
    skyboxTextureLoaded = TextureLoader(SkyTex[1], "../images/skybox_forest.png");
    skyboxTextureLoaded = TextureLoader(SkyTex[2], "../images/skybox_sky.png");
    skyboxTextureLoaded = TextureLoader(SkyTex[3], "../images/skybox_universe.jpg");

    // Generate texture IDs for each model
    glGenTextures(4, Tex);

    // Load each texture
    objTextureLoaded = TextureLoader(Tex[0] , "../images/2k_earth_daymap.jpg");
    objTextureLoaded = TextureLoader(Tex[1] , "../images/2k_moon.jpg");
    objTextureLoaded = TextureLoader(Tex[2] , "../images/2k_mars.jpg");
    objTextureLoaded = TextureLoader(Tex[3] , "../images/2k_sun.jpg");

    //scale multiplier (set to 1 when not in use)
    sbMulC = 100.0f;  //for skyBox

    //rotation degree variables for glRotatef
    skyBoxAngleX = 0, skyBoxAngleY = 180;
    modelAngleX = -90, modelAngleY = 0;

    // Parse all .obj files, Initialize arrays
    for (int i = 0; i < 6; ++i) {
        bool isDoubleSlash = isDoubleSlashFormat(objFiles[i]);

        if (isDoubleSlash){
            if (parseObjFile2(objFiles[i], vertices[i], normals[i], faces2[i])) {
                std::cout << "Successfully parsed: " << objFiles[i] << std::endl;
            } else {
                std::cerr << "Failed to parse: " << objFiles[i] << std::endl;
            }
        } else {
            if (parseObjFile(objFiles[i], vertices[i], textureCoords[i], normals[i], faces[i])) {
                std::cout << "Successfully parsed: " << objFiles[i] << std::endl;
            } else {
                std::cerr << "Failed to parse: " << objFiles[i] << std::endl;
            }
        }
    }
}           

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1280,720);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Model Loader");
    init();

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}