#include "Light.h"

Light & Light::Get() {
	static Light mainLight = Light();
	return mainLight;
}

Light::Light() {
	numberOfLights = 0;
	initialPosition = new glm::vec3[10];
	position = new glm::vec3[10];
	color = new glm::vec3[10];
	shader = new Shader("shaders/light_vShader.txt", "shaders/light_fShader.txt");
	GLfloat vertices[] = {
		-2.0f,  -1.5f, 2.0f, 
		2.0f, -1.5f, 2.0f, 
		2.0f, -1.5f, -2.0f, 
		-2.0f, -2.0f, -2.0f,
		0.0f,  1.5f, 0.0f 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 2, 1,
		0, 3, 2,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
}

void Light::Draw() {
	shader->Use();
	glBindVertexArray(VAO);
	glm::mat4 modelTrans;
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(Camera::Get().cameraMatrix));
	for (int i = 0; i < numberOfLights; i++) {
		modelTrans = glm::translate(glm::mat4(), Get().position[i]);
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTrans));
		glUniform3f(glGetUniformLocation(shader->Program, "lightColor"), Get().color[i].x, Get().color[i].y, Get().color[i].z);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}

void Light::AddLight(glm::vec3 position, glm::vec3 color) {
	if (Get().numberOfLights==10) {
		cout << "number of lights reach maximum value, currently no code to handle the resize of arrays when number of lights goes above 10" << endl;
	}
	Get().initialPosition[Get().numberOfLights] = position;
	Get().position[Get().numberOfLights] = position;
	Get().color[Get().numberOfLights] = color;
	Get().numberOfLights += 1;
}

void Light::Configuration() {

}