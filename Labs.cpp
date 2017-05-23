#include "Labs.h"

void printThisMatrix(GLfloat **matrixInput, int m, int n) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << matrixInput[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void printThisMatrix(GLfloat * matrixInput, int m, int n) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << matrixInput[i*n + j] << " ";
		}
		std::cout << std::endl;
	}
}

void printThisMatrix(glm::mat4 printeeMatrix) {
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			std::cout << printeeMatrix[i][j] << ", ";
		}
		std::cout << std::endl;
	}
}

void printThisVector(glm::vec3 inputVec) {
	std::cout << inputVec.x << "  " << inputVec.y << "  " << inputVec.z << "  " << std::endl;
}

void printThisVector(glm::vec4 inputVec) {
	std::cout << inputVec.x << "  " << inputVec.y << "  " << inputVec.z << "  " << inputVec.w << std::endl;
}

void printThisVector(GLfloat * vectorInput, int m) {
	for (int i = 0; i < m; i++) {
		std::cout << vectorInput[i] << " ";
	}
	std::cout << std::endl;
}

glm::vec3 getPointByIndex(GLfloat * vertices, std::vector<std::vector<GLint> >&polygons, int polygonIndex, int pointIndex) {
	return glm::vec3(vertices[(polygons[polygonIndex][pointIndex] - 1) * 3], vertices[(polygons[polygonIndex][pointIndex] - 1) * 3 + 1], vertices[(polygons[polygonIndex][pointIndex] - 1) * 3 + 2]);
}

glm::vec3 vecCross(glm::vec3 v1, glm::vec3 v2) {
	glm::vec3 returnVector;
	returnVector[0] = v1[1] * v2[2] - v1[2] * v2[1];
	returnVector[1] = v1[2] * v2[0] - v1[0] * v2[2];
	returnVector[2] = v1[0] * v2[1] - v1[1] * v2[0];
	return returnVector;
}

GLfloat vecDot(glm::vec3 v1, glm::vec3 v2) {
	return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

glm::mat4 mulMatrix(glm::mat4 matrix1, glm::mat4 matrix2) {
	glm::mat4 returnMatrix;
	for (int i = 0; i <= 3; i++) {
		for (int j = 0; j <= 3; j++) {
			returnMatrix[i][j] = matrix1[i][0] * matrix2[0][j] + matrix1[i][1] * matrix2[1][j] + matrix1[i][2] * matrix2[2][j] + matrix1[i][3] * matrix2[3][j];
		}
	}
	return returnMatrix;
}

glm::vec4 mulMatrix(glm::mat4 matrix1, glm::vec4 vector1) {
	glm::vec4 returnVector;
	for (int i = 0; i <= 3; i++) {
		returnVector[i] = matrix1[i][0] * vector1[0] + matrix1[i][1] * vector1[1] + matrix1[i][2] * vector1[2] + matrix1[i][3] * vector1[3];
	}
	return returnVector;
}
