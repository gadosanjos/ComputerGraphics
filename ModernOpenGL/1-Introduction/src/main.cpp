#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions, GLint which is a type built in in OpenGL
const GLint WIDTH = 800, HEIGHT = 600;

int main() {

	// Initialize GLFW so we can make a window with it glfwInit returns the success rate so we can use for errors
	if (!glfwInit()) {
		printf("GLFW FAIL");
		glfwTerminate();
		return 1;
	}
	// Set GLFW window properties
	// OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core Profile = No Backwards Compatibility old code that is removed/outdated won't work
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create a window with an OpenGL context
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Context", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer Size information
	// This call ask for the window I just created (mainWindow), 
	// what’s the actual size of the framebuffer that OpenGL will be drawing into?
	// Window size → the logical size you request, e.g. 800x600.
	// Window size = the “paper size” you asked for.
	// Framebuffer size → the actual size of the pixel buffer that OpenGL will render to.
	// Framebuffer size = the “actual number of dots the printer can print”.
	int bufferWidth, bufferHeight;
	// Function to grab those values	
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use ie Make the OpenGL context current so it knows where to draw to
	// This way you could switch to drawing to another window if u wanted to
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	// Initializing glew libraries
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup Viewport size or the size we want to be drawing in our window, here we will drawn to the whole space
	// args are X and Y, width and height
	glViewport(0, 0, bufferWidth, bufferHeight);

	GLfloat colorChange = 0.0;
	// Loop until window closed
	// should close is triggeered by clicking the close button on the window	
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get + Handle user input events like keyboard mouse resizing or closing the window
		glfwPollEvents();
		
		// Clear window
		// 	sets the color that will be used next time you clear.
		glClearColor(0.0f, colorChange, 0.0f, 1.0f);
		// clears the framebuffer using the color from glClearColor
		glClear(GL_COLOR_BUFFER_BIT);
		if(colorChange >= 1.0){
			colorChange = 0.0;
		} else {
			colorChange += 0.01;
		}
		// Most windows use double buffering:
		// One buffer is being drawn to (back buffer).
		// The other is being displayed (front buffer).
		// glfwSwapBuffers swaps them, The back buffer (you just cleared + drew into) becomes visible.
		// A fresh back buffer is given to you for the next frame.
		glfwSwapBuffers(mainWindow);
	}

	return 0;
}