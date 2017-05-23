#include "Model.h"

//below is implementation for gouraud and phong shading model
Model::Model(GLchar * path, GLchar * texPath, bool texCoordsIncluded, glm::vec3 position, GLfloat rotationDegree, GLfloat shininess, enum Shading shadingMethod) {
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->rotationAxle = glm::vec3(0.0f, 1.0f, 0.0f);
	this->shininess = shininess;
	this->texPath = texPath;
	this->shadingMethod = shadingMethod;
	stride = 8;
	string tmpStr;
	GLint tmpInt;
	stringstream oneLine;
	dataFile.open(path, ios::in);
	if (!dataFile.is_open()) {
		cout << "error, data file is not open!" << endl;
	}
	getline(dataFile, tmpStr);
	oneLine.str(tmpStr);
	oneLine >> tmpStr >> numberOfPoints >> numberOfPolygons;
	//cout << "number of points: " << numberOfPoints << ".\n" << "number of polygons: " << numberOfPolygons << endl;
	vertices = new GLfloat[numberOfPoints * stride];
	polygons = new GLuint[numberOfPolygons * 3];
	//load point coordinates
	for (int i = 0; i < numberOfPoints; i++) {
		oneLine.clear();
		getline(dataFile, tmpStr);
		oneLine.str(tmpStr);
		oneLine >> vertices[i * stride] >> vertices[i * stride + 1] >> vertices[i * stride + 2];
	}
	//load polygons' points indices
	for (int i = 0; i < numberOfPolygons; i++) {
		oneLine.clear();
		getline(dataFile, tmpStr);
		oneLine.str(tmpStr);
		oneLine >> tmpInt >> polygons[i * 3] >> polygons[i * 3 + 1] >> polygons[i * 3 + 2];
	}
	//load tex coordinates
	if (texCoordsIncluded) {
		for (int i = 0; i < numberOfPoints; i++) {
			oneLine.clear();
			getline(dataFile, tmpStr);
			oneLine.str(tmpStr);
			oneLine >> tmpInt >> vertices[i * stride + 6] >> vertices[i * stride + 7];
		}
	}
	else {
		for (int i = 0; i < numberOfPoints; i++) {
			vertices[i * stride + 6] = (GLfloat)((rand() % 100000)) / 100000;
			vertices[i * stride + 7] = (GLfloat)((rand() % 100000)) / 100000;
		}
	}
	CalculateNormalByVertex();
	//setup shader
	if (shadingMethod == phong) {
		shader = new Shader("shaders/model_phong_vShader.txt", "shaders/model_phong_fShader.txt");
	}
	else if (shadingMethod == gouraud) {
		shader = new Shader("shaders/model_gouraud_vShader.txt", "shaders/model_gouraud_fShader.txt");
	}
	SetupTexture();
	SetupGL();
}
void Model::UpdateModelTrans() {
	modelTrans = glm::translate(glm::mat4(), this->position);
	modelTrans = glm::rotate(modelTrans, glm::radians(rotationDegree), rotationAxle);
}
void Model::SetupGL() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	//buffer vertices data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numberOfPoints*stride, &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numberOfPolygons * 3, &polygons[0], GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Tex coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO
	//write shininess to uniform
	shader->Use();
	glUniform1f(glGetUniformLocation(shader->Program, "shininess"), this->shininess);
}
void Model::SetupTexture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(texPath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	/* for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			cout << (int)image[j*width + i] << " ";
		}
		cout << endl;
	}*/
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Model::CalculateNormalByVertex() {
	glm::vec3 * normalByVertex = new glm::vec3[numberOfPoints];
	int * numPolygonsForEachPoint = new int[numberOfPoints];
	for (int i = 0; i < numberOfPoints; i++) {
		numPolygonsForEachPoint[i] = 0;
	}
	glm::vec3 firstPoint, secondPoint, thirdPoint, firstVector, secondVector, currentNormal;
	for (int i = 0; i < numberOfPolygons; i++) {
		firstPoint = glm::vec3(vertices[polygons[i * 3 + 0]* stride + 0], vertices[polygons[i * 3 + 0]* stride + 1], vertices[polygons[i * 3 + 0]* stride + 2]);
		secondPoint = glm::vec3(vertices[polygons[i * 3 + 1] * stride + 0], vertices[polygons[i * 3 + 1] * stride + 1], vertices[polygons[i * 3 + 1]* stride + 2]);
		thirdPoint = glm::vec3(vertices[polygons[i * 3 + 2] * stride + 0], vertices[polygons[i * 3 + 2] * stride + 1], vertices[polygons[i * 3 + 2]* stride + 2]);
		firstVector = secondPoint-firstPoint;
		secondVector = thirdPoint-secondPoint;
		currentNormal = glm::normalize(vecCross(firstVector,secondVector));
		/*printThisVector(firstPoint);
		printThisVector(secondPoint);
		printThisVector(thirdPoint);
		printThisVector(firstVector);
		printThisVector(secondVector);
		printThisVector(currentNormal);*/
		//get number of points this polygon has used
		for (int pointIndex = 0; pointIndex < 3; pointIndex++) {
			int currentPoint = polygons[i * 3 + pointIndex];
			normalByVertex[currentPoint] = normalByVertex[currentPoint] + currentNormal;
			numPolygonsForEachPoint[currentPoint]++;
		}
	}
	for (int i = 0; i < numberOfPoints; i++) {
		normalByVertex[i] = glm::normalize(normalByVertex[i] / ((GLfloat)numPolygonsForEachPoint[i]));
		vertices[i * stride + 3] = normalByVertex[i].x;
		vertices[i * stride + 4] = normalByVertex[i].y;
		vertices[i * stride + 5] = normalByVertex[i].z;
	}
	delete [] normalByVertex;
	delete [] numPolygonsForEachPoint;
}
void Model::Draw() {
	shader->Use();
	glBindTexture(GL_TEXTURE_2D, texture);
	UpdateModelTrans();
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(Camera::Get().cameraMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTrans));
	glUniform3f(glGetUniformLocation(shader->Program, "viewPos"), Camera::Get().position.x, Camera::Get().position.y, Camera::Get().position.z);
	glUniform1i(glGetUniformLocation(shader->Program, "numberOfLights"), Light::Get().numberOfLights);
	glUniform3fv(glGetUniformLocation(shader->Program, "lightColor"), Light::Get().numberOfLights, glm::value_ptr(Light::Get().color[0]));
	glUniform3fv(glGetUniformLocation(shader->Program, "lightPos"), Light::Get().numberOfLights, glm::value_ptr(Light::Get().position[0]));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numberOfPolygons * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
Model::~Model() {
	delete [] vertices;
	delete [] polygons;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

//below is the implementation for constant shading model
ConstantModel::ConstantModel(GLchar * path, GLchar * texPath, bool texCoordsIncluded, glm::vec3 position, GLfloat rotationDegree, GLfloat shininess) {
	this->position = position;
	this->rotationDegree = rotationDegree;
	this->rotationAxle = glm::vec3(0.0f, 1.0f, 0.0f);
	this->shininess = shininess;
	this->texPath = texPath;
	stride = 8;
	string tmpStr;
	GLint tmpInt;
	stringstream oneLine;
	dataFile.open(path, ios::in);
	if (!dataFile.is_open()) {
		cout << "error, data file is not open!" << endl;
	}
	getline(dataFile, tmpStr);
	oneLine.str(tmpStr);
	oneLine >> tmpStr >> numberOfPoints >> numberOfPolygons;
	//cout << "number of points: " << numberOfPoints << ".\n" << "number of polygons: " << numberOfPolygons << endl;
	vertices = new GLfloat[numberOfPoints * 5];
	polygons = new GLuint[numberOfPolygons * 3];
	//load point coordinates
	for (int i = 0; i < numberOfPoints; i++) {
		oneLine.clear();
		getline(dataFile, tmpStr);
		oneLine.str(tmpStr);
		oneLine >> vertices[i * 5] >> vertices[i * 5 + 1] >> vertices[i * 5 + 2];
	}
	//load polygons' points indices
	for (int i = 0; i < numberOfPolygons; i++) {
		oneLine.clear();
		getline(dataFile, tmpStr);
		oneLine.str(tmpStr);
		oneLine >> tmpInt >> polygons[i * 3] >> polygons[i * 3 + 1] >> polygons[i * 3 + 2];
	}
	//load tex coordinates
	if (texCoordsIncluded) {
		for (int i = 0; i < numberOfPoints; i++) {
			oneLine.clear();
			getline(dataFile, tmpStr);
			oneLine.str(tmpStr);
			oneLine >> tmpInt >> vertices[i * 5 + 3] >> vertices[i * 5 + 4];
		}
	}
	else {
		for (int i = 0; i < numberOfPoints; i++) {
			vertices[i * 5 + 3] = (GLfloat)((rand() % 100000)) / 100000;
			vertices[i * 5 + 4] = (GLfloat)((rand() % 100000)) / 100000;
		}
	}
	CalculateNormalByPolygon();
	//setup shader
	shader = new Shader("shaders/model_constant_vShader.txt", "shaders/model_constant_fShader.txt");
	SetupTexture();
	SetupGL();
}

void ConstantModel::CalculateNormalByPolygon() {
	finalPassedVertices = new GLfloat[numberOfPolygons*3*stride];
	glm::vec3 normalByPolygon,points[3], firstVector, secondVector;
	for (int i = 0; i < numberOfPolygons; i++) {
		points[0] = glm::vec3(vertices[polygons[i * 3 + 0] * 5 + 0], vertices[polygons[i * 3 + 0] * 5 + 1], vertices[polygons[i * 3 + 0] * 5 + 2]);
		points[1] = glm::vec3(vertices[polygons[i * 3 + 1] * 5 + 0], vertices[polygons[i * 3 + 1] * 5 + 1], vertices[polygons[i * 3 + 1] * 5 + 2]);
		points[2] = glm::vec3(vertices[polygons[i * 3 + 2] * 5 + 0], vertices[polygons[i * 3 + 2] * 5 + 1], vertices[polygons[i * 3 + 2] * 5 + 2]);
		firstVector = points[1] - points[0];
		secondVector = points[2] - points[1];
		normalByPolygon = glm::normalize(vecCross(firstVector, secondVector));
		for (int pointIndex = 0; pointIndex < 3; pointIndex++) {
			//move point xyz
			finalPassedVertices[(i * 3 + pointIndex) * stride + 0] = points[pointIndex].x;
			finalPassedVertices[(i * 3 + pointIndex) * stride + 1] = points[pointIndex].y;
			finalPassedVertices[(i * 3 + pointIndex) * stride + 2] = points[pointIndex].z;
			//move point normal
			finalPassedVertices[(i * 3 + pointIndex) * stride + 3] = normalByPolygon.x;
			finalPassedVertices[(i * 3 + pointIndex) * stride + 4] = normalByPolygon.y;
			finalPassedVertices[(i * 3 + pointIndex) * stride + 5] = normalByPolygon.z;
			//move point tex coordinates
			finalPassedVertices[(i * 3 + pointIndex) * stride + 6] = vertices[polygons[i * 3 + pointIndex] * 5 + 3];
			finalPassedVertices[(i * 3 + pointIndex) * stride + 7] = vertices[polygons[i * 3 + pointIndex] * 5 + 4];
		}
	}
}

void ConstantModel::SetupTexture() {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(texPath, &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	/* for (int i = 0; i < width; i++) {
	for (int j = 0; j < width; j++) {
	cout << (int)image[j*width + i] << " ";
	}
	cout << endl;
	}*/
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ConstantModel::SetupGL() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	//buffer vertices data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*numberOfPolygons*3*stride, &finalPassedVertices[0], GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Tex coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO
						  //write shininess to uniform
	shader->Use();
	glUniform1f(glGetUniformLocation(shader->Program, "shininess"), this->shininess);
}

void ConstantModel::UpdateModelTrans() {
	modelTrans = glm::translate(glm::mat4(), this->position);
	modelTrans = glm::rotate(modelTrans, glm::radians(rotationDegree), rotationAxle);
}

ConstantModel::~ConstantModel() {
	delete[] vertices;
	delete[] polygons;
	delete[] finalPassedVertices;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void ConstantModel::Draw() {
	shader->Use();
	glBindTexture(GL_TEXTURE_2D, texture);
	UpdateModelTrans();
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(Camera::Get().cameraMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTrans));
	glUniform3f(glGetUniformLocation(shader->Program, "viewPos"), Camera::Get().position.x, Camera::Get().position.y, Camera::Get().position.z);
	glUniform1i(glGetUniformLocation(shader->Program, "numberOfLights"), Light::Get().numberOfLights);
	glUniform3fv(glGetUniformLocation(shader->Program, "lightColor"), Light::Get().numberOfLights, glm::value_ptr(Light::Get().color[0]));
	glUniform3fv(glGetUniformLocation(shader->Program, "lightPos"), Light::Get().numberOfLights, glm::value_ptr(Light::Get().position[0]));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, numberOfPolygons*3);
	glBindVertexArray(0);
}