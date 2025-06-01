#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <iostream>
#include "Shader.h"

// Function Declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // The major version, currently set to OpenGL 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // The minor version, currently set to OpenGL 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required for MacOS but NOT for me :3

	GLFWwindow* Window = glfwCreateWindow(800, 800, "OpenGL Test", NULL, NULL); // Create the window teehee
	if (!Window)
	{
		std::cout << "Failed to create GLFW Window." << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// Make the window we created the current window for the OpenGL context
	glfwMakeContextCurrent(Window);

	// Initialize GLAD
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	// Set up the GLAD viewport
	glViewport(0, 0, 800, 800);
	glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback); // Tie our callback function to window resizing event

	Shader shader("flip.vert", "test.frag");
	Shader otherShader("test.vert", "test.frag");

	// Create our vertex array objects
	unsigned int FirstVAO;
	glGenVertexArrays(1, &FirstVAO);

	unsigned int SecondVAO;
	glGenVertexArrays(1, &SecondVAO);

	// Create our vertex buffer objects
	unsigned int FirstVBO;
	glGenBuffers(1, &FirstVBO); // Generate buffer ID for our vertex buffer

	unsigned int SecondVBO;
	glGenBuffers(1, &SecondVBO);

	// Define vertices for a rectangle
	float firstVertices[] = { // uses normalized coordinates from -1 to 1
		// location         // colors
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Triangle bottom right
		0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // Triangle bottom left
		-0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // Triangle top
	};

	// Define vertices for second triangle
	float secondVertices[] = {
		0.0f, -0.5f, 0.0f, 0.3f, 0.6f, 0.9f,  // Triangle bottom right
		0.5f, -0.5f, 0.0f, 0.9f, 0.6f, 0.3f,  // Triangle bottom left
		0.25f, 0.0f, 0.0f, 0.6f, 0.9f, 0.3f // Triangle top
	};

	// VERTEX ARRAY OBJECT FOR FIRST TRIANGLE
	//   because the vertex array stores attribarray calls,
	//   attribpointer calls, and any VBOs associated to it!

	// Step 0: Create VBO
	unsigned int firstVBO;
	glGenBuffers(1, &firstVBO);

	// Step 1: Create the VAO and bind it
	unsigned int firstVAO;
	glGenVertexArrays(1, &firstVAO);
	glBindVertexArray(firstVAO);

	// Step 2: Copy vertices array buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, firstVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstVertices), firstVertices, GL_STATIC_DRAW);

	// Step 3: Set vertex attribute pointers
	//    3.1: Set up position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0); // the last param is the offset, 0 for position
	glEnableVertexAttribArray(0);

	//   3.2: Set up color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float))); // the last param is offset, 3 for color
	glEnableVertexAttribArray(1);
	
	
	// VERTEX ARRAY OBJECT FOR SECOND TRIANGLE

	unsigned int secondVBO;
	glGenBuffers(1, &secondVBO);

	unsigned int secondVAO;
	glGenVertexArrays(1, &secondVAO);
	glBindVertexArray(secondVAO);

	glBindBuffer(GL_ARRAY_BUFFER, secondVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(secondVertices), secondVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(0);

	//   3.2: Set up color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float))); // the last param is offset, 3 for color
	glEnableVertexAttribArray(1);

	// 
	// RENDER LOOP
	//
	while (!glfwWindowShouldClose(Window))
	{
		// Check for input every frame
		processInput(Window);

		// Set up the clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set clear to use a teal color
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen color buffer
		////////////////////////////////////////////////////////////////////////////////////////////

		//float timeValue = glfwGetTime(); // Get the current time
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // Create a green value between 0 and 0.5 using sin and division, then make sure it is at least 0.5.
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // Get the location of the uniform from our shader program

		shader.Use();
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // Get our uniform variable and supply its value
		glBindVertexArray(firstVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		otherShader.Use();
		glBindVertexArray(secondVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		////////////////////////////////////////////////////////////////////////////////////////////
		// Check and call events and swap buffers
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Callback for when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Checks for key presses
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // Check if the escape key is pressed
	{
		glfwSetWindowShouldClose(window, true);
	}
}