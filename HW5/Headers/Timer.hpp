#ifndef TIMER_H_GUARD
#define TIMER_H_GUARD

/* MACROS */
#include "debug.hpp"

/* GL */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Timer {
private:
	// Time of this frame
	GLfloat time;
	// Difference in time between this frame and the last frame
	GLfloat delta;
public:
	Timer() {
		time = (GLfloat)glfwGetTime();
		delta = 0.0f;
	};

	void tick() {
		GLfloat last = time;
		time = (GLfloat)glfwGetTime();
		delta = time - last;
	};

	GLfloat getDelta() {
		return delta;
	};
	GLfloat getTime() {
		return time;
	};
};

#endif // TIMER_H_GUARD
