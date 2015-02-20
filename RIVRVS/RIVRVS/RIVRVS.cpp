// glfwTest2VS.cpp : main project file.


#include "stdafx.h"
#include <iostream>
//using namespace std;

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include "Shader.h"
#include "SerialClass.h"
#include "Leap.h"
// GLFW
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

using namespace Leap;
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void arduinoTest();
void leapTest();





class SampleListener : public Leap::Listener {
public:
	virtual void onConnect(const Leap::Controller&);
	virtual void onFrame(const Leap::Controller&);
};

void SampleListener::onConnect(const Leap::Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onFrame(const Leap::Controller& controller) {
	//std::cout << "Frame available" << std::endl;
}
SampleListener listener;

Controller controller;
// The MAIN function, from here we start our application and run our Program/Game loop
int main()
{


	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "RIVRVS", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, 800, 600);

	Shader ourShader("VertexShader.vert", "FragmentShader.frag");
	// Set up our vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		//positions                 //colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	

	controller.setPolicy(Leap::Controller::POLICY_IMAGES);
	controller.addListener(listener);
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activates shader
		ourShader.Use();
		GLfloat yOffset = 0.5f;
		glUniform1f(glGetUniformLocation(ourShader.Program, "yOffset"), 1.0f);

		//Draw the triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
void arduinoTest(){
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM3");    // adjust as needed

	if (SP->IsConnected()){
		std::cout << "We're connected" << std::endl;
	}
	else{
		std::cout << "Failed to connect" << std::endl;
	}

	SP->WriteData("3", 256);


}

void leapTest(){

	Frame frame = controller.frame();

	ImageList images = frame.images();

	Image image = images[0];

	const unsigned char* image_buffer = image.data();
//	std::cout << "Image" << 0 << " : " << image.width() << std::endl;
	std::cout << image << std::endl;
	if (image.width() > 1){
		std::cout << "width : " << image.width() << "  height: " << image.height() << std::endl;
		SOIL_save_image("TestImage.bmp",1, image.width(), image.height(),1, image_buffer);
	}

}
// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		
		//arduinoTest();
		leapTest();
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS){

	}
}
