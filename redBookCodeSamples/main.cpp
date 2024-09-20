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
#ifdef __APPLE__          // This checks if you're compiling on a macOS system
#include <GLUT/glut.h>    // GLUT for macOS
#else                     // Not on macOS
#include <GL/glut.h>      // GLUT for other platforms
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

float a, b, c, d, e, f, x, y, x0, y_init; //changed y0 to y_init since it caused conflict with standard namespace libraries
int i = 0;
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
    glOrtho(0,2.75,0,10.5, -1.0, 1.0); // adjusted for suitable viewport/Define orthographic projection

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

    // 0 --- 7 --- 15---------99
    // |  \/    \/       \/
    //f1  f3    f4       f2
    //
    // 0 < 85 < 92 < 99
    //   |    |    |    |
    //   f2   f3   f4   f1

    // Start the iteration loop to plot 200,000 points (simulating the fern growth)
    for(int j=0; j < 100000; j++){
        i = rand()%100;
        if(i < 85){ // Apply Transformation 2 for numbers between 0 and 85. (85% of the time)
            a= 0.85;
            b = -0.04;
            c =0.04;
            d =0.85;
            e =0;
            f=1.6;
        }
        else if(i < 90){ // Apply Transformation 3 for numbers between 85 and 90 (5% of the time)
            a= 0.2;
            b =0.23;
            c =-0.26;
            d =0.22;
            e =0;
            f=1.6;
        } 
        else if (i < 99){ // Apply Transformation 4 for numbers between 90 and 99 (9% of the time)
            a= -0.15;
            b =0.26;
            c =0.28;
            d =0.24;
            e =0;
            f=0.44;

        } else { // Apply Transformation 1 for numbers between 99 and 100 (1% of the time)
            a= 0;
            b =0;
            c =0;
            d =0.16;
            e =0;
            f=0;
        }

        x = a*x0 + c*y_init + e;    // Calculate new x-coordinate
        y = b*x0 + d*y_init + f;    // Calculate new y-coordinate

        // Set the color for the point
        glColor3f(0.0f,1.0f,0.0f);

        // Plot the point at the new (x, y) position
        glBegin(GL_POINTS);         // Begin drawing a point
        glVertex3f(x,y,0);          // Specify the point's 3D coordinates (z=0 since it's 2D)
        glEnd();                    // End drawing the point

        // Update the point (x0, y_init) to the new (x, y) for the next iteration
        x0 = x;
        y_init = y;
    }
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