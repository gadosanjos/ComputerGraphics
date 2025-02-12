#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

	~Light();

private:
	glm::vec3 colour;
	GLfloat ambientIntensity;
};

