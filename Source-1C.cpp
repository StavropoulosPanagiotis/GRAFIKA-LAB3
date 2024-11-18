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
	-4.75f, 2.25f, 0.0f,
	-4.75f, 2.75f, 0.0f,
	-4.25f, 2.25f, 0.0f,
	-4.75f, 2.75f, 0.0f,
	-4.25f, 2.25f, 0.0f,
	-4.25f, 2.75f, 0.0f,

	/* Top Side */
	-4.75f, 2.25f, 1.0f,
	-4.75f, 2.75f, 1.0f,
	-4.25f, 2.25f, 1.0f,
	-4.75f, 2.75f, 1.0f,
	-4.25f, 2.25f, 1.0f,
	-4.25f, 2.75f, 1.0f,

	/* Front Side */
	-4.75f, 2.25f, 0.0f,
	-4.75f, 2.25f, 1.0f,
	-4.25f, 2.25f, 0.0f,
	-4.25f, 2.25f, 0.0f,
	-4.25f, 2.25f, 1.0f,
	-4.75f, 2.25f, 1.0f,

	/* Back Side */
	-4.75f, 2.75f, 0.0f,
	-4.75f, 2.75f, 1.0f,
	-4.25f, 2.75f, 0.0f,
	-4.25f, 2.75f, 0.0f,
	-4.25f, 2.75f, 1.0f,
	-4.75f, 2.75f, 1.0f,

	/* Left Side */
	-4.75f, 2.25f, 0.0f,
	-4.75f, 2.75f, 0.0f,
	-4.75f, 2.75f, 1.0f,
	-4.75f, 2.75f, 1.0f,
	-4.75f, 2.25f, 1.0f,
	-4.75f, 2.25f, 0.0f,

	/* Right Side */
	-4.25f, 2.25f, 0.0f,
	-4.25f, 2.75f, 0.0f,
	-4.25f, 2.75f, 1.0f,
	-4.25f, 2.75f, 1.0f,
	-4.25f, 2.25f, 1.0f,
	-4.25f, 2.25f, 0.0f
};

static int player_pos[2] = { 2, 0 }; // The initial position of the player in the maze[10][10] array

// The B object (treasure), which is a cube with each side being 0.8 in length
static GLfloat treasure_color[RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM];
static GLfloat treasure[] = {
	/* Bottom Side */
	-3.75f, 2.25f, 0.0f,
	-3.75f, 2.75f, 0.0f,
	-3.25f, 2.25f, 0.0f,
	-3.75f, 2.75f, 0.0f,
	-3.25f, 2.25f, 0.0f,
	-3.25f, 2.75f, 0.0f,

	/* Top Side */
	-3.75f, 2.25f, 1.0f,
	-3.75f, 2.75f, 1.0f,
	-3.25f, 2.25f, 1.0f,
	-3.75f, 2.75f, 1.0f,
	-3.25f, 2.25f, 1.0f,
	-3.25f, 2.75f, 1.0f,

	/* Front Side */
	-3.75f, 2.25f, 0.0f,
	-3.75f, 2.25f, 1.0f,
	-3.25f, 2.25f, 0.0f,
	-3.25f, 2.25f, 0.0f,
	-3.25f, 2.25f, 1.0f,
	-3.75f, 2.25f, 1.0f,

	/* Back Side */
	-3.75f, 2.75f, 0.0f,
	-3.75f, 2.75f, 1.0f,
	-3.25f, 2.75f, 0.0f,
	-3.25f, 2.75f, 0.0f,
	-3.25f, 2.75f, 1.0f,
	-3.75f, 2.75f, 1.0f,

	/* Left Side */
	-3.75f, 2.25f, 0.0f,
	-3.75f, 2.75f, 0.0f,
	-3.75f, 2.75f, 1.0f,
	-3.75f, 2.75f, 1.0f,
	-3.75f, 2.25f, 1.0f,
	-3.75f, 2.25f, 0.0f,

	/* Right Side */
	-3.25f, 2.25f, 0.0f,
	-3.25f, 2.75f, 0.0f,
	-3.25f, 2.75f, 1.0f,
	-3.25f, 2.75f, 1.0f,
	-3.25f, 2.25f, 1.0f,
	-3.25f, 2.25f, 0.0f
};
	

/* FOR THE CAMERA */
glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second

void camera_function()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Turn right with w
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		horizontalAngle += deltaTime * speed;
	}
	// Turn left with x
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		horizontalAngle -= deltaTime * speed;
	}
	// Look up with q
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		verticalAngle += deltaTime * speed;
	}
	// Look down with z
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		verticalAngle -= deltaTime * speed;
	}
	// Zoom in with Up arrow
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += glm::vec3(0.0f, 0.0f, -1.0f) * deltaTime * speed;
	}
	// Zoom out with Down arrow
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position += glm::vec3(0.0f, 0.0f, 1.0f) * deltaTime * speed;
	}

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Set the view matrix based on the static position and updated direction
	ViewMatrix = glm::lookAt(
		position,           // Keep the camera at a fixed position
		position + direction,  // Update where the camera is looking
		up                  // Head is up
	);

	lastTime = currentTime;
}

/////////////////////////////////////////////////
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

	for (int i = 0; i < (RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM); i += 3) {
		treasure_color[i] = 1.0f;
		treasure_color[i + 1] = 1.0f;
		treasure_color[i + 2] = 0.0f;
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
		break;
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

	GLfloat len = 5.0f, wid = 2.5f, heig = 2.5f;

	// Initialize maze
	initMaze();

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(treasure), treasure, GL_STATIC_DRAW);

	GLuint treasureColorBuffer;
	glGenBuffers(1, &treasureColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, treasureColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(treasure_color), treasure_color, GL_STATIC_DRAW);

	do {

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		camera_function();

		glm::mat4 Projection = glm::perspective(glm::radians(100.0f), 4.0f / 4.0f, 0.1f, 100.0f);
		// Camera matrix
		glm::mat4 View = getViewMatrix();
		glm::mat4 Model = glm::mat4(1.0f);
		glm::mat4 MVP = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// For the maze 
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, mazebuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset           
		);

		// For the color of the maze
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, mazeColorBuffer);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// Draw maze
		glDrawArrays(GL_TRIANGLES, 0, 55 * RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM);

		// For the player
		glBindBuffer(GL_ARRAY_BUFFER, playerbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(player), player, GL_STATIC_DRAW);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// For the color of the player
		glBindBuffer(GL_ARRAY_BUFFER, playerColorBuffer);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// Draw player
		glDrawArrays(GL_TRIANGLES, 0, RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM);

		// For the treasure
		glBindBuffer(GL_ARRAY_BUFFER, treasurebuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(treasure), treasure, GL_STATIC_DRAW);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// For the color of the treasure
		glBindBuffer(GL_ARRAY_BUFFER, treasureColorBuffer);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		// Draw treasure
		glDrawArrays(GL_TRIANGLES, 0, RECT_VERTICES_NUM * SIDE_NUM * COORDS_NUM);

		// For the movement of the player
		glfwSetKeyCallback(window, movePlayer);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the SPACE key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &mazebuffer);
	glDeleteBuffers(1, &mazeColorBuffer);
	glDeleteBuffers(1, &playerbuffer);
	glDeleteBuffers(1, &playerColorBuffer);
	glDeleteBuffers(1, &treasurebuffer);
	glDeleteBuffers(1, &treasureColorBuffer);


	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}