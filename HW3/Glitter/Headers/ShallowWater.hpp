#ifndef SHALLOW_WATER_H_GUARD
#define SHALLOW_WATER_H_GUARD

// GL
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "debug.hpp"

class ShallowWater {
private:
	// helpers
	GLfloat* hm;
	GLfloat* uhm;
public:
	GLint ncells;
	GLfloat g;
	GLfloat dx;
	GLfloat* h;
	GLfloat* uh;
	GLfloat* mesh;

	ShallowWater(GLuint ncells, GLfloat g);

	//this should be deprecated in favor of a Physics system/manager
	void update(GLfloat dt);
	void simulate(GLfloat dt);
	void computeMesh();

	/* shallow water equations:
	(dh u / dt) = (h / p0) (dp / dx) - (dh u^2 / dx) - (dh u v / dy) + f h v
	(dh v / dt) = (h / p0) (dp / dy) - (dh u v / dx) - (dh v^2 / dy) - f h u
	(dh / dt) + (dh u / dx) + (dh v / dy) = 0

	rewritten:
	(dh / dt) + (dh u / dx) + (dh v / dy) = 0
	(dh u / dt) + (d(h u^2 + 1/2 g h^2) / dx) + (dh u v / dy) = 0
	(dh v / dt) + (dh u v / dx) + (d(h v^2 + 1/2 g h^2) / dy) = 0

	solve for dh and integrate!
	lax-wendroff scheme (like midpoint) extend to space and time
	ym = y0 + .5 dT f(y0)
	y1 = y0 + dT f(ym)

	user euler with .5 timestep at half way between gridpoints
	compute derivatives from new state
	apply new derivatives to initial state

	1D eq: (y's go away, v's go away)
	dh / dt = -(dh u / dx)
	(dh u / dt) = -(d(hu^2 + .5 g h^2) / dx)
	*/
};

#endif // SHALLOW_WATER_H_GUARD