#include "main.hpp"

int main(int argc, char * argv[]) {
    /* Load GLFW  */
	std::cout << "Initializing GLFW for OpenGL 3.3...";
    glfwInit();
	// Set required hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //For macs
	OK();

	/* Create Window Context */
	std::cout << "Creating GLFW Window Context...";
	//handle fullscreen
	GLFWmonitor* monitor = nullptr;
	if (G_winFullscreen)
		monitor = glfwGetPrimaryMonitor();
	//Make a window
	std::string title = "Dalton Hildreth - 5611 - Homework 2: Cloth Simulations ::: OpenGL v3.3";
    GLFWwindow* window = glfwCreateWindow(G_winWidth, G_winHeight, title.c_str(), monitor, nullptr);
    if (window == nullptr) {
		std::cerr << "Failed to create OpenGL Context" << std::endl;
		return DIE(EXIT_FAILURE);
    }
	glfwMakeContextCurrent(window);
	OK();

	/* Define callbacks */
	std::cout << "Setting Callbacks...";
	glfwSetKeyCallback(window, key_callback);
	OK();

	/* load OpenGL 3.3 functions with glad */
	std::cout << "Loading OpenGL with glad...";
	gladLoadGL();
	fprintf(stderr, "OK ::: OpenGL %s\n", glGetString(GL_VERSION));

	/* Handle Viewport */
	std::cout << "Creating viewport...";
	int width; int height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	OK();

	/* Triangle */
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* Game Loop */
	std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl;
    while (!glfwWindowShouldClose(window)) {
		//Callbacks
		glfwPollEvents();

		/* Render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Double Buffer
		glfwSwapBuffers(window);
    }
	std::cout << std::endl << "Exiting Game Loop..." << std::endl << std::endl;
	
	/* Exit */
    return DIE(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//Press ESC to close Application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		std::cout << "ESC was pressed" << std::endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

int DIE(int retVal) {
	glfwTerminate();
	std::cout << std::endl << "Application Terminated." << std::endl;
	slowPrint(50, 300, "\n\nGoodbye...");
	slowPrint(150, 500, "OK");
	return retVal;
}

void slowPrint(int middle, int after, std::string txt) {
	for (int i = 0; i < txt.size() - 1; i++) {
		std::cout << txt[i];
		std::this_thread::sleep_for(std::chrono::milliseconds(middle));
	}
	std::cout << txt[txt.size() - 1];
	std::this_thread::sleep_for(std::chrono::milliseconds(after));
}

void OK() {
	std::cout << "OK" << std::endl;
}
