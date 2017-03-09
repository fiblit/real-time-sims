#include "ShallowWater.hpp"

ShallowWater::ShallowWater(GLuint ncells, GLfloat g) {
	this->ncells = ncells;
	this->g = g;
	dx = .1f;
	h = new GLfloat[ncells];
	uh = new GLfloat[ncells];

	// helpers
	hm = new GLfloat[ncells];
	uhm = new GLfloat[ncells];

	for (int i = 0; i < ncells; i++) {
		h[i] = hm[i] = 0.5f;
	}

	h[5] = 1.7f;

	mesh = new GLfloat[3 * (6 * (ncells - 1))];
	//*(mesh + 1) = new GLfloat[3 * (ncells - 1)];

	computeMesh();
}

void ShallowWater::update(GLfloat dt) {
	GLfloat sim_dt = 0.002f;
	for (int i = 0; i < (int)(dt / sim_dt); i++)
		simulate(sim_dt);
	computeMesh();
}

void ShallowWater::simulate(GLfloat dt) {
	// compute midpoints
	for (int i = 0; i < ncells - 1; i++) {
		GLfloat d = (uh[i + 1] * uh[i + 1] + 0.5 * g * h[i + 1] * h[i + 1]) - (uh[i] * uh[i] + 0.5 * g * h[i] * h[i]);
		GLfloat duh = uh[i + 1] - uh[i];
		hm[i] = (h[i] + h[i + 1]) / 2.0f + (dt / 2.0f) * (- duh/dx);
		uhm[i] = (uh[i] + uh[i + 1]) / 2.0f + (dt / 2.0f) * (-d/dx);
	}

	// compute fullsteps
	GLfloat damp = .1;
	for (int i = 0; i < ncells - 2; i++) {
		GLfloat d = (uhm[i + 1] * uhm[i + 1] + 0.5 * g * hm[i + 1] * hm[i + 1]) - (uhm[i] * uhm[i] + 0.5 * g * hm[i] * hm[i]);
		GLfloat duhm = uhm[i + 1] - uhm[i];
		h[i + 1] += dt * (- duhm / dx);
		uh[i + 1] += dt * (- d/dx);
	}

	h[0] = h[1];
	h[ncells - 1] = h[ncells - 2];
	uh[0] = -uh[1];
	uh[ncells - 1] = -uh[ncells - 2];
}

void ShallowWater::computeMesh() {
	if (mesh != nullptr) {
		//for (int i = 0; i < 2; i++)
		//	if (mesh[i] != nullptr)
		//		delete[] mesh[i];
//		delete[] mesh;
	}
	glm::vec3* v = new glm::vec3[6 * (ncells - 1)];
	glm::vec3* n = new glm::vec3[ncells - 1];
	for (int i = 0; i < ncells - 1; i++) {
		GLfloat x1 = i*dx;//+ left?
		GLfloat x2 = (i + 1)*dx;//+left?

		glm::vec3 bl = glm::vec3(x1, h[i], 0);
		glm::vec3 br = glm::vec3(x2, h[i + 1], 0);
		glm::vec3 tl = glm::vec3(x1, h[i], -2);
		glm::vec3 tr = glm::vec3(x2, h[i + 1], -2);

		v[6 * i + 0] = tl;
		v[6 * i + 1] = bl;
		v[6 * i + 2] = br;

		v[6 * i + 3] = tr;
		v[6 * i + 4] = tl;
		v[6 * i + 5] = br;

		glm::vec3 v1 = glm::vec3(x2, h[i + 1], 0) - glm::vec3(x1, h[i], 0);
		glm::vec3 v2 = glm::vec3(x1, h[i], -1) - glm::vec3(x1, h[i], 0);
		n[i] = glm::cross(v[2 * i + 0], v[2 * i + 1]);
	}
	
	for (int i = 0; i < ncells - 1; i++) {
		for (int k = 0; k < 6; k++){
			mesh[3 * 6 * i + 3 * k + 0] = v[6 * i + k].x;
			mesh[3 * 6 * i + 3 * k + 1] = v[6 * i + k].y;
			mesh[3 * 6 * i + 3 * k + 2] = v[6 * i + k].z;
		}

		//mesh[1][3 * i + 0] = n[i].x;
		//mesh[1][3 * i + 1] = n[i].y;
		//mesh[1][3 * i + 2] = n[i].z;
	}
}