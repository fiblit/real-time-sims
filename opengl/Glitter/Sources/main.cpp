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
	GLfloat vertices0[] = {
		// Positions         // Colors
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Top Right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom Right
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // Top Left 
	};
	GLfloat vertices1[] = {
		 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,// Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,// Bottom Left
		-0.5f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f// Top Left
	};
	//GLfloat ** vertices = new GLfloat*[2];
	//vertices[0] = vertices1;
	//vertices[1] = vertices2;
	GLuint indices[] = {  // Note that we start from 0
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	GLuint * VAO = new GLuint[2];
	glGenVertexArrays(2, VAO);
	GLuint * VBO = new GLuint[2];
	glGenBuffers(2, VBO);
	//GLuint EBO;
	//glGenBuffers(1, &EBO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. Bind Vertex Array Object
	glBindVertexArray(VAO[0]);
	// 2. Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	// 3. Copy our index array in an element buffer for OpenGL to use
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// 3. Then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attr
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// 4. Unbind the VAO (NOT the EBO)
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// Position attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attr
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	/* Shaders */
	Shader* rainbow = new Shader("E:\\djdjh\\Documents\\Classes\\5611\\5611-HW\\opengl\\Glitter\\Shaders\\identity.vert", "E:\\djdjh\\Documents\\Classes\\5611\\5611-HW\\opengl\\Glitter\\Shaders\\simple.frag");
	Shader* yellow = new Shader("E:\\djdjh\\Documents\\Classes\\5611\\5611-HW\\opengl\\Glitter\\Shaders\\identity.vert", "E:\\djdjh\\Documents\\Classes\\5611\\5611-HW\\opengl\\Glitter\\Shaders\\yellow.frag");
	//TODO: finish the textures tutorial:
	//TODO: ... turtles all the way down ... finish 'em all!

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /* Game Loop */
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);
	while (!glfwWindowShouldClose(window)) {
		//Callbacks
		glfwPollEvents();

		/* Render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		rainbow->Use();
		//GLfloat timeValue = glfwGetTime();
		//GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		//GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		yellow->Use();
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		//Double Buffer
		glfwSwapBuffers(window);
    }
	D(std::cout << std::endl << "Exiting Game Loop..." << std::endl << std::endl);
	
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

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
