#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	GLuint Program;
	// Constructor generates the shader on the fly
	Shader(GLchar * vertexPath, GLchar * fragmentPath);
	// Uses the current shader
	void Use();
};