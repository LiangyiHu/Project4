#pragma once
//Standard include
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

// GL Includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void printThisMatrix(GLfloat **matrixInput, int m, int n);

void printThisMatrix(GLfloat * matrixInput, int m, int n);

void printThisMatrix(glm::mat4 printeeMatrix);

void printThisVector(glm::vec3 inputVec);

void printThisVector(glm::vec4 inputVec);

void printThisVector(GLfloat * vectorInput, int m);

glm::vec3 getPointByIndex(GLfloat * vertices, std::vector<std::vector<GLint> >&polygons, int polygonIndex, int pointIndex);

glm::vec3 vecCross(glm::vec3 v1, glm::vec3 v2);

GLfloat vecDot(glm::vec3 v1, glm::vec3 v2);

glm::mat4 mulMatrix(glm::mat4 matrix1, glm::mat4 matrix2);

glm::vec4 mulMatrix(glm::mat4 matrix1, glm::vec4 vector1);
