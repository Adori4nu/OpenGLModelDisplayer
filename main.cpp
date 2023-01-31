#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"


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
	{ //				COORDINATES                  /		COLORS			//
		-0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f,	0.8f, 0.3f, 0.02f,  // Lower left
		0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f,	0.8f, 0.3f, 0.02f,  // Lower right
		0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f,	1.0f, 0.6f, 0.32f,	// Upper corrner
		-0.25f, 0.5f * float(sqrt(3)) / 6,	   0.0f,	0.9f, 0.45f, 0.17f,	// Inner left
		0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,	0.9f, 0.45f, 0.17f,	// Inner right
		0.0f,  -0.5f * float(sqrt(3)) / 3,     0.0f,	0.8f, 0.3f, 0.02f,	// Inner down
	};

	GLuint indices[] =
	{
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Lower right trinagle
		5, 4, 1  // Upper triangle
	};

	// Create a GLFWwindow object
	GLFWwindow* window = glfwCreateWindow(1024, 1024, "FreeCodeCamp_OpenGL", NULL, NULL);
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
	glViewport(0, 0, 1024, 1024);

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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbinds all to prevent accidental modifying
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		glUniform1f(uniID, 0.5f);
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the fornt buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the progam
	glfwTerminate();
	return 0;
}