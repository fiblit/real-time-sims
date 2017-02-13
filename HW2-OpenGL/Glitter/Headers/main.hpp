#ifndef MAIN
#define MAIN
#pragma once

// GL
#include <glad/glad.h> //OpenGL function loader: https://github.com/Dav1dde/glad
#include <GLFW/glfw3.h>

// STL
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

// Globals
const GLint G_winWidth = 1280;
const GLint G_winHeight = 800;
const GLboolean G_winFullscreen = GL_FALSE;

// Prototypes
void OK();
int DIE(int retVal);
void slowPrint(int middle, int after, std::string txt);// just for fun

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

#endif //MAIN
