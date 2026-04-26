#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);

	void SetColor(GLfloat r, GLfloat g, GLfloat b) { color = glm::vec3(r, g, b); }

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

