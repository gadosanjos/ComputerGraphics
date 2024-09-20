#include <string.h>
#ifdef __APPLE__          // This checks if you're compiling on a macOS system
#include <GLUT/glut.h>    // GLUT for macOS
#else                     // Not on macOS
#include <GL/glut.h>      // GLUT for other platforms
#endif

#include <stdlib.h>

float m = 1.0f;  // Slope of the line (m)
float b = 1.0f;  // Y-intercept (b)

using namespace std;

// The resize() function is a callback function that handles window resizing.
void resize(int width, int height)
{
/* 
    glViewport is called to adjust the size and position of the viewport based on the new window dimensions.
        If the window's width is less than or equal to the height, it adjusts the viewport so that the content is centered vertically.
        If the width is greater than the height, it centers the content horizontally.
 */
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);     // Center horizontally 

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height); // Center vertically
}

// The init() function sets up the projection matrix.
void init()
{
/* 
    glMatrixMode(GL_PROJECTION): Switches the matrix mode to the projection matrix, which controls how objects are projected onto the screen.
    glLoadIdentity(): Resets the current matrix to the identity matrix.
    glOrtho(): Defines an orthographic (2D) projection matrix. It specifies the coordinate system that will be mapped to the screen.
        In this case, the x-axis goes from -2.5 to 2.75, the y-axis from 10.5 to 0, and the z-axis from -1.0 to 1.0. 
        These values set the bounds of the projection for rendering the content. 
*/
    glMatrixMode(GL_PROJECTION);          // Set projection matrix mode
    glLoadIdentity();                     // Reset the matrix
    glOrtho(0,10,0,10.5, -1.0, 1.0); // adjusted for suitable viewport/Define orthographic projection

}

// The display() function is the callback function that renders the scene.
void display(void)
{
/* 
    glClear(GL_COLOR_BUFFER_BIT): Clears the screen (the color buffer) to prepare for drawing a new frame.
    The glFlush() command forces OpenGL to render any pending operations (to ensure the image gets drawn to the screen).
    Note: There's a comment indicating that this is where you would add your drawing code.
*/
    glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
    // your code here

    // Set the color for the point
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_LINE_STRIP);        // Begin drawing the line

    // Iterate over x-values and calculate the corresponding y-values using y = mx + b
    for (float x = -10.0f; x <= 10.0f; x += 0.1f) {
        float y = m * x + b;  // y = mx + b
        glVertex2f(x, y);     // Plot the point (x, y)
    }

    glEnd();                    // End drawing the point
    glFlush ();                           // clear buffer/Ensure all OpenGL commands are executed
    
}

// The key() function handles keyboard input.
void key(unsigned char key, int x, int y)
{
/*  
    It checks if the user presses the ESC key (ASCII code 27) or the 'q' key. If either is pressed, it calls exit(0) to terminate the program.
    glutPostRedisplay(): Requests a redraw of the window after handling the key press. This ensures the screen is updated if necessary.
*/
    switch (key)
    {
        case 27 :     // esc key to exit
        case 'q':     // 'q' key
            exit(0);
            break;
    }

    glutPostRedisplay(); // Redraw the screen
}



/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);                        // Initialize GLUT
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // Set display mode (single buffer, RGB color)
   glutInitWindowSize (600, 800);                // Set the initial window size
   glutInitWindowPosition (0, 0);                // Set the initial window position
   glutCreateWindow ("Program1");                // Create the window with a title
   init();                                       // Call initialization function

   glutDisplayFunc(display);                     // callback function for display
   glutReshapeFunc(resize);                      // callback for reshape
   glutKeyboardFunc(key);                        // callback function for keyboard

   glutMainLoop();                               // Enter the GLUT event processing loop

    return EXIT_SUCCESS;
}