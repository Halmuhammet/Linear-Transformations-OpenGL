/*
* Title: Linear Transformations: Module 4 - Transformations
* Description: This program will render the translated, rotated, scaled, and skewed versions of original square
* Author: Halmuhammet Muhamedorazov
* Date: 09/30/2024
* Version number: g++ 13.2.0, gcc 11.4.0
* Requirements: This program requires GLAD, GLFW, and GLM libraries
* Note: User can press UP, DOWN, RIGHT, and BOTTOM to see different translations to the square object
*		Check the terminal for instructions after running the code
* Version requirement: This program requires GLFW 3.3 or above
*/
//import the useful libraries

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// These two functions are prototypes which are defined at the end of this program
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, unsigned int shaderProgram);

// vertex shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"}\0";

// fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"  // Define color uniform
"void main()\n"
"{\n"
"   FragColor = color;\n"  // Use the color uniform
"}\n\0";



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

	// Create a GLFWwindow object of 950 by 950 pixels, naming it "Linear Transformations"
	GLFWwindow* window = glfwCreateWindow(950, 950, "Linear Transformations", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to initialize the window object" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make the context of our window the main context in current window
	glfwMakeContextCurrent(window);
	
	// This function dynamically sets the viewport size when the user resizes window
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load GLAD so it configures OpenGL
	//Glad helps getting the address of OpenGL functions which are OS specific
	gladLoadGL();


	// Create Vertex Shader Object and get its reference
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);

	//check for vertex shader compilation error
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create Fragment Shader Object and get its reference
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	//check for fragment shader compilation error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create Shader Program Object and get its reference
	unsigned int shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// square vertices (normalized)
	float vertices[] =
	{		
		0.2f, 0.2f, 0.0f, // top right corner
		0.2f, -0.2, 0.0f, // bottom right corner
		-0.2f, -0.2f, 0.0f, // bottom left corner
		-0.2f, 0.2f, 0.0f // top left corner
	};
	

	// It is important that the indices are integers; took me a while to debug this
	unsigned int indices[] =
	{
		0, 1, 3, // indices for top triangle
		1, 2, 3	// indices for bottom triangle
	};

	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAO, VBO, EBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// we also need to bind the EBO so that vertex shader knows how to form triangles
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// we want to draw in wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Show the instructins to the user regarding transformation action keys
	std::cout << "Press and hold RIGHT key to TRANSLATE the original square\n";
	std::cout << "Press and hold DOWN key to ROTATE the original square\n";
	std::cout << "Press and hold LEFT key to SCALE the original square\n";
	std::cout << "Press and hold UP key to SKEW the original square\n";
	std::cout << "---------------------------------------------------------\n";
	std::cout << "BLUE = TRANSLATION\n";
	std::cout << "RED = RATATION\n";
	std::cout << "GREEN = SCALED\n";
	std::cout << "PURPLE = SKEWED";


	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		
		// Specify the color of the background
		glClearColor(0.1f, 0.4f, 0.3f, 1.0f);
		// Clean the back buffer and assign the new color to it
		// We need glClear since we do not want drawings to persist in the background
		// In each frame, we want it to clear out to a color we chose
		glClear(GL_COLOR_BUFFER_BIT);
		
		// make the lines thicker
		glLineWidth(4.0f);

		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);
		// Bind the VAO so OpenGL knows to use it
		glBindVertexArray(VAO);


		// Draw the first square in black color
		// First, get the identity matrix with floating point values along the diagonal
		glm::mat4 transform = glm::mat4(1.0f);
		// Send the identity matrix (transform) to the vertex shader program
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
		// In the fragment shader, there is a field called "color". We want to give a value to that variable here to set the object color
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
		// Draw the triangle using the GL_TRIANGLES primitive, connecting the indices provided
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
		// Check if the user presses an action key
		// NOTE: all the translations take place if the user presses the action key
		processInput(window, shaderProgram);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();	
	}



	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// we can set many other input keys inside this function which will then be called in the rendering loop
void processInput(GLFWwindow* window, unsigned int shaderProgram)
{
	// ESC key will cause the window to close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// RIGHT key will cause the original square to be translated
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		// Draw the translated object in blue
		// As we have done above, get the identity matrix first
		glm::mat4 transform = glm::mat4(1.0f);
		// Let's apply the translation and update the transform matrix
		// Since the original matrix has a width of 0.4, 1.5 units to the right would mean 0.4*1.5 = 0.6 to the right
		transform = glm::translate(transform, glm::vec3(0.6f, 0.0f, 0.0f));
		// Now, we are ready to send this transform matrix into the vertex shader program
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));
		// Let's also give it a green color
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 0.0f, 1.0f, 1.0f);
		// Ready to draw the translated square
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// DOWN key will cause the original square to be rotated by 45 degrees
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		// Draw the rotated object in red
		glm::mat4 transform = glm::mat4(1);
		// Rotate the square by 45 degrees along the z-axis
		transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// Now, ready to send the transform matrix to the vertex shader program
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, value_ptr(transform));
		// Let's change this square's color
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), 1.0f, 0.0f, 0.0f, 0.0f);
		// Ready to draw it
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// LEFT key will cause the original square to be scaled
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		// Draw the scaled object in green
		glm::mat4 transform = glm::mat4(1);
		// scale the square by 1.5 (z-component stays at 1.0f since we are dealing with 2D object)
		transform = glm::scale(transform, glm::vec3(1.5f, 1.5f, 1.0f));
		// send the scaled transform matrix to the vertex shader
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, value_ptr(transform));
		// Change the color to green
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.0f, 1.0f, 0.0f, 1.0f);
		// Ready to draw the scaled square
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// UP key will cause the original squrare to be skewed
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

		// Lastly, draw the skewed square in purple
		glm::mat4 transform = glm::mat4(1);
		// Skew the square by 0.5 along the x-axis and 0.2 along the y-axis
		transform[1][0] = 0.5; // along x-axis ???
		transform[0][1] = 0.2; // along the y-axis ???
		//send the transforma matrix to the vertex shader program
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, value_ptr(transform));
		// change the color of the skewed square object
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), 0.5f, 0.0f, 0.5f, 1.0f);
		// Ready to draw the object
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

