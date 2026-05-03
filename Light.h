#pragma once

#include <glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);
	void SetColor(GLfloat red, GLfloat green, GLfloat blue);
	void SetDiffuseIntensity(GLfloat dIntensity) { diffuseIntensity = dIntensity; }
	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

