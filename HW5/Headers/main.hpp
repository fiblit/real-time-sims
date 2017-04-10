#ifndef MAIN
#define MAIN
#pragma once

/* LOCALS */
/* MACROS */
#include "debug.hpp"

/* CLASSES */
#include "Shader.hpp"
#include "Camera.hpp"
#include "Timer.hpp"
#include "PRM.hpp"
#include "GlobalMotionPlanner.hpp"

/* GL */
// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <glad/glad.h>
// glfw: a library for I/O and OpenGL context creation: http://www.glfw.org/
#include <GLFW/glfw3.h>
// glm: OpenGL mathematics: http://glm.g-truc.net/0.9.8/index.html
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// assimp: model loading: https://github.com/assimp/assimp
//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
// stb: image loading: https://github.com/nothings/stb/blob/master/stb_image.h
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/* STL */
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

/* Globals */
namespace G {
	const GLint WIN_WIDTH = 1920;
	const GLint WIN_HEIGHT = 1080;
	const GLboolean WIN_FULLSCREEN = GL_FALSE;
}
namespace obj {//should be in G
	GLfloat cube[] = {
		// Positions          // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	GLuint NR_CUBES = 0;
	glm::vec3 * cubeSpecularColor;
	glm::vec3 * cubeDiffuseColor;
	GLfloat * cubeScale;
	glm::vec3 * cubePositions;

	GLuint NR_OBST = 0;
	GLfloat * obstScale;
	glm::vec4 * obstRotation;
	glm::vec3 * obstPositions;

	GLuint NR_RECT = 0;
	glm::vec2 * rectScale;
	glm::vec3 * rectPositions;

	GLuint NR_AGENT = 0;
	GLfloat * agentScale;
	glm::vec4 * agentRotation;
	glm::vec3 * agentPositions;
}
namespace mouse {
	GLfloat lastX = G::WIN_WIDTH / 0.5f;
	GLfloat lastY = G::WIN_HEIGHT / 0.5f;
	GLboolean focus = false;
	//GLfloat sensitivity = 0.25f;
}

Camera* cam;
Timer* timer;
const GLuint cylinder_res = 11;

Cspace_2D * cspace;
PRM * prm;

GLuint cur_mode;
Circle * cur_cob;
Rect * cur_rob;

Point startPoint, goalPoint;
std::vector<Rect> rectBounds;
Rect * ragentBound;
std::vector<Circle> obstBounds;
Circle * cagentBound;
std::vector<Node<Point> *> * pathVec;
std::unordered_set<Node<Point> *> * path_;

GLboolean isFlashlightOn;
GLuint completed_nodes_;

/* UI prototypes */
bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

void toggleFlashlight();

void init_planning();
void init_planning_vis();
void replan();
void placeObst(glm::vec3 pos);
void placeGoalNode(glm::vec3 pos);
void placeStartNode(glm::vec3 pos);
void modeToggleCurrentObstacle();
void scaleCurrentObstacle(GLfloat xs, GLfloat ys, GLfloat dt);
void moveCurrentObstacle(GLfloat xs, GLfloat ys, GLfloat dt);

/* Other Prototypes */
int DIE(int retVal);
void animate_agent(Cspace_2D * c, std::vector<Node<Point> *> * path, GLuint * completed_nodes, GLfloat dt);
/*
TODO:
115 points
----
Videos (+5)
	==> 2hr
Write-up (including A* comparison) (+5)
	==> .5hr

----
125 Points
*/

#endif //MAIN
