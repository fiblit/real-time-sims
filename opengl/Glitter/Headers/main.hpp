#ifndef MAIN
#define MAIN
#pragma once

/* LOCALS */
/* MACROS */
#include "debug.hpp"

#include "Shader.hpp"

/* GL */
// glad: an OpenGL function loader: https://github.com/Dav1dde/glad
#include <glad/glad.h>
// glfw: a library for I/O and OpenGL context creation: http://www.glfw.org/
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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

#endif //MAIN
