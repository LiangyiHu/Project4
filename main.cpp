//standard include
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

//OpenGL include
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//other include
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//own function group
#include "Labs.h"
#include "Camera.h"
#include "Shader.h"
#include "Background.h"
#include "Model.h"
#include "Light.h"

//screen setting
const GLuint screenWidth = 800, screenHeight = 600;

//camera setting
glm::vec3 cameraPosition = glm::vec3(0.0f, 10.0f, 20.0f);

//video & control setting
GLfloat framesPerSec = 60.0f;
bool keys[1024] = { false };
bool gameStart = false;
bool initialized = false;
GLfloat gameStartTime;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat currentFrame = 0.0f;
GLfloat lastUpdate = 0.0f;
GLfloat updateInterval = 1 / framesPerSec;
//function declaration
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void Do_Movement();
GLFWwindow* GetEverythingInitialized();

using namespace std;
int main() {
	//initialize everything
	GLFWwindow* window = GetEverythingInitialized();

	//create one light
	Light::AddLight(glm::vec3(8.0, 10.5, 5.0), glm::vec3(1.0, 1.0, 1.0));
	//Light::AddLight(glm::vec3(-5.0, 8.5, -5.0), glm::vec3(1.0, 1.0, 1.0));
	//Light::AddLight(glm::vec3(0.0, 8.5, 0.0), glm::vec3(0.0, 0.0, 1.0));
	// glm::vec3(7.5,1.0,7.5
	//build background
	//Background background("data/background.png");
	//background.shader = &backgroundShader;
	//create models
	Model ground("data/ground.d2", "data/grassland.png", true, glm::vec3(0.0f, 0.0f, 0.0f), 0.0f,0.0);
	Model * walls[4];
	walls[0] = new Model("data/wall.d2", "data/brickpic.png", true, glm::vec3(0.0f, 0.0f, -40.0f), 0.0f);
	walls[1] = new Model("data/wall.d2", "data/brickpic.png", true, glm::vec3(0.0f, 0.0f, 40.0f), 0.0f);
	walls[2] = new Model("data/wall.d2", "data/brickpic.png", true, glm::vec3(40.0f, 0.0f, 0.0f), 90.0f);
	walls[3] = new Model("data/wall.d2", "data/brickpic.png", true, glm::vec3(-40.0f, 0.0f, 0.0f), -90.0f);

	Model * cows[2];
	cows[0] = new Model("data/house.d", "data/solidcolor.png", false, glm::vec3(0.0f, 3.8f, 0.0f), -90.0f, 0.5,gouraud);
	cows[1] = new Model("data/house.d", "data/solidcolor.png", false, glm::vec3(8.0f, 3.8f, 0.0f), -90.0f,0.5, phong);
	ConstantModel cow3("data/house.d", "data/solidcolor.png", false, glm::vec3(-8.0f, 3.8f, 0.0f), -90.0f);
	//cows[2] = new Model("data/cow.d", "data/solidcolor.png", false, glm::vec3(8.0f, 3.8f, 0.0f), -90.0f, phong);
	//Model cow2("cow.d", "vShader.txt", "fShader.txt",glm::vec3(10.0f, 3.6f, 0.0f), -50.0f);
	//ModelTex cow3("duck.d2", "duck_upsidedown.jpg","tex_vShader.txt", "tex_fShader.txt",glm::vec3(-10.0f, -3.6f, 0.0f), 0.0f);

	//after all models built, set up the drawer, create the image
	std::cout << "Press B key to Begin!" << std::endl;
	//start generating images
	do {
		// Set frame time for camera control
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events for user input control
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer and build background
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update cameraMatrix for all
		Camera::Get().UpdateCameraMatrix();

		//Draw background picture
		//background.Draw();

		//Check if program is still going on.
		if (gameStart) {
			if (!initialized) {
				initialized = true;
				lastUpdate = glfwGetTime();
				gameStartTime= glfwGetTime();
				/* initialization code comes here */
				cout << "game initialized." << endl;
			}
			//Check if a new frame should be calculated.
			if ((glfwGetTime() - lastUpdate) >= updateInterval) {
				lastUpdate = glfwGetTime();
				Light::Get().position[0] = Light::Get().initialPosition[0] + glm::vec3(20 * glm::cos((glfwGetTime() - gameStartTime)/2), 0.0f, 15 * glm::sin((glfwGetTime() - gameStartTime)/2));
				//Light::Get().position[1] = Light::Get().initialPosition[1] + glm::vec3(0.0f, 0.0f, 30 * glm::sin((glfwGetTime() - gameStartTime)*0.7));
				//Light::Get().position[2] = Light::Get().initialPosition[2] + glm::vec3(30 * glm::sin((glfwGetTime() - gameStartTime)*1.3), 0.0f, 0.0f);
			}
		}
		ground.Draw();
		for (int i = 0; i < 4; i++) walls[i]->Draw();
		for (int i = 0; i < 2; i++) cows[i]->Draw();
		cow3.Draw();
		Light::Get().Draw();
		// Swap the buffers
		glfwSwapBuffers(window);

		//Until user press escape or close the window, the program should loop.
	} while (!glfwWindowShouldClose(window));
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//checking before game start
	if (gameStart == false) {
		//If B pressed, game started.
		if (key == GLFW_KEY_B) {
			gameStart = true;
		}
	}
	//update key pressing
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Camera::Get().ProcessMouseMovement(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::Get().ProcessMouseScroll(yoffset);
}

void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		Camera::Get().ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		Camera::Get().ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		Camera::Get().ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		Camera::Get().ProcessKeyboard(RIGHT, deltaTime);
}

GLFWwindow* GetEverythingInitialized() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// Options, disable mouse cursor.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();
	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);
	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	//initialize camera
	Camera::Get().init(cameraPosition,screenWidth,screenHeight);
	return window;
}