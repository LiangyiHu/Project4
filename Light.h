#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Shader.h"
#include "Camera.h"
using namespace std;

class Light {
public:
	int numberOfLights;
	glm::vec3 * initialPosition;
	glm::vec3 * position;
	glm::vec3 * color;

	//singleton function declaration
	static Light & Get();
	static void AddLight(glm::vec3 position, glm::vec3 color);
	void Configuration();
	void Draw();
private:
	Shader * shader;
	GLuint VBO, VAO, EBO;

	Light();
	Light(const Light &);
	Light & operator = (const Light &);
};
