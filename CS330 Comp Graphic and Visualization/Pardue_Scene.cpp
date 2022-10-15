#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sphere.h"

#include "shader.h"

#include <iostream>
#include "camera.h"

#include "cylinder.h"





void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
void processInput(GLFWwindow* window);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraDown = glm::vec3(1.0f, 0.0f, 0.0f);

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

bool firstMouse = true;
bool orthographic = false;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Pardue Coffee Table", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("shaderfiles/7.3.camera.vs", "shaderfiles/7.3.camera.fs");
	//applied to objects
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	//applied to light cube
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {

		-2.5f, -0.5f, -1.5f,  0.0f, 0.0f, //back right  side
		 0.5f, -0.5f, -1.5f,  1.0f, 0.0f,
		 0.5f,  0.0f, -1.5f,  1.0f, 1.0f,
		 0.5f,  0.0f, -1.5f,  1.0f, 1.0f, // back left  side
		-2.5f,  0.0f, -1.5f,  0.0f, 1.0f,
		-2.5f, -0.5f, -1.5f,  0.0f, 0.0f,
		
		-2.5f, -0.5f,  0.5f,  0.0f, 0.0f, //front right side
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.0f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.0f,  0.5f,  1.0f, 1.0f,  //front left side
		-2.5f,  0.0f,  0.5f,  0.0f, 1.0f,
		-2.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		 -2.5f,  0.0f,  0.5f,  1.0f, 0.0f,  //left  back side 
		 -2.5f,  0.0f, -1.5f,  1.0f, 1.0f,
		 -2.5f, -0.5f, -1.5f,  0.0f, 1.0f,
		 -2.5f, -0.5f, -1.5f,  0.0f, 1.0f,  // left front side 
		 -2.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -2.5f,  0.0f,  0.5f,  1.0f, 0.0f,
		 
		 0.5f,  0.0f,  0.5f,  1.0f, 0.0f, //right back side 
		 0.5f,  0.0f, -1.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -1.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -1.5f,  0.0f, 1.0f, //right front side 
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.0f,  0.5f,  1.0f, 0.0f,
		 
		 -2.5f, -0.5f, -1.5f,  0.0f, 1.0f, //bottom back right 
		  0.5f, -0.5f, -1.5f,  1.0f, 1.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //bottom front left 
		 -2.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 -2.5f, -0.5f, -1.5f,  0.0f, 1.0f,


	};



	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(10.0f,  0.2f,  -20.0f),
		
	};

	unsigned int VBO, VAO;   //game box
	unsigned int VBO2, VAO2; //game piece1
	unsigned int VBO3, VAO3; //game piece2
	unsigned int VBO4, VAO4; //game piece3
	unsigned int VBO5, VAO5; //game piece4
	unsigned int VBO6, VAO6; //game piece5

	unsigned int VBO7, VAO7; //candle
	unsigned int VBO8, VAO8; //candle2

	unsigned int VBO9, VAO9; //coffee table glass top
	unsigned int VBO10, VAO10; //coffee table stucture
	unsigned int VBO11, VAO11; //coffee table leg1
	unsigned int VBO12, VAO12;  //coffee table leg2
	unsigned int VBO13, VAO13;  //coffee table leg3
	unsigned int VBO14, VAO14;  //coffee table leg4

	unsigned int VBO15, VAO15;  //candle wick1
	unsigned int VBO16, VAO16;  //candle wick2

	unsigned int VBO17, VAO17;  //candle stand1 piece1
	unsigned int VBO18, VAO18;  //candle stand1 piece2
	unsigned int VBO19, VAO19;  //candle stand1 piece3
	unsigned int VBO20, VAO20;  //candle stand1 piece4
	unsigned int VBO21, VAO21;  //candle stand1 piece5
	unsigned int VBO22, VAO22;  //candle stand1 piece6
	unsigned int VBO23, VAO23;  //candle stand1 piece7

	unsigned int VBO24, VAO24;  //candle stand2 piece1
	unsigned int VBO25, VAO25;  //candle stand2 piece2
	unsigned int VBO26, VAO26;  //candle stand2 piece3
	unsigned int VBO27, VAO27;  //candle stand2 piece4
	unsigned int VBO28, VAO28;  //candle stand2 piece5
	unsigned int VBO29, VAO29;  //candle stand2 piece6
	unsigned int VBO30, VAO30;  //candle stand2 piece7
	unsigned int VBO31, VAO31;  //lighting object

	glGenVertexArrays(1, &VAO); //game box
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glGenVertexArrays(1, &VAO2);// game piece1
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	glGenVertexArrays(1, &VAO3);  //game piece2
	glGenBuffers(1, &VBO3);
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);

	glGenVertexArrays(1, &VAO4);  //game piece3
	glGenBuffers(1, &VBO4);
	glBindVertexArray(VAO4);
	glBindBuffer(GL_ARRAY_BUFFER, VBO4);

	glGenVertexArrays(1, &VAO5);  //game piece4
	glGenBuffers(1, &VBO5);
	glBindVertexArray(VAO5);
	glBindBuffer(GL_ARRAY_BUFFER, VBO5);

	glGenVertexArrays(1, &VAO6);  //game piece5
	glGenBuffers(1, &VBO6);
	glBindVertexArray(VAO6);
	glBindBuffer(GL_ARRAY_BUFFER, VBO6);

	glGenVertexArrays(1, &VAO7);  //candle1
	glGenBuffers(1, &VBO7);
	glBindVertexArray(VAO7);
	glBindBuffer(GL_ARRAY_BUFFER, VBO7);

	glGenVertexArrays(1, &VAO8);  //candle2
	glGenBuffers(1, &VBO8);
	glBindVertexArray(VAO8);
	glBindBuffer(GL_ARRAY_BUFFER, VBO8);

	glGenVertexArrays(1, &VAO9);  //coffee table glass top
	glGenBuffers(1, &VBO9);
	glBindVertexArray(VAO9);
	glBindBuffer(GL_ARRAY_BUFFER, VBO9);

	glGenVertexArrays(1, &VAO10);  //coffee table structure 
	glGenBuffers(1, &VBO10);
	glBindVertexArray(VAO10);
	glBindBuffer(GL_ARRAY_BUFFER, VBO10);

	glGenVertexArrays(1, &VAO11);  //coffee table structure leg1
	glGenBuffers(1, &VBO11);
	glBindVertexArray(VAO11);
	glBindBuffer(GL_ARRAY_BUFFER, VBO11);

	glGenVertexArrays(1, &VAO12);  //coffee table structure leg2
	glGenBuffers(1, &VBO12);
	glBindVertexArray(VAO12);
	glBindBuffer(GL_ARRAY_BUFFER, VBO12);

	glGenVertexArrays(1, &VAO13);  //coffee table structure leg3
	glGenBuffers(1, &VBO13);
	glBindVertexArray(VAO13);
	glBindBuffer(GL_ARRAY_BUFFER, VBO13);

	glGenVertexArrays(1, &VAO14);  //coffee table structure leg4
	glGenBuffers(1, &VBO14);
	glBindVertexArray(VAO14);
	glBindBuffer(GL_ARRAY_BUFFER, VBO14);

	glGenVertexArrays(1, &VAO15);  //candle wick1
	glGenBuffers(1, &VBO15);
	glBindVertexArray(VAO15);
	glBindBuffer(GL_ARRAY_BUFFER, VBO15);

	glGenVertexArrays(1, &VAO16);  //candle wick2
	glGenBuffers(1, &VBO16);
	glBindVertexArray(VAO16);
	glBindBuffer(GL_ARRAY_BUFFER, VBO16);

	glGenVertexArrays(1, &VAO17);  //candle stand1 piece
	glGenBuffers(1, &VBO17);
	glBindVertexArray(VAO17);
	glBindBuffer(GL_ARRAY_BUFFER, VBO17);

	glGenVertexArrays(1, &VAO18);  //candle stand1 piece2
	glGenBuffers(1, &VBO18);
	glBindVertexArray(VAO18);
	glBindBuffer(GL_ARRAY_BUFFER, VBO18);

	glGenVertexArrays(1, &VAO19);  //candle stand1 piece3
	glGenBuffers(1, &VBO19);
	glBindVertexArray(VAO19);
	glBindBuffer(GL_ARRAY_BUFFER, VBO19);

	glGenVertexArrays(1, &VAO20);  //candle stand1 piece4
	glGenBuffers(1, &VBO20);
	glBindVertexArray(VAO20);
	glBindBuffer(GL_ARRAY_BUFFER, VBO20);

	glGenVertexArrays(1, &VAO21);  //candle stand1 piece5
	glGenBuffers(1, &VBO21);
	glBindVertexArray(VAO21);
	glBindBuffer(GL_ARRAY_BUFFER, VBO21);

	glGenVertexArrays(1, &VAO22);  //candle stand1 piece6
	glGenBuffers(1, &VBO22);
	glBindVertexArray(VAO22);
	glBindBuffer(GL_ARRAY_BUFFER, VBO22);

	glGenVertexArrays(1, &VAO23);  //candle stand1 piece7
	glGenBuffers(1, &VBO23);
	glBindVertexArray(VAO23);
	glBindBuffer(GL_ARRAY_BUFFER, VBO23);

	glGenVertexArrays(1, &VAO24);  //candle stand2 piece1
	glGenBuffers(1, &VBO24);
	glBindVertexArray(VAO24);
	glBindBuffer(GL_ARRAY_BUFFER, VBO24);

	glGenVertexArrays(1, &VAO25);  //candle stand2 piece2
	glGenBuffers(1, &VBO25);
	glBindVertexArray(VAO25);
	glBindBuffer(GL_ARRAY_BUFFER, VBO25);

	glGenVertexArrays(1, &VAO26);  //candle stand2 piece3
	glGenBuffers(1, &VBO26);
	glBindVertexArray(VAO26);
	glBindBuffer(GL_ARRAY_BUFFER, VBO26);

	glGenVertexArrays(1, &VAO27);  //candle stand2 piece4
	glGenBuffers(1, &VBO27);
	glBindVertexArray(VAO27);
	glBindBuffer(GL_ARRAY_BUFFER, VBO27);

	glGenVertexArrays(1, &VAO28);  //candle stand2 piece5
	glGenBuffers(1, &VBO28);
	glBindVertexArray(VAO28);
	glBindBuffer(GL_ARRAY_BUFFER, VBO28);

	glGenVertexArrays(1, &VAO29);  //candle stand2 piece6
	glGenBuffers(1, &VBO29);
	glBindVertexArray(VAO29);
	glBindBuffer(GL_ARRAY_BUFFER, VBO29);

	glGenVertexArrays(1, &VAO30);  //candle stand2 piece7
	glGenBuffers(1, &VBO30);
	glBindVertexArray(VAO30);
	glBindBuffer(GL_ARRAY_BUFFER, VBO30);

	glGenVertexArrays(1, &VAO31);  //lamp object
	glGenBuffers(1, &VBO31);
	glBindVertexArray(VAO31);
	glBindBuffer(GL_ARRAY_BUFFER, VBO31);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2, texture3, texture4, texture5, texture6;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("images/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// texture 2 seaglass coffee table
// ---------
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data2 = stbi_load("images/underwater.jpg", &width, &height, &nrChannels, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data2);

	// texture 3 game box
// ---------
	glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data3 = stbi_load("images/wood_table.jpg", &width, &height, &nrChannels, 0);
	if (data3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data3);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data3);

	// texture 4 candles
// ---------
	glGenTextures(1, &texture4);
	glBindTexture(GL_TEXTURE_2D, texture4);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data4 = stbi_load("images/transparentGlass.jpg", &width, &height, &nrChannels, 0);
	if (data4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data4);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data4);

	// texture 5 coffee table structure
// ---------
	glGenTextures(1, &texture5);
	glBindTexture(GL_TEXTURE_2D, texture5);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data5 = stbi_load("images/sun.png", &width, &height, &nrChannels, 0);
	if (data5)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data5);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data5);

	// texture 6 candle stands
// ---------
    glGenTextures(1, &texture6);
	glBindTexture(GL_TEXTURE_2D, texture6);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data6 = stbi_load("images/steel2.jpg", &width, &height, &nrChannels, 0);
	if (data6)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data6);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data6);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);




	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setInt("texture3", 2);
	ourShader.setInt("texture4", 3);
	ourShader.setInt("texture5", 4);
	ourShader.setInt("texture6", 5);


	glm::mat4 model;
	float angle;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//activate shader when setting uniforms/drawing objects
		
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		// directional light
		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2

		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3

		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4

		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight

		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		// activate shader
		ourShader.use();

		// camera/view transformation
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);
		
		// world transform
		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);

		// we now draw as many light bulbs as we have point lights.
		unsigned int lightCubeVAO;
		glGenVertexArrays(1, &lightCubeVAO);
		glBindVertexArray(lightCubeVAO);
		
		glBindVertexArray(lightCubeVAO);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -25.0f));
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		ourShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	
		

		// render boxes

		// table game
		//-----------------------------------------------------------------------------
		glBindVertexArray(VAO);
		model = glm::mat4(1.0f); // game box	
		model = glm::translate(model, glm::vec3(-4.0f, -2.2f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightCubeShader.setMat4("model", model);
		//ourShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		lightingShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO2); //game piece 1 middle
		model = glm::mat4(1.0f); 		
		model = glm::translate(model, glm::vec3(-4.0f, -2.3f, -25.0f));
		ourShader.setMat4("model", model);
		
		Sphere S(0.4, 300, 20);
		S.Draw();
		
        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO3);  //game piece 2  bottom
		model = glm::mat4(1.0f); 	
		model = glm::translate(model, glm::vec3(-5.0f, -2.3f, -25.0f));
		ourShader.setMat4("model", model);

		//lightCubeShader.setMat4("model", model);
		Sphere S2(1, 300, 20);
		S.Draw();
		

        glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO4);  //game piece 3 top right
		model = glm::mat4(1.0f); 	
		model = glm::translate(model, glm::vec3(-4.5f, -2.3f, -27.0f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		Sphere S3(1, 300, 20);
		S.Draw();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO5);  //game piece 4 top left
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.5f, -2.3f, -27.0f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		Sphere S4(1, 300, 20);
		S.Draw();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glBindVertexArray(VAO6);  //game piece 5 bottom left
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, -2.3f, -25.0f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		Sphere S5(1, 300, 20);
		S.Draw();

		// candles
        //------------------------------------------------------------------------------------
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4); // candle texture
		
	    glBindVertexArray(VAO7);//candle 1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.2f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C2(0.5, 20, 1.5, true, true, true);
		C2.render();
		
		
		glBindVertexArray(VAO8);// candle2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, 1.0f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder C3(0.5, 20, 1.5, true, true, true);
		C3.render();
		

		// coffee table 
		//--------------------------------------------------------------------------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture2);// coffee table top texture
		
		glBindVertexArray(VAO9);//coffee table top
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -30.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.5f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C(9.8, 300, 0.5, true, true, true);
		C.render();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5); // coffee table structure texture

		glBindVertexArray(VAO10);// coffee table structure
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -30.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C4(10, 300, 0.2, true, true, true);
		C4.render();

		
		glBindVertexArray(VAO11);// coffee table structure leg back right
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, -5.7f, -38.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C5(0.1, 20, -5.5, true, true, true);
		C5.render();


		glBindVertexArray(VAO12);// coffee table structure leg2 back left
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, -5.7f, -38.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C6(0.1, 20, -5.5, true, true, true);
		C6.render();

		glBindVertexArray(VAO13);// coffee table structure leg3 front left
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-6.0f, -5.7f, -22.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C7(0.1, 20, -5.5, true, true, true);
		C7.render();

		glBindVertexArray(VAO14);// coffee table structure leg4 front right
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(6.0f, -5.7f, -22.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C8(0.1, 20, -5.5, true, true, true);
		C8.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5); // candle wick texture

		glBindVertexArray(VAO15);// candle wick
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 1.0f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C9(0.04, 20, 0.2, true, true, true);
		C9.render();

		glBindVertexArray(VAO16);// candle wick2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, 1.8f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		static_meshes_3D::Cylinder C10(0.04, 20, 0.2, true, true, true);
		C10.render();

		// candle stands
		//-----------------------------------------------------------------------------
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6); // candle stand texture
		
		glBindVertexArray(VAO17);// candle stand1 piece1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -2.6f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C11(0.8, 20, 0.3, true, true, true);
		C11.render();
		
		glBindVertexArray(VAO18);// candle stand1 piece2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -2.2f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C12(0.6, 20, 0.3, true, true, true);
		C12.render();

		glBindVertexArray(VAO19);// candle stand1 piece3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -1.8f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C13(0.4, 20, 0.6, true, true, true);
		C13.render();

		glBindVertexArray(VAO20);// candle stand1 piece4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -1.4f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C14(0.6, 20, 0.3, true, true, true);
		C14.render();

		glBindVertexArray(VAO21);  //candle stand1 piece5
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -1.0f, -25.0f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		Sphere S6(0.4, 300, 20);
		S6.Draw();

		
		glBindVertexArray(VAO22);// candle stand1 piece6
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -1.3f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C15(0.3, 20, 3.0, true, true, true);
		C15.render();

		
		glBindVertexArray(VAO23);// candle stand1 piece7
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, 0.2f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C16(0.8, 20, 0.05, true, true, true);
		C16.render();

		// candle stand2
		
		glBindVertexArray(VAO24);// candle stand2 piece1
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -2.6f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C17(0.8, 20, 0.3, true, true, true);
		C17.render();

		glBindVertexArray(VAO25);// candle stand2 piece2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -2.2f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C18(0.6, 20, 0.3, true, true, true);
		C18.render();

		glBindVertexArray(VAO26);// candle stand2 piece3
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -1.8f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C19(0.4, 20, 0.6, true, true, true);
		C19.render();
		
		glBindVertexArray(VAO27);// candle stand2 piece4
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -1.4f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C20(0.6, 20, 0.3, true, true, true);
		C20.render();

		glBindVertexArray(VAO28);  //candle stand2 piece5
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -1.0f, -25.0f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		Sphere S7(0.4, 300, 20);
		S7.Draw();

		glBindVertexArray(VAO29);// candle stand2 piece6
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -2.6f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		//lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C21(0.3, 20, 1.0, true, true, true);
		C21.render();
		
		

		glBindVertexArray(VAO30);// candle stand2 piece7
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, -0.6f, -25.0f));
		angle = 0.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.1f));
		ourShader.setMat4("model", model);
		lightCubeShader.setMat4("model", model);
		static_meshes_3D::Cylinder C22(0.8, 20, 0.05, true, true, true);
		C22.render();

		lightCubeShader.use();
		glBindVertexArray(VAO31);  // lamp object
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 5.0f, -25.0f));
		Sphere S8(0.8, 300, 20);
		S8.Draw();
        lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("model", model);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);  //game box texture
		
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 5.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //camera zoom in
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //camera zoom out
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //camera left
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //camera right
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)   //Down Camera
		cameraPos -= glm::normalize(glm::cross(cameraFront,  cameraDown)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)   //Up Camera
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraDown)) * cameraSpeed;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)//mouse camera up
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraDown)) * cameraSpeed;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)//mouse camera down
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraDown)) * cameraSpeed;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)//mouse camera scroll out
		cameraPos -= cameraSpeed * cameraFront;

	//if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		//orthographic = !orthographic;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.005f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw))* cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

unsigned int loadTexture(const char* path)
{
	return 0;
}
