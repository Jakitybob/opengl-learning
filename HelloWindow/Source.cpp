#pragma once

#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <iostream>

// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

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

	// Create our VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO); // Generate buffer ID for our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind VBO to the GL ARRAY BUFFER

	// Create our basic vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Define vertexShader as an actual vertex shader

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Assign the shader source code to the shader
	glCompileShader(vertexShader); // Compile the shader

	// Create our basic fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // Assign the shader source code to the shader
	glCompileShader(fragmentShader); // Compile the shader

	// Make sure the vertex shader correctly compiled
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Make sure the fragment shader correctly compiled
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create our shader program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // Store the ID of the program

	// Link our shaders to our program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Delete old shader objects as we don't need them anymore, they are already linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Define vertices for a rectangle
	float vertices[] = { // uses normalized coordinates from -1 to 1
		0.25f, 0.25f, 0.0f,   // top right
		0.25f, -0.25f, 0.0f,  // bottom right
		-0.25f, -0.25f, 0.0f, // bottom left
		-0.25f, 0.25f, 0.0f   // top left
	};

	// Define the indices of our triangles
	unsigned int indices[] = { // note that it starts from 0
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Create our element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);

	// Bind the VBO and give it the indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Copy our vertices into our array buffer bound to the vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Tell OpenGL how to interpret vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// PARAM 1: specifies the vertex attribute we want to configure. Since the shader is setup with
	//            layout (location = 0) we also want to pass in 0.
	// PARAM 2: The next attribute specifies the size of the vertex attribute. Since we are using
	//            a 3D vector (vec3) we pass in 3.
	// PARAM 3: The third argument specifies the type of data we're using via an enum. Since we are
	//            using floats, we pass in the GL_FLOAT which is a vec* consisting of float values.
	// PARAM 4: This attribute specifies if we want the data to be normalized. If we're putting
	//            integer data types in and we've set this to GL_TRUE the integer data is normalized
	//            to 0 or -1 and 1 when converted to a float. This is not relevant to us.
	// PARAM 5: This argument is known as the "stride" and tells us the space between consecutive
	//            vertex attributes. Since the next step of position data is located exactly 3 times
	//            the size of float, that is our stride. This is possible because the array is tightly
	//            packed.
	// PARAM 6: This param is of type void* and thus requires a really weird cast. This is the offset
	//            where the position data begins in the buffer. Since the position data starts at [0],
	//            we can just pass in 0. This parameter is explored elsewhere.

	// This loop basically renders a new frame every time it is called
	while (!glfwWindowShouldClose(Window))
	{
		// Check for input every frame
		processInput(Window);

		// Rendering commands here...
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Set clear to use a teal color
		glClear(GL_COLOR_BUFFER_BIT); // Clear the screen color buffer
		
		// Enable wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// Draw our triangle
		glUseProgram(shaderProgram); // Tell OpenGL to use our shader program object
		glBindVertexArray(VAO);

		// Draw our triangle using our array and VAO instead of our EBO
		

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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
