#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
//self library include
#include "Labs.h"
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
class Camera
{
public:
	glm::vec3 position, lookAt, worldUp,  front, up, right;
	GLfloat lastX = 400, lastY = 300, yaw = -90, pitch = -30.0f;
	glm::mat4 cameraMatrix;
	
	static Camera & Get();
	void init(glm::vec3 position, GLuint screenWidth, GLuint screenHeight);
	void UpdateCameraMatrix();
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset);
	void ProcessMouseScroll(GLfloat yoffset);
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
private: 
	GLfloat mouseSensitivity = 0.25f;
	GLfloat zoom = 45.0f;
	GLfloat movementSpeed = 30.0f;
	GLuint screenWidth, screenHeight;
	bool firstMouse = true;

	//private constructor for singleton mode
	Camera();
	Camera(const Camera &);
	Camera & operator = (const Camera &);
	//innner calculation functions
	void UpdateVectors();
};