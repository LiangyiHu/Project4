#pragma once
// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"  //create and compile shader program
#include "Camera.h"

//SOIL include
#include <SOIL.h>

class Background
{
private:
	GLuint bgVBO, bgVAO, textureBG;
	int width, height, size;
	unsigned char* image;
	Shader * shader;
public:

	Background(GLchar * path) {
		shader = new Shader("shaders/bg_vShader.txt", "shaders/bg_fShader.txt");
		GLfloat background[] = {
			-10.0f,  0.01f, 10.0f,  0.0f,  0.0f,
			10.0f,  0.01f,  10.0f,  1.0f,  0.0f,
			10.0f,   0.01f, -10.0f,  1.0f,  1.0f,
			10.0f,   0.01f, -10.0f,  1.0f,  1.0f,
			-10.0f,  0.01f, -10.0f,  0.0f,  1.0f,
			-10.0f,  0.01f, 10.0f,  0.0f,  0.0f,
		};
		glGenVertexArrays(1, &bgVAO);
		glGenBuffers(1, &bgVBO);
		glBindVertexArray(bgVAO);
		glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(background), background, GL_STATIC_DRAW);
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// TexCoord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0); // Unbind VAO
							  //Import texture and bind it to the background vertices.
		glGenTextures(1, &textureBG);
		glBindTexture(GL_TEXTURE_2D, textureBG);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);//unbind texture.
		shader->Use();
		glUniform1i(glGetUniformLocation(shader->Program, "backgroundTex"), 0);
	}
	~Background() {
		glDeleteVertexArrays(1, &bgVAO);
		glDeleteBuffers(1, &bgVBO);
	}

	void Draw(){
		shader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureBG);
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(Camera::Get().cameraMatrix));
		glBindVertexArray(bgVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0); //unbind
	}
};