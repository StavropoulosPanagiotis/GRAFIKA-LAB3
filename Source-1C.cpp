//********************************
//Αυτό το αρχείο θα το χρησιμοποιήσετε
// για να υλοποιήσετε την άσκηση 1B της OpenGL
//
//ΑΜ:                         Όνομα:
//ΑΜ:                         Όνομα:

//*********************************

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace glm;
using namespace std;


#define RECT_VERTICES_NUM 6 // How many vertices are in each rectangle
#define SIDE_NUM 6 // How many sides we have in a single cube
#define COORDS_NUM 3 // The number of coordinates (x, y, z)

static GLfloat maze_walls_vertices[55 * RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM]; // The array that will hold the coordinates for each vertex in the maze (55 is total number of cubes needed)
static GLfloat maze_color[55 * RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM]; // The array that will hold the color values of each vertex of the maze

static int maze[10][10] = { // The layout of the maze, 1 represents a "wall" and 0 is a "free path"
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{0,0,1,1,1,1,0,1,0,1},
	{1,0,1,0,0,0,0,1,0,1},
	{1,0,1,0,1,1,0,1,0,1},
	{1,0,0,0,0,1,0,0,0,1},
	{1,0,1,1,0,1,1,1,0,1},
	{1,0,0,0,0,0,0,1,0,0},
	{1,0,1,0,1,1,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};

static GLfloat player_color[RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM]; // The array that will hold the color values of each vertex of the player (a single cube)
static GLfloat player[] = { // The coordinates of each vertex of each side of the player (a single cube)
	/* Bottom Side */
	-4.75f, 2.25f, 0.25f,
	-4.75f, 2.75f, 0.25f,
	-4.25f, 2.25f, 0.25f,
	-4.75f, 2.75f, 0.25f,
	-4.25f, 2.25f, 0.25f,
	-4.25f, 2.75f, 0.25f,

	/* Top Side */
	-4.75f, 2.25f, 0.75f,
	-4.75f, 2.75f, 0.75f,
	-4.25f, 2.25f, 0.75f,
	-4.75f, 2.75f, 0.75f,
	-4.25f, 2.25f, 0.75f,
	-4.25f, 2.75f, 0.75f,

	/* Front Side */
	-4.75f, 2.25f, 0.25f,
	-4.75f, 2.25f, 0.75f,
	-4.25f, 2.25f, 0.25f,
	-4.25f, 2.25f, 0.25f,
	-4.25f, 2.25f, 0.75f,
	-4.75f, 2.25f, 0.75f,

	/* Back Side */
	-4.75f, 2.75f, 0.25f,
	-4.75f, 2.75f, 0.75f,
	-4.25f, 2.75f, 0.25f,
	-4.25f, 2.75f, 0.25f,
	-4.25f, 2.75f, 0.75f,
	-4.75f, 2.75f, 0.75f,

	/* Left Side */
	-4.75f, 2.25f, 0.25f,
	-4.75f, 2.75f, 0.25f,
	-4.75f, 2.75f, 0.75f,
	-4.75f, 2.75f, 0.75f,
	-4.75f, 2.25f, 0.75f,
	-4.75f, 2.25f, 0.25f,

	/* Right Side */
	-4.25f, 2.25f, 0.25f,
	-4.25f, 2.75f, 0.25f,
	-4.25f, 2.75f, 0.75f,
	-4.25f, 2.75f, 0.75f,
	-4.25f, 2.25f, 0.75f,
	-4.25f, 2.25f, 0.25f
};

static int player_pos[2] = { 2, 0 }; // The initial position of the player in the maze[10][10] array

// ---------- FOR THE TREASURE ----------
// Initial treasure position coordinates(the treasure is a cube with 0.8 length on each side)
static GLfloat treasure[] = {
	/* Bottom Side */
	-4.9f, 2.1f, 0.1f,
	-4.9f, 2.9f, 0.1f,
	-4.1f, 2.1f, 0.1f,
	-4.9f, 2.9f, 0.1f,
	-4.1f, 2.1f, 0.1f,
	-4.1f, 2.9f, 0.1f,

	/* Top Side */
	-4.9f, 2.1f, 0.9f,
	-4.9f, 2.9f, 0.9f,
	-4.1f, 2.1f, 0.9f,
	-4.9f, 2.9f, 0.9f,
	-4.1f, 2.1f, 0.9f,
	-4.1f, 2.9f, 0.9f,

	/* Front Side */
	-4.9f, 2.1f, 0.1f,
	-4.9f, 2.1f, 0.9f,
	-4.1f, 2.1f, 0.1f,
	-4.1f, 2.1f, 0.1f,
	-4.1f, 2.1f, 0.9f,
	-4.9f, 2.1f, 0.9f,

	/* Back Side */
	-4.9f, 2.9f, 0.1f,
	-4.9f, 2.9f, 0.9f,
	-4.1f, 2.9f, 0.1f,
	-4.1f, 2.9f, 0.1f,
	-4.1f, 2.9f, 0.9f,
	-4.9f, 2.9f, 0.9f,

	/* Left Side */
	-4.9f, 2.1f, 0.1f,
	-4.9f, 2.9f, 0.1f,
	-4.9f, 2.9f, 0.9f,
	-4.9f, 2.9f, 0.9f,
	-4.9f, 2.1f, 0.9f,
	-4.9f, 2.1f, 0.1f,

	/* Right Side */
	-4.1f, 2.1f, 0.1f,
	-4.1f, 2.9f, 0.1f,
	-4.1f, 2.9f, 0.9f,
	-4.1f, 2.9f, 0.9f,
	-4.1f, 2.1f, 0.9f,
	-4.1f, 2.1f, 0.1f,
};

// The u-v coordinates of each face of the cube
static GLfloat treasure_uv_coords[] = {
	// Bottom side
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	// Top side
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	// Front side
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	// Back side
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	// Left side
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,

	// Right side
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

static GLfloat treasureOnCol[] = {
	/* Bottom Side */
	-4.7f, 2.3f, 0.3f,
	-4.7f, 2.7f, 0.3f,
	-4.3f, 2.3f, 0.3f,
	-4.7f, 2.7f, 0.3f,
	-4.3f, 2.3f, 0.3f,
	-4.3f, 2.7f, 0.3f,

	/* Top Side */
	-4.7f, 2.3f, 0.7f,
	-4.7f, 2.7f, 0.7f,
	-4.3f, 2.3f, 0.7f,
	-4.7f, 2.7f, 0.7f,
	-4.3f, 2.3f, 0.7f,
	-4.3f, 2.7f, 0.7f,

	/* Front Side */
	-4.7f, 2.3f, 0.3f,
	-4.7f, 2.3f, 0.7f,
	-4.3f, 2.3f, 0.3f,
	-4.3f, 2.3f, 0.3f,
	-4.3f, 2.3f, 0.7f,
	-4.7f, 2.3f, 0.7f,

	/* Back Side */
	-4.7f, 2.7f, 0.3f,
	-4.7f, 2.7f, 0.7f,
	-4.3f, 2.7f, 0.3f,
	-4.3f, 2.7f, 0.3f,
	-4.3f, 2.7f, 0.7f,
	-4.7f, 2.7f, 0.7f,

	/* Left Side */
	-4.7f, 2.3f, 0.3f,
	-4.7f, 2.7f, 0.3f,
	-4.7f, 2.7f, 0.7f,
	-4.7f, 2.7f, 0.7f,
	-4.7f, 2.3f, 0.7f,
	-4.7f, 2.3f, 0.3f,

	/* Right Side */
	-4.3f, 2.3f, 0.3f,
	-4.3f, 2.7f, 0.3f,
	-4.3f, 2.7f, 0.7f,
	-4.3f, 2.7f, 0.7f,
	-4.3f, 2.3f, 0.7f,
	-4.3f, 2.3f, 0.3f
};

//Initial position of the cube in relation to the maze
static int treasure_pos[2] = { 2, 0 }; // [0] is for y and [1] is for x

// 44 available "pairs" (x, y) for the treasure to spawn (in relation to maze[10][10])
static int availableTreasurePos[44][2];

// Initialize values to -1
void initAvailableTreasurePos() {
	for (int i = 0; i < 44; i++) {
		availableTreasurePos[i][0] = -1;
		availableTreasurePos[i][1] = -1;
	}
}

// To check where the treasure can spawn
void findAvailableTreasurePos() {
	int index = 0; // For each one of the 44 pairs
	for (int i = 0; i < 10; i++) { // i is for the y
		for (int j = 0; j < 10; j++) { // j is for the x
			if (maze[i][j] != 1 && (player_pos[0] != i || player_pos[1] != j)) {
				availableTreasurePos[index][0] = i; // Store y-coordinate
				availableTreasurePos[index][1] = j; // Store x-coordinate
				index++;
			}
		}
	}
}

// Random position of treasure (in relation to maze[10][10])
int generateRandomNumber() {
	srand(static_cast<unsigned int>(time(0)));
	int randomNumber = rand() % 44;
	return randomNumber;
}

// Update the coordinates of the treasure accordingly
void updateTreasurePos(double newY, double newX) {
	if (newY < treasure_pos[0]) { //This means we are going up
		for (int i = 1; i < (36 * 3); i += 3) {
			treasure[i] += abs(newY - treasure_pos[0]) * 1.0f;
			treasureOnCol[i] += abs(newY - treasure_pos[0]) * 1.0f;
		}
	}

	if (newY > treasure_pos[0]) { //This means we are going down
		for (int i = 1; i < (36 * 3); i += 3) {
			treasure[i] -= abs(newY - treasure_pos[0]) * 1.0f;
			treasureOnCol[i] -= abs(newY - treasure_pos[0]) * 1.0f;
		}
	}

	if (newX < treasure_pos[1]) { //This means we are going to the left
		for (int i = 0; i < (36 * 3); i += 3) {
			treasure[i] -= abs(newX - treasure_pos[1]) * 1.0f;
			treasureOnCol[i] -= abs(newX - treasure_pos[1]) * 1.0f;
		}
	}

	if (newX > treasure_pos[1]) { //This means we are going to the right
		for (int i = 0; i < (36 * 3); i += 3) {
			treasure[i] += abs(newX - treasure_pos[1]) * 1.0f;
			treasureOnCol[i] += abs(newX - treasure_pos[1]) * 1.0f;
		}
	}

	treasure_pos[0] = newY;
	treasure_pos[1] = newX;
}

// For the texture
GLuint loadTexture(const char* filename) {
	GLuint textureID;
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

	if (data) {
		glGenTextures(1, &textureID); // Generate textureID
		glBindTexture(GL_TEXTURE_2D, textureID); // Bind texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Set image data

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data); // Free the loaded image memory
	}
	else {
		std::cerr << "Failed to load texture" << std::endl;
	}

	return textureID;
}
//------------------------------------------------

// Check if player "touched" the treasure
GLboolean checkCollision() {
	if ((player_pos[0] == treasure_pos[0]) && (player_pos[1] == treasure_pos[1])) {
		return true;
	}
	return false;
};

void makeTreasureSmaller() {
	for (int i = 0; i < 108; i++) {
		treasure[i] = treasureOnCol[i];
	}
}

/* FOR THE CAMERA */
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;
glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

// Field Of View
float FOV = 60.0f;
// Initial position
glm::vec3 position = glm::vec3(0, 0, 20.0);
// Where is camera looking
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.25f);
// Up vector
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

void camera_function()
{
	float speed = 5.0f; // 5 units per second

	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Move forward with UP arrow
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position -= glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime * speed;
	}
	// Move backward with DOWN arrow
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position += glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime * speed;
	}
	// Move right with 'h' (along X-axis)
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		position += glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime * speed;
	}
	// Move left with 'g' (along X-axis)
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		position -= glm::vec3(1.0f, 0.0f, 0.0f) * deltaTime * speed;
	}
	// Move up with 't' (along Y-axis)
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		position += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * speed;
	}
	// Move down with 'b' (along Y-axis)
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		position -= glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * speed;
	}

	// Update the view matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera position
		cameraTarget,       // Camera target (where it is always looking)
		cameraUp            // Up direction
	);
	lastTime = currentTime;
}

///////////////////////////////////////////////////
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
///////////////////////////////////////////////////

static void initColors() {
	for (int i = 0; i < (55 * RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM); i += 3) {
		maze_color[i] = 0.0f;
		maze_color[i + 1] = 0.0f;
		maze_color[i + 2] = 1.0f;
	}

	for (int i = 0; i < (RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM); i += 3) {
		player_color[i] = 1.0f;
		player_color[i + 1] = 1.0f;
		player_color[i + 2] = 0.0f;
	}
}

static void initMaze() {
	static int index = 0;

	for (int y = -5; y < 5; y++) {
		for (int x = -5; x < 5; x++) {
			if (maze[abs(y - 4)][x + 5] == 1) {
				/* bottom side */
				maze_walls_vertices[index] = (GLfloat)x;
				maze_walls_vertices[index + 1] = (GLfloat)y;
				maze_walls_vertices[index + 2] = 0.0f;

				maze_walls_vertices[index + 3] = (GLfloat)x;
				maze_walls_vertices[index + 4] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 5] = 0.0f;

				maze_walls_vertices[index + 6] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 7] = (GLfloat)y;
				maze_walls_vertices[index + 8] = 0.0f;

				maze_walls_vertices[index + 9] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 10] = (GLfloat)y;
				maze_walls_vertices[index + 11] = 0.0f;

				maze_walls_vertices[index + 12] = (GLfloat)x;
				maze_walls_vertices[index + 13] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 14] = 0.0f;

				maze_walls_vertices[index + 15] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 16] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 17] = 0.0f;

				/* top side */

				maze_walls_vertices[index + 18] = (GLfloat)x;
				maze_walls_vertices[index + 19] = (GLfloat)y;
				maze_walls_vertices[index + 20] = 1.0f;

				maze_walls_vertices[index + 21] = (GLfloat)x;
				maze_walls_vertices[index + 22] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 23] = 1.0f;

				maze_walls_vertices[index + 24] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 25] = (GLfloat)y;
				maze_walls_vertices[index + 26] = 1.0f;

				maze_walls_vertices[index + 27] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 28] = (GLfloat)y;
				maze_walls_vertices[index + 29] = 1.0f;

				maze_walls_vertices[index + 30] = (GLfloat)x;
				maze_walls_vertices[index + 31] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 32] = 1.0f;

				maze_walls_vertices[index + 33] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 34] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 35] = 1.0f;

				/* front side */

				maze_walls_vertices[index + 36] = (GLfloat)x;
				maze_walls_vertices[index + 37] = (GLfloat)y;
				maze_walls_vertices[index + 38] = 0.0f;

				maze_walls_vertices[index + 39] = (GLfloat)x;
				maze_walls_vertices[index + 40] = (GLfloat)y;
				maze_walls_vertices[index + 41] = 1.0f;

				maze_walls_vertices[index + 42] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 43] = (GLfloat)y;
				maze_walls_vertices[index + 44] = 0.0f;

				maze_walls_vertices[index + 45] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 46] = (GLfloat)y;
				maze_walls_vertices[index + 47] = 0.0f;

				maze_walls_vertices[index + 48] = (GLfloat)x;
				maze_walls_vertices[index + 49] = (GLfloat)y;
				maze_walls_vertices[index + 50] = 1.0f;

				maze_walls_vertices[index + 51] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 52] = (GLfloat)y;
				maze_walls_vertices[index + 53] = 1.0f;

				/* back side */

				maze_walls_vertices[index + 54] = (GLfloat)x;
				maze_walls_vertices[index + 55] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 56] = 0.0f;

				maze_walls_vertices[index + 57] = (GLfloat)x;
				maze_walls_vertices[index + 58] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 59] = 1.0f;

				maze_walls_vertices[index + 60] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 61] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 62] = 0.0f;

				maze_walls_vertices[index + 63] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 64] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 65] = 0.0f;

				maze_walls_vertices[index + 66] = (GLfloat)x;
				maze_walls_vertices[index + 67] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 68] = 1.0f;

				maze_walls_vertices[index + 69] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 70] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 71] = 1.0f;

				/* left side */

				maze_walls_vertices[index + 72] = (GLfloat)x;
				maze_walls_vertices[index + 73] = (GLfloat)y;
				maze_walls_vertices[index + 74] = 0.0f;

				maze_walls_vertices[index + 75] = (GLfloat)x;
				maze_walls_vertices[index + 76] = (GLfloat)y;
				maze_walls_vertices[index + 77] = 1.0f;

				maze_walls_vertices[index + 78] = (GLfloat)x;
				maze_walls_vertices[index + 79] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 80] = 0.0f;

				maze_walls_vertices[index + 81] = (GLfloat)x;
				maze_walls_vertices[index + 82] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 83] = 0.0f;

				maze_walls_vertices[index + 84] = (GLfloat)x;
				maze_walls_vertices[index + 85] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 86] = 1.0f;

				maze_walls_vertices[index + 87] = (GLfloat)x;
				maze_walls_vertices[index + 88] = (GLfloat)y;
				maze_walls_vertices[index + 89] = 1.0f;

				/* right side */

				maze_walls_vertices[index + 90] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 91] = (GLfloat)y;
				maze_walls_vertices[index + 92] = 0.0f;

				maze_walls_vertices[index + 93] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 94] = (GLfloat)y;
				maze_walls_vertices[index + 95] = 1.0f;

				maze_walls_vertices[index + 96] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 97] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 98] = 0.0f;

				maze_walls_vertices[index + 99] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 100] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 101] = 0.0f;

				maze_walls_vertices[index + 102] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 103] = (GLfloat)(y + 1);
				maze_walls_vertices[index + 104] = 1.0f;

				maze_walls_vertices[index + 105] = (GLfloat)(x + 1);
				maze_walls_vertices[index + 106] = (GLfloat)y;
				maze_walls_vertices[index + 107] = 1.0f;

				index += 108;
			}
		}
	}

	initColors();
}

bool isMoveValid(int key) {
	int maze_walls_vertices_size = (55 * 3 * 3 * 2);

	switch (key) {
	case GLFW_KEY_I:
		if (maze[player_pos[0] - 1][player_pos[1]] == 1 || (player_pos[0] - 1) < 0) {
			return false;
		}

		player_pos[0] -= 1;

		break;
	case GLFW_KEY_J:
		if (maze[player_pos[0]][player_pos[1] - 1] == 1 || (player_pos[1] - 1) < 0) {
			return false;
		}

		player_pos[1] -= 1;

		break;
	case GLFW_KEY_K:
		if (maze[player_pos[0] + 1][player_pos[1]] == 1 || (player_pos[0] + 1) > 9) {
			return false;
		}

		player_pos[0] += 1;

		break;
	case GLFW_KEY_L:
		if (maze[player_pos[0]][player_pos[1] + 1] == 1 || (player_pos[1] + 1) > 9) {
			return false;
		}

		player_pos[1] += 1;

		break;
	}
	return true;
}

void movePlayer(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_I:
		if (action == GLFW_PRESS) {
			if (!isMoveValid(GLFW_KEY_I))
				break;

			for (int i = 1; i < (36 * 3); i += 3) {
				player[i] += 1.0f;
			}
		}
		break;

	case GLFW_KEY_J:
		if (action == GLFW_PRESS) {
			if (player_pos[0] == 2 && player_pos[1] == 0) { // If player is at it's initial position and moves left
				player_pos[0] = 7;
				player_pos[1] = 9;

				for (int i = 0; i < (36 * 3); i += 3) {
					player[i] += 9.0f;
				}

				for (int i = 1; i < (36 * 3); i += 3) {
					player[i] -= 5.0f;
				}
				break;
			}

			if (!isMoveValid(GLFW_KEY_J))
				break;

			for (int i = 0; i < (36 * 3); i += 3) {
				player[i] -= 1.0f;
			}
		}
		break;

	case GLFW_KEY_K:
		if (action == GLFW_PRESS) {
			if (!isMoveValid(GLFW_KEY_K))
				break;

			for (int i = 1; i < (36 * 3); i += 3) {
				player[i] -= 1.0f;
			}
		}
		break;

	case GLFW_KEY_L:
		if (action == GLFW_PRESS) {
			if (player_pos[0] == 7 && player_pos[1] == 9) { // If player is it's rightmost position and moves right
				player_pos[0] = 2;
				player_pos[1] = 0;

				for (int i = 0; i < (36 * 3); i += 3) {
					player[i] -= 9.0f;
				}

				for (int i = 1; i < (36 * 3); i += 3) {
					player[i] += 5.0f;
				}

				break;
			}

			if (!isMoveValid(GLFW_KEY_L))
				break;

			for (int i = 0; i < (36 * 3); i += 3) {
				player[i] += 1.0f;
			}
		}
	default:
		break;
	}
}

int main(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 950x950 window with it's name
	window = glfwCreateWindow(950, 950, u8"Άσκηση 1Γ - 2024", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// background color (black)
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("P1BVertexShader.vertexshader", "P1BFragmentShader.fragmentshader");
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Initialize maze
	initMaze();

	// Initialize the availableTreasurePos array
	initAvailableTreasurePos();

	/* All the buffers we are going to need */
	GLuint mazebuffer;
	glGenBuffers(1, &mazebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mazebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(maze_walls_vertices), maze_walls_vertices, GL_STATIC_DRAW);

	GLuint mazeColorBuffer;
	glGenBuffers(1, &mazeColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mazeColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(maze_color), maze_color, GL_STATIC_DRAW);

	GLuint playerbuffer;
	glGenBuffers(1, &playerbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, playerbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player), player, GL_STATIC_DRAW);

	GLuint playerColorBuffer;
	glGenBuffers(1, &playerColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, playerColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player_color), player_color, GL_STATIC_DRAW);

	GLuint treasurebuffer;
	glGenBuffers(1, &treasurebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, treasurebuffer);

	GLuint treasureUVbuffer;
	glGenBuffers(1, &treasureUVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, treasureUVbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(treasure_uv_coords), treasure_uv_coords, GL_STATIC_DRAW);

	// Load the texture
	GLuint textureID = loadTexture("coins.jpg");

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// "Call" the camera
		camera_function();

		glm::mat4 Projection = glm::perspective(glm::radians(FOV), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = getViewMatrix();
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// For the shaders (in order to apply texture)
		glUniform1i(glGetUniformLocation(programID, "useTexture"), 0); // 0 = false

		// For the maze 
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mazebuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// For the color of the maze
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mazeColorBuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw maze
		glDrawArrays(GL_TRIANGLES, 0, 55 * RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM);

		// For the movement of the player
		glfwSetKeyCallback(window, movePlayer);

		// For the player. The player is visible when there hasn't been a collision
		if (!checkCollision()) {
			glBindBuffer(GL_ARRAY_BUFFER, playerbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(player), player, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			// For the color of the player
			glBindBuffer(GL_ARRAY_BUFFER, playerColorBuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// Draw player
			glDrawArrays(GL_TRIANGLES, 0, RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM);
		}
		else {
			// Clear the player buffer when not visible
			glBindBuffer(GL_ARRAY_BUFFER, playerbuffer);
			glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); // Clear the buffer

			glBindBuffer(GL_ARRAY_BUFFER, playerColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); // Clear the UV buffer
		}

		// This is for the first random spawn of the treasure
		static GLboolean firstSpawn = true;
		if (firstSpawn) {
			int randomNumber = generateRandomNumber();
			findAvailableTreasurePos();
			int newY = availableTreasurePos[randomNumber][0];
			int newX = availableTreasurePos[randomNumber][1];
			updateTreasurePos(newY, newX);
			firstSpawn = false;
		}

		// The treasure moves around the maze every 3 seconds. If the treasure has been touched it does not change it's position
		static GLboolean touched = false;
		if (checkCollision()) {
			makeTreasureSmaller();
			touched = true;
		}

		if (!touched) { // Not yet touched so change position randomly
			static double lastTime = 0.0;
			double currentTime = glfwGetTime();
			if (currentTime - lastTime >= 3.0) {  // Move every 3 seconds
				int randomNumber = generateRandomNumber();
				findAvailableTreasurePos();
				int newY = availableTreasurePos[randomNumber][0];
				int newX = availableTreasurePos[randomNumber][1];
				updateTreasurePos(newY, newX);
				lastTime = currentTime;
			}
		}else { // Player touched the treasure, make it small, let the user see it and the dissapear
			static double lastTime2 = glfwGetTime();
			double currentTime2 = glfwGetTime();
			if (currentTime2 - lastTime2 >= 3.0) {  // After 3 seconds
				updateTreasurePos(-100, 100); // Move the treasure out of the view
				lastTime2 = currentTime2;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, treasurebuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(treasure), treasure, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glUniform1i(glGetUniformLocation(programID, "useTexture"), 1);
		glActiveTexture(GL_TEXTURE0);
		glEnableVertexAttribArray(2);

		// For the texture
		glBindBuffer(GL_ARRAY_BUFFER, treasureUVbuffer);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM);

		glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0); // Texture unit 0

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &mazebuffer);
	glDeleteBuffers(1, &mazeColorBuffer);
	glDeleteBuffers(1, &playerbuffer);
	glDeleteBuffers(1, &playerColorBuffer);
	glDeleteBuffers(1, &treasurebuffer);

	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}