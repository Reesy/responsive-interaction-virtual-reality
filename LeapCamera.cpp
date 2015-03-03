
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include <SOIL.h>

// Other includes
#include "Shader.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <LEAP/Leap.h>
#include <LEAP/LeapMath.h>
#include "LeapCamera.h"


using namespace Leap;


class SampleListener : public Listener {
public:
	virtual void onConnect(const Controller&);
	virtual void onFrame(const Controller&);
};

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
//	std::cout << "Frame available" << std::endl;
}


SampleListener listener;
Controller controller;
InteractionBox leapBox;
ImageList images;
Image LeftCam;
Image RightCam;
Frame frame;
//const unsigned char * image_buffer;
GLuint left_texture;
GLuint right_texture;
GLuint VBOs[2], VAOs[2], EBOs[2];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


LeapCamera::LeapCamera(){

	controller.setPolicy(Leap::Controller::POLICY_IMAGES);
	controller.addListener(listener);




}

LeapCamera::~LeapCamera(){

	


}
Vector leapToWorld(Vector leapPoint, InteractionBox iBox)
{
    leapPoint.z *= -1.0; //right-hand to left-hand rule
    Vector normalized = iBox.normalizePoint(leapPoint, false);
    normalized += Vector(0.5, 0, 0.5); //recenter origin
    return normalized * 100.0; //scale
}


void LeapCamera::run(){
		// Init GLFW
		glfwInit();
		// Set all the required options for GLFW
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		// Create a GLFWwindow object that we can use for GLFW's functions
		GLFWwindow* window = glfwCreateWindow(800, 640, "LeapCamera", nullptr, nullptr);
		glfwMakeContextCurrent(window);

		// Set the required callback functions
		glfwSetKeyCallback(window, key_callback);

		// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
		glewExperimental = GL_TRUE;
		// Initialize GLEW to setup the OpenGL Function pointers
		glewInit();

		// Build and compile our shader program
		Shader ourShader("/Users/JRees/Documents/workspace/RIVR/VertexShader.vert", "/Users/JRees/Documents/workspace/RIVR/FragmentShader.frag");

		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat left_eye_verticies[] = {
			// Positions            // Colors                  // Texture Coords
			0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 0.0f,           1.0f, 1.0f, // Top Right
			0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.0f,           1.0f, 0.0f, // Bottom Right
		   -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 0.0f,           0.0f, 0.0f, // Bottom Left
		   -0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 0.0f,           0.0f, 1.0f  // Top Left 
		};
		GLuint left_eye_indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};
		

		GLfloat right_eye_verticies[] = {
			// Positions            // Colors                  // Texture Coords
			0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
			0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom Left
			-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // Top Left 
		};
		GLuint right_eye_indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};



		//OpenGL set up code
		
		glEnable(GL_TEXTURE_2D);

		glGenVertexArrays(2, VAOs);
		glGenBuffers(2, VBOs);
		glGenBuffers(2, EBOs);

		//Left Eye set up
		glBindVertexArray(VAOs[0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(left_eye_verticies), left_eye_verticies, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(left_eye_indices), left_eye_indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

		
		
		//Right Eye set up
		glBindVertexArray(VAOs[1]);

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(right_eye_verticies), right_eye_verticies, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(right_eye_indices), right_eye_indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		// Load and create a texture and fills it with test data. 
		//	GLuint texture;
		glGenTextures(1, &left_texture);
		glBindTexture(GL_TEXTURE_2D, left_texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load image, create texture and generate mipmaps
		int width, height;
     	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	

		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		//glTexImage2D(GL_TEXTURE_2D, 0, 0, GL_RGB, LeftCam.width(), LeftCam.height(), GL_RGB, GL_UNSIGNED_BYTE, LeftCam.data());
		glGenerateMipmap(GL_TEXTURE_2D);


		// Game loop
		while (!glfwWindowShouldClose(window))
		{
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Render
			// Clear the colorbuffer
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			

			update();

			//Bind texture
			glBindTexture(GL_TEXTURE_2D, left_texture);

			// Activate shader
			ourShader.Use();




			// Draw left eye
			glBindVertexArray(VAOs[0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

	


		
			// Swap the screen buffers
			glfwSwapBuffers(window);

		}

		glfwTerminate();


}


void LeapCamera::render(){



}

void LeapCamera::update(){
	controller.setPolicyFlags(Controller::POLICY_IMAGES);

	Frame frame = controller.frame();
	if (frame.isValid()) {
		//Update image and distortion textures
		Image left = frame.images()[0];
		if (left.width() > 0) {
			glBindTexture(GL_TEXTURE_2D, left_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, left.width(), left.height(), 0, GL_RED, GL_UNSIGNED_BYTE, left.data());
		
		
		}
	
	}
    
    HandList hands = frame.hands();
    
    Hand firstHand = hands[0];
    
    Vector position = firstHand.palmPosition();
    
    InteractionBox mybox = controller.frame().interactionBox();
    
    Vector normalisedPosition = leapToWorld(position, mybox);
    
    
  //  std::cout << "Position of detected hand: " << position << std::endl;
    std::cout << "Normalised position: " << normalisedPosition << std::endl;
}



void LeapCamera::CreateTestImage(){


}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
		SOIL_save_image("TestFile.bmp", 1, RightCam.width(), RightCam.height(), 1, LeftCam.data());
		std::cout << "Image saved!" << std::endl;
		glBindTexture(GL_TEXTURE_2D, left_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LeftCam.width(), LeftCam.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, LeftCam.data());
	}

}