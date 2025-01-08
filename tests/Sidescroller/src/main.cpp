#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Player.h"

#include <SOIL.h>

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow(800, 600);
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "../Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "../Shaders/shader.frag";

// Texture Handling
GLuint Tex[4], SkyTex[6];
int TexIndex = 0;
bool objTextureLoaded = false;
bool skyboxTextureLoaded = false;

// Player States
Player player;

bool TextureLoader(GLuint textureID, const char* fileName)
{
    unsigned char* image;              // Place your file name
    int width, height;                 // width & height for the image reader
	width = 0;
	height = 0;

    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

    if(!image)
	{
        std::cout << "SOIL loading error: " << SOIL_last_result() << "' (" << fileName << ")" << std::endl;
        return false;  // Return false if loading fails
    }

	glBindTexture(GL_TEXTURE_2D, textureID); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.

    // Set texture parameters for wrapping and filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

    SOIL_free_image_data(image);

    return true;  // Return true if loading is successful
}

void GenTextures() 
{
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
}

void CreateObjects() 
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//  X      Y     Z	   U	 V
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.5f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.5f, 1.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

		unsigned int indices2[] = {
		0, 1, 2,
		0, 2, 3
	};

	GLfloat vertices2[] = {
		//  X      Y     Z	   U	 V
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,		// 0: Bottom-left
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  	// 1: Bottom-right
    	 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 	// 2: Top-right
    	-1.0f,  1.0f, 0.0f, 0.0f, 0.0f  	// 3: Top-left
	};

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices2, indices2, 20, 6);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() 
{
	mainWindow.Initialise();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GenTextures();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformcurrentFrame = 0, uniformnumFrames = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		player.HandleKeyboard(mainWindow.getsKeys());
		player.Update(deltaTime);
		

		// Clear the window
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformcurrentFrame = shaderList[0].GetCurrFrameLocation();
		uniformnumFrames = shaderList[0].GetNumFramesLocation();

		glm::mat4 model(1.0f);	

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, SkyTex[5]);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		glm::vec3 playerPosition = player.GetPosition();
		model = glm::rotate(model, -0.785398f, glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::translate(model, playerPosition + glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		// figure out the currentFrame, numFrames from player
        int currentFrame = player.GetCurrentFrame();
        int numFrames    = player.GetNumFrames();

		glUniform1f(uniformcurrentFrame, (GLfloat)currentFrame);
        glUniform1f(uniformnumFrames,    (GLfloat)numFrames);

		bool flipHorizontally = (player.GetDirection() == Direction::LEFT);
		glUniform1i(glGetUniformLocation(shaderList[0].GetShaderID(), "flipHorizontally"), flipHorizontally);

		switch (player.GetState())
		{
			case PlayerState::IDLE:
                glBindTexture(GL_TEXTURE_2D, Tex[0]);
                break;
            case PlayerState::RUNNING:
                glBindTexture(GL_TEXTURE_2D, Tex[1]);
                break;
            case PlayerState::ATTACKING:
                glBindTexture(GL_TEXTURE_2D, Tex[2]);
				break;
			default:
				break;
		}
		meshList[1]->RenderMesh();

		mainWindow.swapBuffers();
	}

	return 0;
}