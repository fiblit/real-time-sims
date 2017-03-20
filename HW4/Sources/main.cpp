#include "main.hpp"

int main() {
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
	std::string title = "Dalton Hildreth ::: gg engine ::: OpenGL v3.3";
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

	/* load all OpenGL functions */
	D(std::cout << "Loading OpenGL with glad...");
	// via glad using the glfw loader function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		return DIE(EXIT_FAILURE);
	}
	// Alternative use the builtin loader, e.g. if no other loader function is available
	/*if (!gladLoadGL()) {
		std::cerr << "Failed to initialize OpenGL context" << std::endl;
		return DIE(EXIT_FAILURE);
	}*/
	D(std::cout << "OK ::: OpenGL " << glGetString(GL_VERSION) << std::endl);

	/* Handle Viewport */
	D(std::cout << "Creating viewport...");
	GLint width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	D(OK());

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Management */
	timer = new Timer();

	/* Shaders */
	Shader* cubeShader = new Shader(((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.vert").c_str(), ((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.frag").c_str());
	Shader* lampShader = new Shader(((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.vert").c_str(), ((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.frag").c_str());
	cam = new Camera();

	/* Objects */
	// cubes
	const GLuint numBuf = 1;
	GLuint VAO[numBuf]; // should probably make this dynamically resizable... / managed
	glGenVertexArrays(numBuf, VAO);
	GLuint VBO[numBuf];
	glGenBuffers(numBuf, VBO);

	glBindVertexArray(VAO[0]);				// occasionally a crash here
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(obj::cube), obj::cube, GL_STATIC_DRAW);
	// Position attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attr
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Tex Coords attr
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// lamps
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	// Position attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	/* Load textures */
	GLenum tex_format;
	GLint tex_width, tex_height, tex_channels;
	GLubyte * image = stbi_load(((std::string)PROJECT_SOURCE_DIR + "/Resources/container2.png").c_str(), &tex_width, &tex_height, &tex_channels, 0);
	if (!image)
		std::cerr << "Failed to load texture ..//..//Resources//container2.png" << std::endl;
	switch (tex_channels) {
		case 1: tex_format = GL_ALPHA;     break;
		case 2: tex_format = GL_LUMINANCE; break;
		case 3: tex_format = GL_RGB;       break;
		case 4: tex_format = GL_RGBA;      break;
	}

	GLuint tex_container;
	glGenTextures(1, &tex_container);
	glBindTexture(GL_TEXTURE_2D, tex_container);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, tex_format, tex_width, tex_height, 0, tex_format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	image = stbi_load(((std::string)PROJECT_SOURCE_DIR + (std::string)"/Resources/container2_specular.png").c_str(), &tex_width, &tex_height, &tex_channels, 0);
	if (!image)
		std::cerr << "Failed to load texture ..//Resources//container2_specular.png" << std::endl;
	switch (tex_channels) {
		case 1: tex_format = GL_ALPHA;     break;
		case 2: tex_format = GL_LUMINANCE; break;
		case 3: tex_format = GL_RGB;       break;
		case 4: tex_format = GL_RGBA;      break;
	}

	GLuint tex_container_specular;
	glGenTextures(1, &tex_container_specular);
	glBindTexture(GL_TEXTURE_2D, tex_container_specular);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, tex_format, tex_width, tex_height, 0, tex_format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 1);

	/* Path Planning */
	const int NR_OBST = 1;
	Circle agentBounds;
	Point start; start.x = -9.0f; start.y = -9.0f;
	Point goal; goal.x = 9.0f; goal.y = 9.0f;
	agentBounds.o = start;
	agentBounds.r = 1.0f;
	Circle obstBounds[NR_OBST];
	obstBounds[0].o.x = 0.0f;
	obstBounds[0].o.y = 0.0f;
	obstBounds[0].r = 2.0f;
	Cspace_2D * cspace = new Cspace_2D(obstBounds, NR_OBST, agentBounds);
	PRM * prm = new PRM(start, goal, cspace);
	std::vector<Node<Point> *> * path = prm->findPathUCS(); //UCS before A*; 'tis simpler

	std::vector<Node<Point> *> * verts = prm->roadmap->vertices;
	obj::cubePositions = new glm::vec3[verts->size()];
	for (int i = 0; i < verts->size(); i++)
		obj::cubePositions[i] = glm::vec3(verts->at(i)->data.x, 0.0f, verts->at(i)->data.y);

	//change properties for the path; maybe turn into a set so I can just easily check membership
	//

    /* Game Loop */
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);
	while (!glfwWindowShouldClose(window)) {
		timer->tick();

		/* Callbacks */
		glfwPollEvents();
		do_movement();

		/* Render */
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam->getView();
		glm::mat4 model;

		glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f); // Decrease the influence
		glm::vec3 lightAmbient = glm::vec3(0.05f, 0.05f, 0.05f); // Low influence
		glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};
		glm::vec3 dirLightDir = glm::vec3(0.0f, -1.0f, 0.0f);

		cubeShader->use();
		/*TODO: something to make these lines shorter / not as many */
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform1i(glGetUniformLocation(cubeShader->getProgram(), "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(cubeShader->getProgram(), "material.specular"), 1);
		glUniform1f(glGetUniformLocation(cubeShader->getProgram(), "material.shine"), 32.0f);

		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "dirLight.direction"), dirLightDir.x, dirLightDir.y, dirLightDir.z);
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "dirLight.ambient"), lightAmbient.x, lightAmbient.y, lightAmbient.z);
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "dirLight.diffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "dirLight.specular"), lightSpecular.x, lightSpecular.y, lightSpecular.z);

		for (GLuint i = 0; i < 4; i++) {
			std::string si = "pointLights[" + std::to_string(i) + "].";
			glUniform3f(glGetUniformLocation(cubeShader->getProgram(), (si + "position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform1f(glGetUniformLocation(cubeShader->getProgram(), (si + "constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(cubeShader->getProgram(), (si + "linear").c_str()), 0.045f);
			glUniform1f(glGetUniformLocation(cubeShader->getProgram(), (si + "quadratic").c_str()), 0.0075f);
			glUniform3f(glGetUniformLocation(cubeShader->getProgram(), (si + "ambient").c_str()), lightAmbient.x, lightAmbient.y, lightAmbient.z);
			glUniform3f(glGetUniformLocation(cubeShader->getProgram(), (si + "diffuse").c_str()), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
			glUniform3f(glGetUniformLocation(cubeShader->getProgram(), (si + "specular").c_str()), lightSpecular.x, lightSpecular.y, lightSpecular.z);
		}

		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.position"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.direction"), cam->dir.x, cam->dir.y, cam->dir.z);
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.ambient"), lightAmbient.x, lightAmbient.y, lightAmbient.z);
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.diffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
		glUniform3f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.specular"), lightSpecular.x, lightSpecular.y, lightSpecular.z);
		glUniform1f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.linear"), 0.045f);
		glUniform1f(glGetUniformLocation(cubeShader->getProgram(), "spotLight.quadratic"), 0.0075f);

		glUniformMatrix4fv(glGetUniformLocation(cubeShader->getProgram(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_container);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_container_specular);

		glBindVertexArray(VAO[0]);

		/* TODO: replace with 
			render tinybox @ start point with color 1
			render tinybox @ goal point with color 2
			render tinybox @ path points with color 3
			render tintinybox @ non ^^ verts with color 4
			render box (cyl) @ agent with color 5
			render box (cyl) @ obsts with color 6
		*/
		for (GLuint i = 0; i < 10; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::cubePositions[i]);
			model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i % 3 == 0)
				model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.3f, 0.7f, 0.8f));
			glUniformMatrix4fv(glGetUniformLocation(cubeShader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(glGetUniformLocation(cubeShader->getProgram(), "normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		lampShader->use();
		glUniformMatrix4fv(glGetUniformLocation(lampShader->getProgram(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(glGetUniformLocation(lampShader->getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniform3f(glGetUniformLocation(lampShader->getProgram(), "lightColor"), lightSpecular.x, lightSpecular.y, lightSpecular.z);

		for (int i = 0; i < 4; i++) {
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(glGetUniformLocation(lampShader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS) {
		D(std::cout << "Key Pressed: " << /*glfwGetKeyName(key, key)*/ key << std::endl);
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
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
		cam->translateCamera(G::CAMERA::FORWARD, timer->getDelta());
	if (keys[GLFW_KEY_S])
		cam->translateCamera(G::CAMERA::BACKWARD, timer->getDelta());
	if (keys[GLFW_KEY_A])
		cam->translateCamera(G::CAMERA::LEFT, timer->getDelta());
	if (keys[GLFW_KEY_D])
		cam->translateCamera(G::CAMERA::RIGHT, timer->getDelta());
}

int DIE(int retVal) {
	glfwTerminate();

	std::cerr << std::endl << "Application Terminated. With exit value: " << retVal << std::endl;
	D(slowPrint(50, 300, "\n\nGoodbye..."));
	D(slowPrint(150, 500, "OK"));

	return retVal;
}
