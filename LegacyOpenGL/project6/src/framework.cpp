#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <SOIL.h>

#define PI 3.14159
#define NUM_LAYERS 6

using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };
// Player States
enum PlayerState { IDLE, RUNNING, ATTACKING };
PlayerState playerState = IDLE; // Default state
enum Direction { LEFT, RIGHT };
Direction playerDirection = RIGHT; // Default direction
bool isRunning = false; // Tracks if running keys are pressed
bool isAttacking = false; // Tracks if the attack key is pressed
static int attackAnimationStartTime = -1; // Tracks the start of the attack animation
bool isMouseHeld = false; // Tracks if the left mouse button is being held

// Texture Handling
GLuint Tex[4], SkyTex[6];
int TexIndex = 0;
bool objTextureLoaded = false;
bool skyboxTextureLoaded = false;
float Wwidth,Wheight;

// Background
GLfloat sbMulC;//for skyBox
float xmin[NUM_LAYERS] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}; // Start of the texture for each layer
float xmax[NUM_LAYERS] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f}; // End of the texture for each layer
float parallaxFactors[NUM_LAYERS] = {0.5f,0.3f,0.25f,0.2f,0.15f,0.1}; // Parallax speed for each layer Foreground, midground, background
float playerPositionX = 0.0f; // Track player's movement

// Variables for mouse interaction
int numbOfClicks = 0; // Track the number of clicks
const int MAX_CLICKS = 100; // Maximum allowed clicks
float mouseX = 0.0f; // Mouse X coordinate
float mouseY = 0.0f; // Mouse Y coordinate

/* GLUT callback Handlers */
void CharacterStateMachine()
{
    switch (playerState)
    {
        case IDLE:
            isAttacking = false;
            isRunning = false;
            break;
        case RUNNING:
            isAttacking = false;
            break;
        case ATTACKING:
            //isRunning = false;
            break;
        default:
            break;
    }
}

void playerIdle(){
/* 
    By default, SOIL and similar libraries load image data with the origin at the top-left corner of the image (this is common in most image file formats, like PNG and JPEG).
    Inputs to glTexCoord2f(u, v)
    u (first parameter): The horizontal texture coordinate (X-axis).
    u = 0.0 is the left edge of the texture.
    u = 1.0 is the right edge of the texture.
    v (second parameter): The vertical texture coordinate (Y-axis).
    v = 0.0 is the bottom edge of the texture.
    v = 1.0 is the top edge of the texture.
*/
    int numFrames = 4;          // Total frames in the sprite sheet
    float frameDuration = 0.15f; // Duration of each frame in seconds (10 FPS)

    // Calculate the current frame
    int elapsedTime = glutGet(GLUT_ELAPSED_TIME); // Milliseconds since the program started
    int currentFrame = (int)(elapsedTime / (frameDuration * 1000)) % numFrames;

    // Calculate texture coordinates for the current frame
    float u_start = (float)currentFrame / numFrames;
    float u_end = (float)(currentFrame + 1) / numFrames;

    // Flip texture coordinates for LEFT direction
    if (playerDirection == LEFT)
    {
        std::swap(u_start, u_end);
    }

    glBindTexture(GL_TEXTURE_2D, Tex[0]);
    glPushMatrix();
    glTranslatef(-2.5f,-8.0f,0.0f); 
    glBegin(GL_QUADS);
        glNormal3f(0, 0, -1);
        glTexCoord2f(u_start, 1.0f); glVertex3f(0, 0, 0); // Bottom-left
        glTexCoord2f(u_end, 1.0f);   glVertex3f(5, 0, 0);  // Bottom-right
        glTexCoord2f(u_end, 0.0f);   glVertex3f(5, 5, 0);   // Top-right
        glTexCoord2f(u_start, 0.0f); glVertex3f(0, 5, 0);  // Top-left
    glEnd();
    glPopMatrix();
}

void playerRunning()
{
    int numFrames = 8; // Number of frames in the running sprite sheet
    float frameDuration = 0.1f; // Faster animation for running

    int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    int currentFrame = (int)(elapsedTime / (frameDuration * 1000)) % numFrames;

    float u_start = (float)currentFrame / numFrames;
    float u_end = (float)(currentFrame + 1) / numFrames;

    // Flip texture coordinates for LEFT direction
    if (playerDirection == LEFT)
    {
        std::swap(u_start, u_end);
    }
    
    glBindTexture(GL_TEXTURE_2D, Tex[1]); // Use the running texture
    glPushMatrix();
    glTranslatef(-2.5f,-8.0f,0.0f);  
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(u_start, 1.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(u_end, 1.0f);   glVertex3f(5, 0, 0);
    glTexCoord2f(u_end, 0.0f);   glVertex3f(5, 5, 0);
    glTexCoord2f(u_start, 0.0f); glVertex3f(0, 5, 0);
    glEnd();
    glPopMatrix();
}

void playerAttacking()
{
    int numFrames = 8; // Number of frames in the attacking sprite sheet
    float frameDuration = 0.15f; // Duration of each frame in seconds

    int elapsedTime = glutGet(GLUT_ELAPSED_TIME);

    // Calculate the current frame based on the attack animation timer
    int attackElapsedTime = elapsedTime - attackAnimationStartTime;
    int currentFrame = (int)(attackElapsedTime / (frameDuration * 1000)) % numFrames;

    float u_start = (float)currentFrame / numFrames;
    float u_end = (float)(currentFrame + 1) / numFrames;

    // Flip texture coordinates for LEFT direction
    if (playerDirection == LEFT)
    {
        std::swap(u_start, u_end);
    }

    glBindTexture(GL_TEXTURE_2D, Tex[2]); // Use the attacking texture
    glPushMatrix();
    glTranslatef(-2.5f,-8.0f,0.0f); 
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1);
    glTexCoord2f(u_start, 1.0f); glVertex3f(0, 0, 0);
    glTexCoord2f(u_end, 1.0f);   glVertex3f(5, 0, 0);
    glTexCoord2f(u_end, 0.0f);   glVertex3f(5, 5, 0);
    glTexCoord2f(u_start, 0.0f); glVertex3f(0, 5, 0);
    glEnd();
    glPopMatrix();
}

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

void updateParallax() {
    for (int i = 0; i < NUM_LAYERS; i++) {
        // Scroll the texture coordinates based on the player's movement
        float offset = playerPositionX * parallaxFactors[i];

        // Ensure smooth wrapping by keeping the offset within [0, 1]
        offset = fmod(offset, 1.0f);
        if (offset < 0.0f) offset += 1.0f; // Handle negative offsets

        // Set the new texture coordinates
        xmin[i] = offset;
        xmax[i] = xmin[i] + 1.0f; // Ensure xmax is one unit ahead of xmin
    }
}

void parallaxBackground() {
    for (int i = NUM_LAYERS-1; i >= 0; i--) {
        // Bind the background texture
        glBindTexture(GL_TEXTURE_2D, SkyTex[i]);

        // Disable depth testing to ensure the background is always behind
        glDisable(GL_DEPTH_TEST);                      

        // Render two quads side by side for seamless texture wrapping
        glPushMatrix();
        glScalef(sbMulC, sbMulC, sbMulC);

        // First quad
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glTexCoord2f(xmin[i], 1.0f);    glVertex3f(-1, -1, -1); // Bottom-left
            glTexCoord2f(xmax[i], 1.0f);      glVertex3f(1, -1, -1);  // Bottom-right
            glTexCoord2f(xmax[i], 0.0f);      glVertex3f(1, 1, -1);   // Top-right
            glTexCoord2f(xmin[i], 0.0f);    glVertex3f(-1, 1, -1);  // Top-left 
        glEnd();

        // Second quad (continuation of the texture)
        glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);
            glTexCoord2f(xmin[i] -1.0f, 1.0f); glVertex3f(-1, -1, -1); // Bottom-left
            glTexCoord2f(xmax[i] -1.0f, 1.0f); glVertex3f(1, -1, -1);  // Bottom-right
            glTexCoord2f(xmax[i] -1.0f, 0.0f); glVertex3f(1, 1, -1);   // Top-right
            glTexCoord2f(xmin[i] -1.0f, 0.0f); glVertex3f(-1, 1, -1);  // Top-left
        glEnd();
        glPopMatrix();
    }
    glEnable(GL_DEPTH_TEST);  // Re-enable depth test
}

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

    gluLookAt(0,0,20.0,0.0,0.0,0.0,0.0,1.0,100.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // Draw the background
    parallaxBackground();
    

    switch (playerState)
    {
        case IDLE:
            playerIdle();
            break;
        case RUNNING:
            playerRunning();
            break;
        case ATTACKING:
            playerAttacking();
            break;
        default:
            break;
    }

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
        case 'Q':
            exit(0);
            break;
        case 'a': // move left
            isRunning = true;
            playerState = RUNNING;
            playerDirection = LEFT;
            playerPositionX -= 0.05f; // Adjust movement step as needed
            break;
        case 'd': // move right
            isRunning = true;
            playerState = RUNNING;
            playerDirection = RIGHT;
            playerPositionX += 0.05f; // Adjust movement step as needed
            break;
        case 'A': // move left
            isRunning = true;
            playerState = RUNNING;
            playerDirection = LEFT;
            playerPositionX -= 0.05f; // Adjust movement step as needed
            break;
        case 'D': // move right
            isRunning = true;
            playerState = RUNNING;
            playerDirection = RIGHT;
            playerPositionX += 0.05f; // Adjust movement step as needed
            break;
        case 'j': // Attack key
            if (!isAttacking) // Prevent restarting mid-animation
            {
                isAttacking = true;
                playerState = ATTACKING;
                attackAnimationStartTime = -1; // Reset timer for new attack
            }
            break;
        case 'J': // Attack key
            if (!isAttacking) // Prevent restarting mid-animation
            {
                isAttacking = true;
                playerState = ATTACKING;
                attackAnimationStartTime = -1; // Reset timer for new attack
            }
            break;
	    case 'w':
		    WireFrame =!WireFrame;
	        break;
    }
    // Synchronize state-related flags
    CharacterStateMachine();
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_RIGHT:
            isRunning = true;
            playerState = RUNNING;
            playerDirection = RIGHT;
            playerPositionX += 0.05f; // Adjust movement step as needed
            break;

        case GLUT_KEY_LEFT:
            isRunning = true;
            playerState = RUNNING;
            playerDirection = LEFT;
            playerPositionX -= 0.05f; // Adjust movement step as needed
            break;
    }
    // Synchronize state-related flags
    CharacterStateMachine();
    glutPostRedisplay();
}

void SpecialkeysUp(int key, int x, int y)
{
    if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT)
    {
        isRunning = false;
    }
    // Synchronize state-related flags
    CharacterStateMachine();
    glutPostRedisplay(); 
}

void keyUp(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'a': // move left
            isRunning = false;
            break;
        case 'd': // move right
            isRunning = false;
            break;
        case 'A': // move left
            isRunning = false;
            break;
        case 'D': // move right
            isRunning = false;
            break;
        default:
            break;
    }
    // Synchronize state-related flags
    CharacterStateMachine();
    glutPostRedisplay(); 
}

static void idle(void)
{
    // Use parametric equation with t increment for xpos and y pos
    updateParallax();
    // Transition to IDLE state if no keys are pressed
    static int attackDuration = 1200; // Attack animation duration in milliseconds
    // Mouse update logic
    static int lastUpdateTime = 0; // Tracks the last update time in milliseconds
    int currentTime = glutGet(GLUT_ELAPSED_TIME); // Get the current time in milliseconds
    int deltaTime = currentTime - lastUpdateTime; // Time elapsed since the last update

    // Handle ATTACKING state
    if (playerState == ATTACKING)
    {
        int elapsedTime = glutGet(GLUT_ELAPSED_TIME);

        // Initialize the animation start time if not set
        if (attackAnimationStartTime == -1)
        {
            attackAnimationStartTime = elapsedTime;
        }

        // Check if the attack animation has completed
        if (elapsedTime - attackAnimationStartTime >= attackDuration)
        {
            attackAnimationStartTime = -1; // Reset for the next attack
            playerState = isRunning ? RUNNING : IDLE;
        }
    }
    else if (isMouseHeld && deltaTime > 60) // If the mouse is being held, move the player
    {
        if (playerDirection == RIGHT)
        {
            playerPositionX += 0.05f; // Adjust movement step as needed
        }
        else if (playerDirection == LEFT)
        {
            playerPositionX -= 0.05f; // Adjust movement step as needed
        }
        lastUpdateTime = currentTime; // Update the last update time
        isRunning = true;  // Ensure running state
        playerState = RUNNING;
    }
    else if (isRunning)
    {
        playerState = RUNNING;
    }
    else
    {
        playerState = IDLE;
    }

    // Synchronize state-related flags
    CharacterStateMachine();
    glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
    float scale = 54 * (Wwidth / Wheight); // Adjust scaling based on window dimensions

    switch (btn)
    {
        case GLUT_LEFT_BUTTON: // Left click for movement
            if (state == GLUT_DOWN)
            {
                isMouseHeld = true; // Indicate that the mouse is being held
                float clickedX = (x - (Wwidth / 2)) / scale; // Calculate X-coordinate relative to the center

                // Update direction and position based on the click position
                if (clickedX > 0) // Clicked on the right side
                {
                    isRunning = true;
                    playerState = RUNNING;
                    playerDirection = RIGHT;
                    playerPositionX += 0.05f; // Adjust movement step as needed
                }
                else // Clicked on the left side
                {
                    isRunning = true;
                    playerState = RUNNING;
                    playerDirection = LEFT;
                    playerPositionX -= 0.05f; // Adjust movement step as needed
                }
            }
            else if (state == GLUT_UP) // Stop movement when the button is released
            {
                isRunning = false;
                isMouseHeld = false;
            }
            break;

        case GLUT_RIGHT_BUTTON: // Right click for attacking
            if (state == GLUT_DOWN)
            {
                if (!isAttacking) // Prevent restarting mid-animation
                {
                    isAttacking = true;
                    playerState = ATTACKING;
                    attackAnimationStartTime = -1; // Reset timer for new attack
                }
            }
            break;

        default:
            break;
    }

    glutPostRedisplay(); // Update display after mouse input
}


static void init(void)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.5f, 0.5f, 1.0f, 0.0f);                 // assign a color you like

    glEnable(GL_NORMALIZE); // Normalize normals to unit length
    glEnable(GL_COLOR_MATERIAL); // Use vertex colors as material properties
    glEnable(GL_DEPTH_TEST); // Enable depth testing for correct 3D rendering
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Smooth perspective
    glShadeModel(GL_SMOOTH); // Smooth shading for lighting

    // Set up light properties
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Material properties
    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0); // Enable light 0
    glEnable(GL_LIGHTING); // Enable the lighting system
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    // Texture handling
    glEnable(GL_TEXTURE_2D);    // Enable 2D texture mapping
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D

    // Generate texture IDs for each skybox
    glGenTextures(6, SkyTex);

    skyboxTextureLoaded = TextureLoader(SkyTex[0], "../images/1 close1.png");
    skyboxTextureLoaded = TextureLoader(SkyTex[1], "../images/2 mid1.png");
    skyboxTextureLoaded = TextureLoader(SkyTex[2], "../images/3 far mid1.png");
    skyboxTextureLoaded = TextureLoader(SkyTex[3], "../images/4 far1.png");
    skyboxTextureLoaded = TextureLoader(SkyTex[4], "../images/5 real far1.png");
    skyboxTextureLoaded = TextureLoader(SkyTex[5], "../images/6 sky1.png");

    // Generate texture IDs for each model
    glGenTextures(4, Tex);

    // Load each texture
    objTextureLoaded = TextureLoader(Tex[0] , "../images/Idle-Sheet.png");
    objTextureLoaded = TextureLoader(Tex[1] , "../images/Run-Sheet.png");
    objTextureLoaded = TextureLoader(Tex[2] , "../images/Attack-01-Sheet.png");
    objTextureLoaded = TextureLoader(Tex[3] , "../images/Jump-All-Sheet.png");

    //scale multiplier (set to 1 when not in use)
    sbMulC = 30.0f;  //for skyBox
}

/* Program entry point */
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(0,0);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project Assignment 6");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutKeyboardUpFunc(keyUp);
    glutSpecialUpFunc(SpecialkeysUp);


    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}