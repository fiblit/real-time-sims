#include "main.hpp"

int main() {

	_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
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

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Shaders */
	Shader* rainbow = new Shader("..\\Glitter\\Shaders\\identity.vert", "..\\Glitter\\Shaders\\simple.frag");
	cam = new Camera();
	ShallowWater* sw = new ShallowWater(50, 9.8f);

	D(std::cout << "1" << std::endl);

	/* Objects */
	GLuint VAO[2];
	glGenVertexArrays(2, VAO);
	GLuint VBO[2];
	glGenBuffers(2, VBO);
	D(std::cout << "2" << std::endl);

	glBindVertexArray(VAO[0]);				// occasionally a crash here
	D(std::cout << "21" << std::endl);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	D(std::cout << "s" << sizeof(obj::cube) << std::endl);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obj::cube), obj::cube, GL_STATIC_DRAW);
	D(std::cout << "22" << std::endl);
	// Position attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attr
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0); // unbind VBO

	D(std::cout << "3" << std::endl);
	
	
	//GLuint* VAO_shallow_water = new GLuint[1];
	//glGenVertexArrays(1, VAO_shallow_water);
	//GLuint* VBO_shallow_water = new GLuint[1];
	//glGenVertexArrays(1, VBO_shallow_water);

	
	glBindVertexArray(VAO[1]);				// occasionally a crash here
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);

	D(std::cout << "4" << std::endl);
	D(std::cout << "s" << sizeof(GLfloat) * 3 * (6 * (sw->ncells - 1)) << std::endl);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * (6 * (sw->ncells - 1)), sw->mesh, GL_STATIC_DRAW);

	D(std::cout << "5" << std::endl);
	// Position attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	D(std::cout << "6" << std::endl);

	/* todo: textures */

    /* Game Loop */
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);
	while (!glfwWindowShouldClose(window)) {

		D(std::cout << "7" << std::endl);
		// Time
		GLfloat currentFrame = (GLfloat) glfwGetTime();
		timer::delta = currentFrame - timer::last;
		timer::last = currentFrame;

		D(std::cout << "8" << std::endl);

		//Callbacks
		glfwPollEvents();
		do_movement();

		D(std::cout << "9" << std::endl);

		/* Render */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		D(std::cout << "10" << std::endl);

		glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam->getView();

		D(std::cout << "11" << std::endl);

		rainbow->use();
		
		glUniformMatrix4fv(glGetUniformLocation(rainbow->getProgram(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(rainbow->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

		D(std::cout << "12" << std::endl);
		
		glBindVertexArray(VAO[0]);
		for (GLuint i = 3; i < 6; i++) {
			glm::mat4 model;
			model = glm::translate(model, obj::cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i % 3 == 0)
				model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.3f, 0.7f, 0.8f));
			glUniformMatrix4fv(glGetUniformLocation(rainbow->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		

		D(std::cout << "13" << std::endl);
		glBindVertexArray(VAO[1]);
		glm::mat4 model;
		glUniformMatrix4fv(glGetUniformLocation(rainbow->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

		D(std::cout << "14" << std::endl);
		D(std::cout << sw->mesh[0] << std::endl);

		for (int i = 0; i < sw->ncells*6; i++) {
			for (int k = 0; k < 1; k++){ 
				GLfloat * vert = &sw->mesh[3* (6 * i + k)];
				glm::vec4 l = glm::vec4(*(vert + 0), *(vert + 1), *(vert + 2), 1.0f);
				glm::vec4 p = proj * view * model * l;
				D(std::cout << "sw " << i << "\t"<<k<<":\t" << p.x/p.z <<" "<< p.y/p.z <<" "<< p.z/p.z << std::endl);
			}
		}

		D(std::cout << "15" << std::endl);
		
		glDrawArrays(GL_TRIANGLES, 0, sw->ncells*6);

		D(std::cout << "16" << std::endl);
		
		glBindVertexArray(0);

		D(std::cout << "17" << std::endl);

		//Double Buffer
		glfwSwapBuffers(window);
		D(std::cout << "18" << std::endl);
    }
	D(std::cout << std::endl << "Exiting Game Loop..." << std::endl << std::endl);
	

	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	D(std::cout << "19" << std::endl);

	/* Exit */
    return DIE(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	//Press ESC to close Application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS) {
		D(std::cout << "KEY: " << /*glfwGetKeyName(key, key)*/ key << std::endl);
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		D(std::cout << "\tKEY: " <</*glfwGetKeyName(key, key)*/ key << std::endl);
		keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!mouse::focus) {
		mouse::lastX = (GLfloat) xpos;
		mouse::lastY = (GLfloat) ypos;
		mouse::focus = true;
	}

	GLfloat xoffset = (GLfloat) xpos - mouse::lastX;
	GLfloat yoffset = (GLfloat) ypos - mouse::lastY;
	mouse::lastX = (GLfloat) xpos;
	mouse::lastY = (GLfloat) ypos;
	//xoffset *= mouse::sensitivity;
	//yoffset *= mouse::sensitivity;

	cam->mouseRotateCamera(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset) {
	cam->scrollZoomCamera((GLfloat) yoffset);
}

void do_movement() {
	if (keys[GLFW_KEY_W])
		cam->translateCamera(G::CAMERA::FORWARD, timer::delta);
	if (keys[GLFW_KEY_S])
		cam->translateCamera(G::CAMERA::BACKWARD, timer::delta);
	if (keys[GLFW_KEY_A])
		cam->translateCamera(G::CAMERA::LEFT, timer::delta);
	if (keys[GLFW_KEY_D])
		cam->translateCamera(G::CAMERA::RIGHT, timer::delta);
}

int DIE(int retVal) {
	glfwTerminate();

	std::cerr << std::endl << "Application Terminated. With exit value: " << retVal << std::endl;
	D(slowPrint(50, 300, "\n\nGoodbye..."));
	D(slowPrint(150, 500, "OK"));

	return retVal;
}
