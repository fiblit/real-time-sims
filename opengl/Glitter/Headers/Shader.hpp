#ifndef SHADER_H_GUARD
#define SHADER_H_GUARD
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// include to get all the required OpenGL headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader {
public:
	GLuint program;
	
	//To be clear, I'm just following the tutorial, Header-only class feels really weird, wrong even
	Shader(const GLchar* vertPath, const GLchar* fragPath) {
		// read in mandatory shaders
		const GLchar* vertCode = readShader(vertPath);
		D(std::cout << "hello" << std::endl << vertCode << std::endl);
		const GLchar* fragCode = readShader(fragPath);
		D(std::cout << "hello2" << std::endl << fragCode << std::endl);

		// compile shaders (and check for errors)
		GLuint vert = compileShader(vertCode, GL_VERTEX_SHADER);
		GLuint frag = compileShader(fragCode, GL_FRAGMENT_SHADER);

		// this way I can link in more shaders if I wanted to :p I wonder if C++ has variable arguments...
		GLuint* shaders = new GLuint[2];
		shaders[0] = vert; shaders[1] = frag;

		// link shaders
		this->program = linkShaders(shaders, 2);

		// clean up shaders as they're linked in and no longer necessary
		deleteShaders(shaders, 2);
	}

	const GLchar* readShader(const GLchar* path) {
		// Retrieve the source code from its path
		std::string code;
		std::ifstream shader;

		// ensures ifstream objects can throw exceptions
		shader.exceptions(std::ifstream::badbit);
		try {
			shader.open(path);
			std::stringstream stream;
			stream << shader.rdbuf();
			shader.close();
			code = stream.str();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "ERROR: Shader file could not be read: " << path << std::endl;
		}

		GLchar* c = new GLchar[code.length()];
		for (int i = 0; i < code.length(); i++)
			c[i] = code[i];
		D(std::cout << "hello" << std::endl << c << std::endl);
		return c;
	}

	GLuint compileShader(const GLchar* code, GLenum shaderType) {
		GLuint shader;
		GLint success;
		GLchar infoLog[512];

		// compile
		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &code, NULL);
		glCompileShader(shader);

		// check for errors
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cerr << "ERROR: A shader file failed to compile" << std::endl << infoLog << std::endl;
		}

		return shader;
	}

	GLuint linkShaders(GLuint shaders[], int n) {
		GLint success;
		GLchar infoLog[512];
		GLuint program = glCreateProgram();
		for (int i = 0; i < n; i++)
			glAttachShader(program, shaders[i]);
		glLinkProgram(program);

		// check for linking errors
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cerr << "ERROR: A shader failed to link" << std::endl << infoLog << std::endl;
		}

		return program;
	}

	void deleteShaders(GLuint shaders[], int n) {
		for (int i = 0; i < n; i++)
			glDeleteShader(shaders[i]);
		delete[] shaders;
	}

	void Use() { glUseProgram(this->program); };
};

#endif // SHADER_H_GUARD
