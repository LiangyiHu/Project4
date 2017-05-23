#include "Camera.h"

Camera & Camera::Get() {
	static Camera thisCamera = Camera();
	return thisCamera;
}

Camera::Camera() {
	this->position = glm::vec3();
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f); 
}

void Camera::init(glm::vec3 position, GLuint screenWidth, GLuint screenHeight) {
	this->position = position;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	UpdateVectors();
}

void Camera::ProcessMouseMovement(GLfloat xpos, GLfloat ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	xoffset *= this->mouseSensitivity;
	yoffset *= this->mouseSensitivity;
	this->yaw += xoffset;
	this->pitch += yoffset;

	if (this->pitch > 89.0f) this->pitch = 89.0f;
	if (this->pitch < -89.0f) this->pitch = -89.0f;
	UpdateVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset){
	if (this->zoom >= 1.0f && this->zoom <= 45.0f)
		this->zoom -= yoffset;
	if (this->zoom <= 1.0f)
		this->zoom = 1.0f;
	if (this->zoom >= 45.0f)
		this->zoom = 45.0f;
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime) {
	GLfloat velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
}

void Camera::UpdateVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	front.y = sin(glm::radians(this->pitch));
	front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, this->worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::UpdateCameraMatrix()
{
	cameraMatrix= glm::lookAt(this->position, this->position + this->front, this->up);
	cameraMatrix = glm::perspective(zoom, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f)*cameraMatrix;
}

