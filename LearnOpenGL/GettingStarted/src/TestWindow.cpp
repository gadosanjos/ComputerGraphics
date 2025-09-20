#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Window dimensions, GLint which is a type built in in OpenGL
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	// glfw: initialize and configure
    // ------------------------------
	// Initialize GLFW so we can make a window with it glfwInit returns the success rate so we can use for errors
	// Set GLFW window properties
	// OpenGL version 3.3
	// Core Profile = No Backwards Compatibility old code that is removed/outdated won't work
	// Allow Forward Compatbility
	if (!glfwInit()) { printf("GLFW FAIL"); glfwTerminate(); return 1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Note that on Mac OS X you need to add
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
    // --------------------
	// OpenGL is by itself a large state machine
	// The state of OpenGL is commonly referred to as the OpenGL context
	/* Whenever we tell OpenGL that we now want to draw lines instead of triangles for example, 
	we change the state of OpenGL by changing some context variable that sets how OpenGL should draw. 
	As soon as we change the context by telling OpenGL it should draw lines, 
	the next drawing commands will now draw lines instead of triangles.

	When working in OpenGL we will come across several state-changing functions that change the context 
	and several state-using functions that perform some operations based on the current state of OpenGL. 
	As long as you keep in mind that OpenGL is basically one large state machine, 
	most of its functionality will make more sense.
	*/
	// Create a window with an OpenGL context
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Context", NULL, NULL);
    if (!mainWindow) { printf("GLFW window creation failed!"); glfwTerminate(); return 1; }

	// Set context for glad to use ie Make the OpenGL context current so it knows where to draw to
	// This way you could switch to drawing to another window if u wanted to
	// this because glfwcreatewindow does not make context current it only creates it
	glfwMakeContextCurrent(mainWindow);
	glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
    // ---------------------------------------
	// Initializing glad libraries, GLAD manages function pointers for OpenGL
	// Load OpenGL functions via GLAD (after context current)
	// We pass GLAD the function to load the address of the OpenGL function pointers which is OS-specific
	// GLFW gives us glfwGetProcAddress that defines the correct function based on which OS we're compiling for.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

	GLfloat colorChange = 0.0;
	// render loop
    // -----------

	// We want to place all the rendering commands in the render loop, 
	// since we want to execute all the rendering commands each iteration or frame of the loop.
	// should close is triggeered by clicking the close button on the window	
	while (!glfwWindowShouldClose(mainWindow))
	{	
		// input
        // -----
        processInput(mainWindow);

		// render
        // ------

		// Clear window
		// sets the color that will be used next time you clear.
		glClearColor(0.0f, 0.0f, colorChange, 1.0f);
		// clears the framebuffer using the color from glClearColor
		/* As you may recall from the OpenGL chapter, 
		the glClearColor function is a state-setting function 
		and glClear is a state-using function in that it uses the current state to retrieve the clearing color from.
		*/
		glClear(GL_COLOR_BUFFER_BIT);
		if(colorChange >= 1.0){
			colorChange = 0.0;
		} else {
			colorChange += 0.01;
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------

		// Most windows use double buffering:
		// One buffer is being drawn to (back buffer).
		// The other is being displayed (front buffer).
		// glfwSwapBuffers swaps them, The back buffer (you just cleared + drew into) becomes visible.
		// A fresh back buffer is given to you for the next frame.
		glfwSwapBuffers(mainWindow);
		// Get + Handle user input events like keyboard mouse resizing or closing the 
		// checks if any events are triggered
		// updates the window state, and calls the corresponding functions (which we can register via callback methods).
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glfwDestroyWindow(mainWindow);
    glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.

	// Setup Viewport size or the size we want to be drawing in our window, here we will drawn to the whole space
	// We have to tell OpenGL the size of the rendering window so OpenGL knows how we want to display the data and coordinates with respect to the window
	// args are left X and left Y coord , width and height
	// The first two parameters of glViewport set the location of the lower left corner of the 
	/* Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed 
	to coordinates on your screen. For example, 
	a processed point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450) in screen coordinates. 
	Note that processed coordinates in OpenGL are between -1 and 1 so we effectively 
	map from the range (-1 to 1) to (0, 800) and (0, 600). */
    glViewport(0, 0, width, height);
}