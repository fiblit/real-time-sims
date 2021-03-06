#include "main.hpp"

/* some of this might look like stuff from learnopengl.com ... probably not much anymore 
   I have been changing it a lot. 
   Oh, and I think there's a bit from github.com/Polytonic/glitter or maybe from the
   stb github repo... It's mostly just stuff to interface with libraries (e.g. GLFW, 
   stb, glad, etc. ) */
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
	std::string title = "Dalton Hildreth ::: Homework 4";
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
	auto shv = [](auto s) {return ; };
	auto shf = [](auto s) {return ((std::string)PROJECT_SOURCE_DIR + "/Shaders/" + s + ".frag").c_str(); };
	Shader* cubeShader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.vert").c_str(), 
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/cube.frag").c_str());
	Shader* flatShader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/flat.frag").c_str());
	Shader* lampShader = new Shader(
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.vert").c_str(),
		((std::string)PROJECT_SOURCE_DIR + "/Shaders/lamp.frag").c_str());
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
	std::string RESOURCES_DIR = (std::string)PROJECT_SOURCE_DIR + "/Resources/";
	GLubyte * image = stbi_load((RESOURCES_DIR + "container2.png").c_str(), &tex_width, &tex_height, &tex_channels, 0);
	if (!image)
		std::cerr << "Failed to load texture " + RESOURCES_DIR + "container2.png" << std::endl;
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

	image = stbi_load((RESOURCES_DIR+"container2_specular.png").c_str(), &tex_width, &tex_height, &tex_channels, 0);
	if (!image)
		std::cerr << "Failed to load texture "+RESOURCES_DIR+"container2_specular.png" << std::endl;
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
	init_planning();
	replan();

	//change properties for the path -- instantiate cube visualizations
	init_planning_vis();

	/* lighting */
	glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f); // Decrease the influence
	glm::vec3 lightAmbient = glm::vec3(0.05f, 0.05f, 0.05f); // Low influence
	glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 pointLightPositions[] = {
		glm::vec3(-8.f,  1.5f,  -8.f),
		glm::vec3(8.f,  1.5f, 8.f),
		glm::vec3(0.f,  1.f, 4.f),
		glm::vec3(5.f,  2.f, -5.f)
	};
	glm::vec3 dirLightDir = glm::vec3(-0.5f, 1.0f, -0.7f);
	isFlashlightOn = true;

    /* Game Loop */
	timer->tick();
	D(std::cout << std::endl << "Entering Game Loop..." << std::endl << std::endl);	
	while (!glfwWindowShouldClose(window)) {
		timer->tick();

		animate_agent(cspace, pathVec, &completed_nodes, timer->getDelta());

		// Callbacks 
		glfwPollEvents();
		do_movement();

		// Render 
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// shaders; pass uniforms; draw
		glm::mat4 proj = glm::perspective(glm::radians(cam->fov), (GLfloat)G::WIN_WIDTH / (GLfloat)G::WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = cam->getView();
		glm::mat4 model;

		cubeShader->use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_container);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_container_specular);

		glUniform3f(cubeShader->Uni("material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform1f(cubeShader->Uni("material.shine"), 32.0f);
		glUniform1i(cubeShader->Uni("material.diffuse"), 0);
		glUniform1i(cubeShader->Uni("material.specular"), 1);

		glUniform3f(cubeShader->Uni("dirLight.direction"), dirLightDir.x, -dirLightDir.y, dirLightDir.z);
		glUniform3f(cubeShader->Uni("dirLight.ambient"), lightAmbient.x*0.1, lightAmbient.y*0.1, lightAmbient.z*0.1);
		glUniform3f(cubeShader->Uni("dirLight.diffuse"), lightDiffuse.x*0.1, lightDiffuse.y*0.1, lightDiffuse.z*0.1);
		glUniform3f(cubeShader->Uni("dirLight.specular"), lightSpecular.x*0.1, lightSpecular.y*0.1, lightSpecular.z*0.1);

		for (GLuint i = 0; i < 4; i++) {
			std::string si = "pointLights[" + std::to_string(i) + "].";
			glUniform3f(cubeShader->Uni(si + "position"), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform1f(cubeShader->Uni(si + "constant"), 1.0f);
			glUniform1f(cubeShader->Uni(si + "linear"), 0.045f);
			glUniform1f(cubeShader->Uni(si + "quadratic"), 0.0075f);
			glUniform3f(cubeShader->Uni(si + "ambient"), lightAmbient.x, lightAmbient.y, lightAmbient.z);
			glUniform3f(cubeShader->Uni(si + "diffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
			glUniform3f(cubeShader->Uni(si + "specular"), lightSpecular.x, lightSpecular.y, lightSpecular.z);
		}

		glUniform3f(cubeShader->Uni("spotLight.position"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform3f(cubeShader->Uni("spotLight.direction"), cam->dir.x, cam->dir.y, cam->dir.z);
		glUniform3f(cubeShader->Uni("spotLight.ambient"), lightAmbient.x*isFlashlightOn, lightAmbient.y*isFlashlightOn, lightAmbient.z*isFlashlightOn);
		glUniform3f(cubeShader->Uni("spotLight.diffuse"), lightDiffuse.x*isFlashlightOn, lightDiffuse.y*isFlashlightOn, lightDiffuse.z*isFlashlightOn);
		glUniform3f(cubeShader->Uni("spotLight.specular"), lightSpecular.x*isFlashlightOn, lightSpecular.y*isFlashlightOn, lightSpecular.z*isFlashlightOn);
		glUniform1f(cubeShader->Uni("spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(cubeShader->Uni("spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(cubeShader->Uni("spotLight.constant"), 1.0f);
		glUniform1f(cubeShader->Uni("spotLight.linear"), 0.007f);
		glUniform1f(cubeShader->Uni("spotLight.quadratic"), 0.0002f);

		glUniformMatrix4fv(cubeShader->Uni("proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(cubeShader->Uni("view"), 1, GL_FALSE, glm::value_ptr(view));

		glBindVertexArray(VAO[0]);

		for (GLuint i = 0; i < obj::NR_OBST; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::obstPositions[i]);
			model = glm::rotate(model, obj::obstRotation[i].w, glm::vec3(obj::obstRotation[i].x, obj::obstRotation[i].y, obj::obstRotation[i].z));
			model = glm::scale(model, glm::vec3(obj::obstScale[i]));
			glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (GLuint i = 0; i < obj::NR_RECT; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::rectPositions[i]);
			model = glm::scale(model, glm::vec3(obj::rectScale[i][0], 1.0f , obj::rectScale[i][1]));
			glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		for (GLuint i = 0; i < obj::NR_AGENT; i++) {
			model = glm::mat4();
			model = glm::translate(model, obj::agentPositions[i]);
			model = glm::rotate(model, obj::agentRotation[i].w, glm::vec3(obj::agentRotation[i].x, obj::agentRotation[i].y, obj::agentRotation[i].z));
			model = glm::scale(model, glm::vec3(obj::agentScale[i]));
			glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		switch (cur_mode) {
		case 0:
			if (cur_cob != nullptr) {
				for (int i = 0; i < cylinder_res; i++) {
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(cur_cob->o.x, 0.0f - 0.001f*i, cur_cob->o.y));
					model = glm::scale(model, glm::vec3(cur_cob->r*sqrt(2)));
					model = glm::rotate(model, glm::radians(360.0f / cylinder_res*i), glm::vec3(0.f, 1.f, 0.f));
					glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
					glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
			break;
		case 1:
			if (cur_rob != nullptr) {
				model = glm::mat4();
				model = glm::translate(model, glm::vec3(cur_rob->o.x, 0.0f, cur_rob->o.y));
				model = glm::scale(model, glm::vec3(cur_rob->w, 1.0f, cur_rob->h));
				glUniformMatrix4fv(cubeShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
				glUniformMatrix4fv(cubeShader->Uni("normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			break;
		}
		/* end cube shaders */

		flatShader->use();
		glUniform3f(flatShader->Uni("material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform1f(flatShader->Uni("material.shine"), 32.0f);

		glUniform3f(flatShader->Uni("dirLight.direction"), dirLightDir.x, dirLightDir.y, dirLightDir.z);
		glUniform3f(flatShader->Uni("dirLight.ambient"), lightAmbient.x*0.1, lightAmbient.y*0.1, lightAmbient.z*0.1);
		glUniform3f(flatShader->Uni("dirLight.diffuse"), lightDiffuse.x*0.1, lightDiffuse.y*0.1, lightDiffuse.z*0.1);
		glUniform3f(flatShader->Uni("dirLight.specular"), lightSpecular.x*0.1, lightSpecular.y*0.1, lightSpecular.z*0.1);

		for (GLuint i = 0; i < 4; i++) {
			std::string si = "pointLights[" + std::to_string(i) + "].";
			glUniform3f(flatShader->Uni( si + "position"), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
			glUniform1f(flatShader->Uni( si + "constant"), 1.0f);
			glUniform1f(flatShader->Uni( si + "linear"), 0.045f);
			glUniform1f(flatShader->Uni( si + "quadratic"), 0.0075f);
			glUniform3f(flatShader->Uni( si + "ambient"), lightAmbient.x, lightAmbient.y, lightAmbient.z);
			glUniform3f(flatShader->Uni( si + "diffuse"), lightDiffuse.x, lightDiffuse.y, lightDiffuse.z);
			glUniform3f(flatShader->Uni( si + "specular"), lightSpecular.x, lightSpecular.y, lightSpecular.z);
		}

		glUniform3f(flatShader->Uni( "spotLight.position"), cam->pos.x, cam->pos.y, cam->pos.z);
		glUniform3f(flatShader->Uni( "spotLight.direction"), cam->dir.x, cam->dir.y, cam->dir.z);
		glUniform3f(flatShader->Uni( "spotLight.ambient"), lightAmbient.x*isFlashlightOn, lightAmbient.y*isFlashlightOn, lightAmbient.z*isFlashlightOn);
		glUniform3f(flatShader->Uni( "spotLight.diffuse"), lightDiffuse.x*isFlashlightOn, lightDiffuse.y*isFlashlightOn, lightDiffuse.z*isFlashlightOn);
		glUniform3f(flatShader->Uni( "spotLight.specular"), lightSpecular.x*isFlashlightOn, lightSpecular.y*isFlashlightOn, lightSpecular.z*isFlashlightOn);
		glUniform1f(flatShader->Uni( "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(flatShader->Uni( "spotLight.fadeOff"), glm::cos(glm::radians(17.5f)));
		glUniform1f(flatShader->Uni( "spotLight.constant"), 1.0f);
		glUniform1f(flatShader->Uni( "spotLight.linear"), 0.045f);
		glUniform1f(flatShader->Uni( "spotLight.quadratic"), 0.0075f);

		glUniformMatrix4fv(flatShader->Uni( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(flatShader->Uni( "view"), 1, GL_FALSE, glm::value_ptr(view));

		for (GLuint i = 0; i < obj::NR_CUBES; i++) {
			glUniform3f(flatShader->Uni("material.diffuse"), obj::cubeDiffuseColor[i].x, obj::cubeDiffuseColor[i].y, obj::cubeDiffuseColor[i].z);
			glUniform3f(flatShader->Uni("material.specular"), obj::cubeSpecularColor[i].x, obj::cubeSpecularColor[i].y, obj::cubeSpecularColor[i].z);
			model = glm::mat4();
			model = glm::translate(model, obj::cubePositions[i]);
			model = glm::scale(model, glm::vec3(obj::cubeScale[i]));
			glUniformMatrix4fv(flatShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(flatShader->Uni( "normalMat"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(view * model))));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		/* end flat shaders */

		lampShader->use();
		glUniformMatrix4fv(lampShader->Uni( "proj"), 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(lampShader->Uni("view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniform3f(lampShader->Uni("lightColor"), lightSpecular.x, lightSpecular.y, lightSpecular.z);

		for (int i = 0; i < 4; i++) {
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(lampShader->Uni("model"), 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		/* end lamp shaders */

		// Swap the Double Buffer
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

	if (keys[GLFW_KEY_P]) {
		keys[GLFW_KEY_P] = false;
		replan();
		init_planning_vis();
	}
	if (keys[GLFW_KEY_O]) {
		keys[GLFW_KEY_O] = false;
		placeObst(cam->pos);
	}
	if (keys[GLFW_KEY_G]) {
		keys[GLFW_KEY_G] = false;
		placeGoalNode(cam->pos);
	}
	if (keys[GLFW_KEY_T]) {
		keys[GLFW_KEY_T] = false;
		placeStartNode(cam->pos);
	}
	if (keys[GLFW_KEY_M]) {
		keys[GLFW_KEY_M] = false;
		modeToggleCurrentObstacle();
	}
	if (keys[GLFW_KEY_I])
		scaleCurrentObstacle(1.f, 1.1f, timer->getDelta());
	if (keys[GLFW_KEY_K])
		scaleCurrentObstacle(1.f, 1/1.1f, timer->getDelta());
	if (keys[GLFW_KEY_J])
		scaleCurrentObstacle(1 / 1.1f, 1.f, timer->getDelta());
	if (keys[GLFW_KEY_L])
		scaleCurrentObstacle(1.1f, 1.f, timer->getDelta());
	if (keys[GLFW_KEY_UP])
		moveCurrentObstacle( 0.f,  1.f, timer->getDelta());
	if (keys[GLFW_KEY_DOWN])
		moveCurrentObstacle( 0.f, -1.f, timer->getDelta());
	if (keys[GLFW_KEY_LEFT])
		moveCurrentObstacle(-1.f,  0.f, timer->getDelta());
	if (keys[GLFW_KEY_RIGHT])
		moveCurrentObstacle( 1.f,  0.f, timer->getDelta());

	if (keys[GLFW_KEY_F]) {
		keys[GLFW_KEY_F] = false;
		toggleFlashlight();
	}
}

int DIE(int retVal) {
	glfwTerminate();

	std::cerr << std::endl << "Application Terminated. With exit value: " << retVal << std::endl;
	D(slowPrint(50, 300, "\n\nGoodbye..."));
	D(slowPrint(150, 500, "OK"));

	return retVal;
}


void animate_agent(Cspace_2D * c, std::vector<Node<Point> *> * path, int * completed_nodes, float dt) {
	if ((*completed_nodes) < path->size()) {
		float velocity = 1.0f; // x m/s
		Point agentNow;
		agentNow.x = obj::agentPositions[0].x;
		agentNow.y = obj::agentPositions[0].z;

		Point nextNode;
		nextNode.x = (*path)[(*completed_nodes)]->data.x;
		nextNode.y = (*path)[(*completed_nodes)]->data.y;

		while ((*completed_nodes) + 1 < path->size() 
				&& c->lineOfSight(agentNow, (*path)[(*completed_nodes) + 1]->data)) {
			(*completed_nodes)++;
			nextNode.x = (*path)[(*completed_nodes)]->data.x;
			nextNode.y = (*path)[(*completed_nodes)]->data.y;
		}

		float dist = distP(nextNode, agentNow);
		if (dist < 0.1f) {
			(*completed_nodes)++;
			//not sure why I have this, as this will, like, never happen.
			if (dist < dt*velocity) {
				agentNow.x = nextNode.x;
				agentNow.y = nextNode.y;
				velocity -= dist / dt;
			}
		}

		Point motion = scaleP(subP(nextNode, agentNow), velocity * dt / distP(nextNode, agentNow));
		for (int i = 0; i < obj::NR_AGENT; i++) {
			obj::agentPositions[i] += glm::vec3(motion.x, 0.0f, motion.y);
		}
	}
}

void init_planning() {
	cur_cob = nullptr;
	cur_rob = nullptr;
	cspace = nullptr;
	prm = nullptr;
	ragentBound = new Rect();// nullptr;
	cagentBound = nullptr;//new Circle();
	pathVec = nullptr;
	path = nullptr;

	startPoint.x = -9.0f; startPoint.y = -9.0f;
	goalPoint.x = 9.0f; goalPoint.y = 9.0f;

	//cagentBound->r = 1.0f;
	ragentBound->w = 1.0f;
	ragentBound->h = 1.0f;

	obstBounds = std::vector<Circle>(2);
	obstBounds[0].o.y = 0.0f;	obstBounds[0].o.x = 0.0f;	obstBounds[0].r = 2.0f;
	obstBounds[1].o.y = -7.0f;	obstBounds[1].o.x = 6.0f;	obstBounds[1].r = 1.0f;

	rectBounds = std::vector<Rect>(10);
	{int NR = 0;
	rectBounds[NR].o.y = 3.0f;  	rectBounds[NR].o.x = 8.0f;		rectBounds[NR].h = 0.1f;	rectBounds[NR].w = 4.0f; NR++;
	rectBounds[NR].o.y = -7.25f;	rectBounds[NR].o.x = -1.0f;		rectBounds[NR].h = 5.5f;	rectBounds[NR].w = 0.1f; NR++;
	rectBounds[NR].o.y = 3.0f;  	rectBounds[NR].o.x = -8.0f;		rectBounds[NR].h = 0.1f;	rectBounds[NR].w = 4.0f; NR++;
	rectBounds[NR].o.y = 3.0f;  	rectBounds[NR].o.x = 0.0f;		rectBounds[NR].h = 0.1f;	rectBounds[NR].w = 4.0f; NR++;
	rectBounds[NR].o.y = 6.5f;  	rectBounds[NR].o.x = -2.0f;		rectBounds[NR].h = 0.1f;	rectBounds[NR].w = 8.0f; NR++;//
	rectBounds[NR].o.y = 4.75f; 	rectBounds[NR].o.x = 2.0f;		rectBounds[NR].h = 3.5f;	rectBounds[NR].w = 0.1f; NR++;
	rectBounds[NR].o.y = -5.0f; 	rectBounds[NR].o.x = -5.0f;		rectBounds[NR].h = 3.0f;	rectBounds[NR].w = 0.1f; NR++;
	rectBounds[NR].o.y = -5.0f; 	rectBounds[NR].o.x = -5.0f;		rectBounds[NR].h = 0.1f;	rectBounds[NR].w = 3.0f; NR++;
	rectBounds[NR].o.y = 0.0f; 		rectBounds[NR].o.x = 4.0f;		rectBounds[NR].h = 0.1f;	rectBounds[NR].w = 4.0f; NR++;
	rectBounds[NR].o.y = -3.0f;		rectBounds[NR].o.x = 6.0f;		rectBounds[NR].h = 6.0f;	rectBounds[NR].w = 0.1f; NR++; }//
}

void init_planning_vis() {
	std::vector<Node<Point> *> * verts = prm->roadmap->vertices;
	obj::NR_CUBES = verts->size() + 1;
	obj::cubePositions = new glm::vec3[obj::NR_CUBES];
	obj::cubeScale = new float[obj::NR_CUBES];
	obj::cubeDiffuseColor = new glm::vec3[obj::NR_CUBES];
	obj::cubeSpecularColor = new glm::vec3[obj::NR_CUBES];

	for (int i = 0; i < obj::NR_CUBES - 1; i++) {
		Node<Point> * v = verts->at(i);
		obj::cubePositions[i] = glm::vec3(v->data.x, -2.0f, v->data.y);
		if (i == 0) {
			obj::cubeScale[i] = 1.0f;
			obj::cubeDiffuseColor[i] = glm::vec3(0.0f, 0.0f, 1.0f);
			obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
			obj::cubePositions[i] = glm::vec3(startPoint.x, - 2.0f, startPoint.y);
		}
		else if (i == 1) {
			obj::cubeScale[i] = 1.0f;
			obj::cubeDiffuseColor[i] = glm::vec3(1.0f, 0.0f, 0.0f);
			obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
			obj::cubePositions[i] = glm::vec3(goalPoint.x, - 2.0f, goalPoint.y);
		}
		else if (path->find(v) != path->end()) {
			obj::cubeScale[i] = 0.75f;
			obj::cubeDiffuseColor[i] = glm::vec3(0.0f, 1.0f, 0.0f);
			obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		else {
			obj::cubeScale[i] = 0.5f;
			obj::cubeDiffuseColor[i] = glm::vec3(0.4f, 0.4f, 0.4f);
			obj::cubeSpecularColor[i] = glm::vec3(1.0f, 1.0f, 1.0f);
		}
	}

	// add ground
	obj::cubeScale[obj::NR_CUBES - 1] = 20.0f;
	obj::cubePositions[obj::NR_CUBES - 1] = glm::vec3(0.0f, -12.0f, 0.0f);
	obj::cubeDiffuseColor[obj::NR_CUBES - 1] = glm::vec3(0.3f, 0.5f, 0.3f);
	obj::cubeSpecularColor[obj::NR_CUBES - 1] = glm::vec3(1.0f, 1.0f, 1.0f);

	obj::NR_OBST = cylinder_res * obstBounds.size();
	obj::obstPositions = new glm::vec3[obj::NR_OBST];
	obj::obstRotation = new glm::vec4[obj::NR_OBST];
	obj::obstScale = new float[obj::NR_OBST];
	for (int i = 0; i < obj::NR_OBST; i++) {
		obj::obstPositions[i] = glm::vec3(obstBounds[(int)i / cylinder_res].o.x, 0.0f - 0.001f*i, obstBounds[(int)i / cylinder_res].o.y);
		obj::obstScale[i] = obstBounds[(int)i / cylinder_res].r * sqrt(2);
		obj::obstRotation[i] = glm::vec4(0.0f, 1.0f, 0.0f, glm::radians(360.0f / cylinder_res * (i%cylinder_res)));
	}

	obj::NR_RECT = rectBounds.size();
	obj::rectPositions = new glm::vec3[obj::NR_RECT];
	obj::rectScale = new glm::vec2[obj::NR_RECT];
	for (int i = 0; i < obj::NR_RECT; i++) {
		obj::rectPositions[i] = glm::vec3(rectBounds[i].o.x, 0.0f, rectBounds[i].o.y);
		obj::rectScale[i] = glm::vec2(rectBounds[i].w, rectBounds[i].h);
	}

	obj::NR_AGENT = cylinder_res * 1;
	obj::agentPositions = new glm::vec3[obj::NR_AGENT];
	obj::agentRotation = new glm::vec4[obj::NR_AGENT];
	obj::agentScale = new float[obj::NR_AGENT];
	for (int i = 0; i < obj::NR_AGENT; i++) {
		if (cagentBound != nullptr) {
			obj::agentPositions[i] = glm::vec3(startPoint.x, 0.0f + 0.001f*i, startPoint.y);
			obj::agentScale[i] = cagentBound->r * sqrt(2);
			obj::agentRotation[i] = glm::vec4(0.0f, 1.0f, 0.0f, glm::radians(360.0f / cylinder_res * (i%cylinder_res)));
		}
		else {
			obj::agentPositions[i] = glm::vec3(startPoint.x, 0.0f + 0.001f*i, startPoint.y);
			obj::agentScale[i] = ragentBound->w;
			obj::agentRotation[i] = glm::vec4(0.0f, 1.0f, 0.0f, 0);

		}
	}
}


void replan() {
	switch (cur_mode) {
	case 0:
		if (cur_cob != nullptr) {
			obstBounds.push_back(*cur_cob);
			delete cur_cob;
			cur_cob = nullptr;
		}
		break;
	case 1:
		if (cur_rob != nullptr) {
			rectBounds.push_back(*cur_rob);
			delete cur_rob;
			cur_rob = nullptr;
		}
		break;
	}
	ragentBound->o = startPoint;
	for (int i = 0; i < obj::NR_AGENT; i++)
		obj::agentPositions[i] = glm::vec3(startPoint.x, 0.0f, startPoint.y);

	if (prm != nullptr)
		init_planning_vis();
	cspace = new Cspace_2D(obstBounds, rectBounds, cagentBound, ragentBound);
	prm = new PRM(startPoint, goalPoint, cspace);

	/* PATH PLANNING METHOD */
	pathVec = prm->findPathUCS(); //new std::vector<Node<Point> *>(); //even this is slow //
	path = new std::unordered_set<Node<Point> *>();
	for (int i = 0; i < pathVec->size(); i++)
		path->insert((*pathVec)[i]);

	completed_nodes = 0;
}

void toggleFlashlight() {
	isFlashlightOn = !isFlashlightOn;
}

void placeObst(glm::vec3 pos) {
	switch (cur_mode) {
	case 0:
		if (cur_cob != nullptr) {
			obstBounds.push_back(*cur_cob);
			init_planning_vis();
			delete cur_cob;
		}
		cur_cob = new Circle();
		cur_cob->o.x = pos.x;
		cur_cob->o.y = pos.z;
		cur_cob->r = 1.0f;
		break;
	case 1:
		if (cur_rob != nullptr) {
			rectBounds.push_back(*cur_rob);
			init_planning_vis();
			delete cur_rob;
		}
		cur_rob = new Rect();
		cur_rob->o.x = pos.x;
		cur_rob->o.y = pos.z;
		cur_rob->w = 1.0f;
		cur_rob->h = 1.0f;
		break;
	}
}

void placeGoalNode(glm::vec3 pos) {
	goalPoint.x = pos.x; goalPoint.y = pos.z;
	init_planning_vis();
}

void placeStartNode(glm::vec3 pos) {
	startPoint.x = pos.x; startPoint.y = pos.z;
	init_planning_vis();
}

void modeToggleCurrentObstacle() {
	switch (cur_mode) {
	case 0:
		if (cur_cob != nullptr) {
			glm::vec3 pos = glm::vec3(cur_cob->o.x, 0.0f, cur_cob->o.y);
			delete cur_cob;
			cur_cob = nullptr;
			cur_mode = 1 - cur_mode;
			placeObst(pos);
		}
		break;
	case 1:
		if (cur_rob != nullptr) {
			glm::vec3 pos = glm::vec3(cur_rob->o.x, 0.0f, cur_rob->o.y);
			delete cur_rob;
			cur_rob = nullptr;
			cur_mode = 1 - cur_mode;
			placeObst(pos);
		}
		break;
	} 
}

void scaleCurrentObstacle(float xs, float ys, float dt) {
	switch (cur_mode) {
	case 0:
		//1 - dt is a lerp
		if (cur_cob != nullptr) {
			cur_cob->r *= (xs == 1.f ? ys : xs)*dt + (1 - dt);
		}
		break;
	case 1:
		if (cur_rob != nullptr) {
			cur_rob->w *= xs*dt + (1 - dt);
			cur_rob->h *= ys*dt + (1 - dt);
		}
		break;
	}
}

void moveCurrentObstacle(float dx, float dy, float dt) {
	switch (cur_mode) {
	case 0:
		if (cur_cob != nullptr) {
			cur_cob->o.x += dx*dt;
			cur_cob->o.y += dy*dt;
		}
		break;
	case 1:
		if (cur_rob != nullptr) {
			cur_rob->o.x += dx*dt;
			cur_rob->o.y += dy*dt;
		}
		break;
	}
}
