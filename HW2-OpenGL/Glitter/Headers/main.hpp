#ifndef MAIN
#define MAIN
#pragma once

/* MACROS */
#include "debug.hpp"

/* GL */
#include <glad/glad.h> //OpenGL function loader: https://github.com/Dav1dde/glad
#include <GLFW/glfw3.h>

/* STL */
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

/* Globals */
namespace G {
	const GLint WIN_WIDTH = 1280;
	const GLint WIN_HEIGHT = 800;
	const GLboolean WIN_FULLSCREEN = GL_FALSE;
}

/* Prototypes */
int DIE(int retVal);
// TODO: outsource to new file
D(void OK();)
D(void slowPrint(int middle, int after, std::string txt);)// just for fun

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

#endif //MAIN
