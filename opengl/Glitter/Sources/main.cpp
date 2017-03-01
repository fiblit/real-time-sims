#include "main.hpp"

int main(int argc, char * argv[]) {
    /* Load GLFW  */
	D(std::cout << "Initializing GLFW for OpenGL 3.3...");
    glfwInit();
	// Set required hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For macs
	D(OK());

	/* Create Window Context */
	D(std::cout << "Creating GLFW Window Context...");
	//handle fullscreen
	GLFWmonitor* monitor = nullptr;
	if (G::WIN_FULLSCREEN)
		monitor = glfwGetPrimaryMonitor();
	//Make a window
	std::string title = "Dalton Hildreth - 5611 - Homework 2: Cloth Simulations ::: OpenGL v3.3";
    GLFWwindow* window = glfwCreateWindow(G::WIN_WIDTH, G::WIN_HEIGHT, title.c_str(), monitor, nullptr);
    if (window == nullptr) {
		std::cerr << "Failed to create OpenGL Context" << std::endl;
		return DIE(EXIT_FAILURE);
    }
	glfwMakeContextCurrent(window);
	D(OK());

	/* Define callbacks */
	D(std::cout << "Setting Callbacks...");
	glfwSetKeyCallback(window, key_callback);
	D(OK());

	/* load OpenGL 3.3 functions with glad */
	D(std::cout << "Loading OpenGL with glad...");
	gladLoadGL();
	D(std::cout << "OK ::: OpenGL " << glGetString(GL_VERSION) << std::endl);

	/* Handle Viewport */
	D(std::cout << "Creating viewport...");
	int width; int height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	D(OK());

	/* Vertices */
	GLfloat vertices[] = {
		 0.5f,  0.5f, 0.0f,  // Top Right
		 0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left
	};
	GLuint indices[] = {  // Note that we start from 0
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. Bind Vertex Array Object
	glBindVertexArray(VAO);
		// 2. Copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// 3. Copy our index array in an element buffer for OpenGL to use
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// 3. Then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	// 4. Unbind the VAO (NOT the EBO)
	glBindVertexArray(0);


	/* Shaders */
	//TODO: loading shaders
	const GLchar * vsh_src =
		"#version 330 core\n\
		\n\
		layout (location = 0) in vec3 position;\n\
		\n\
		void main() {\n\
			gl_Position = vec4(position.x, position.y, position.z, 1.0);\n\
		}\n";

	GLuint vsh;
	vsh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsh, 1, &vsh_src, NULL);
	glCompileShader(vsh);

	// did it compile?
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vsh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vsh, 512, NULL, infoLog);
		std::cerr << "ERROR: Vertex Shader did not compile\n" << infoLog << std::endl;
		return DIE(EXIT_FAILURE);
	}

	const GLchar * fsh_src =
		"#version 330 core\n\
		\n\
		out vec4 color;\n\
		\n\
		void main() {\n\
			color = vec4(1.05, 0.5f, 0.2f, 1.0f);\n\
		}\n";
	GLuint fsh;
	fsh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsh, 1, &fsh_src, NULL);
	glCompileShader(fsh);

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vsh);
	glAttachShader(shaderProgram, fsh);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR: Shaders did not compile\n" << infoLog << std::endl;
		return DIE(EXIT_FAILURE);
	}
	glDeleteShader(vsh);
	glDeleteShader(fsh);

	//TODO: finish the shader tutorial
	//TODO: finish the textures tutorial:
	//TODO: ... turtles all the way down ... finish 'em all!

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Game Loop */
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);
    while (!glfwWindowShouldClose(window)) {
		//Callbacks
		glfwPollEvents();

		/* Render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//Double Buffer
		glfwSwapBuffers(window);
    }
	D(std::cout << std::endl << "Exiting Game Loop..." << std::endl << std::endl);
	
	/* Exit */
    return DIE(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//Press ESC to close Application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		D(std::cout << "ESC was pressed" << std::endl);
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int DIE(int retVal) {
	glfwTerminate();

	std::cerr << std::endl << "Application Terminated. With exit value: " << retVal << std::endl;
	D(slowPrint(50, 300, "\n\nGoodbye..."));
	D(slowPrint(150, 500, "OK"));

	return retVal;
}
