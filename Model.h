#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include "Labs.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"

using namespace std;
enum Shading {
	gouraud,
	phong,
};

class Model {
public:
	//common states
	glm::vec3 position, rotationAxle;
	GLfloat rotationDegree;
	bool texCoordIncluded = false;
	int stride;
	//geo properties
	int numberOfPoints, numberOfPolygons;
	GLfloat * vertices;
	GLuint * polygons;
	enum Shading shadingMethod;

	Model(GLchar * path, GLchar * texPath, bool texCoordsIncluded, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat rotationDegree = 0.0f, GLfloat shininess=0.5, enum Shading inputShadingMethod=phong);
	void Draw();
	~Model();
	void UpdateModelTrans();

	//some variables
	ifstream dataFile;
	glm::mat4 modelTrans;
	Shader * shader;
	GLuint texture;
	GLfloat shininess;
	GLchar * texPath;
	//graphics properties
	GLuint VBO, VAO, EBO;

	//internal function declaration
	void SetupGL();
	void SetupTexture();
	void CalculateNormalByVertex();
};

class ConstantModel {
public:
	//common states
	glm::vec3 position, rotationAxle;
	GLfloat rotationDegree;
	bool texCoordIncluded = false;
	int stride;
	//geo properties
	int numberOfPoints, numberOfPolygons;
	GLfloat * vertices;
	GLuint * polygons;
	GLfloat * finalPassedVertices;

	ConstantModel(GLchar * path, GLchar * texPath, bool texCoordsIncluded, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), GLfloat rotationDegree = 0.0f, GLfloat shininess = 0.5);
	void Draw();
	~ConstantModel();
	void UpdateModelTrans();

	//some variables
	ifstream dataFile;
	glm::mat4 modelTrans;
	Shader * shader;
	GLuint texture;
	GLfloat shininess;
	GLchar * texPath;
	//graphics properties
	GLuint VBO, VAO;

	//internal function declaration
	void SetupGL();
	void SetupTexture();
	void CalculateNormalByPolygon();
};