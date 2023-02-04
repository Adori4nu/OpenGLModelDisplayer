#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb/stb_image.h>


#include "Texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"

constexpr unsigned int WIDTH = 1024;
constexpr unsigned int HEIGHT = 1024;


int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Vertices coordinates
	GLfloat vertices[] =
	{ //		COORDINATES				 /		COLORS					//			TEXTURE MAPPIN CORDINATES
		-0.5f,  0.0f,  0.5f,					0.83f, 0.70f, 0.44f,				0.0f, 0.0f,						// Lower left
		-0.5f,  0.0f, -0.5f,					0.83f, 0.70f, 0.44f,				5.0f, 0.0f,						// Lower right
		 0.5f,  0.0f, -0.5f,					0.83f, 0.70f, 0.44f,				0.0f, 0.0f,						// Upper corrner
		 0.5f,  0.0f,  0.5f,					0.83f, 0.70f, 0.44f,				5.0f, 0.0f,						// Inner left
		 0.0f,  0.8f,  0.0f,					0.92f, 0.86f, 0.76f,				2.5f, 5.0f,						// Inner left
	};

	GLuint indices[] =
	{
		0, 1, 2, // Upper triangle
		0, 2, 3, // Lower trinagle
		0, 1, 4, // Lower trinagle
		1, 2, 4, // Lower trinagle
		2, 3, 4, // Lower trinagle
		3, 0, 4, // Lower trinagle
	};

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "FreeCodeCamp_OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Scpecify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0 to x = 1024, y = 1024
	glViewport(0, 0, WIDTH, HEIGHT);

	// Generates Shader Object using shader default.vert and .frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to verticies
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbinds all to prevent accidental modifying
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Texture
	
	Texture popCat("Resources/pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);

	Camera camera(WIDTH, HEIGHT, glm::vec3(-1.75f, 1.25f, -2.5f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		popCat.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the fornt buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the progam
	glfwTerminate();
	return 0;
}