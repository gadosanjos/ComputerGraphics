#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f); // amount of the color it will be shown in a texture 1 towards showing more, a red brick if we set r = 0 it would be pitch black
	ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity)
{
	colour = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;
}

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation)
{
	glUniform3f(ambientColourLocation, colour.r, colour.g, colour.b); // could also use colour.x, colour.y, colour.z
	glUniform1f(ambientIntensityLocation, ambientIntensity);
}

Light::~Light()
{
}
