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
#include "TextureLoader.h"
#include "Model.h"

//#include "Model.h"

using namespace Leap;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void leapTest();
const GLuint WIDTH = 800, HEIGHT = 600;

class SampleListener : public Listener {
public:
    virtual void onConnect(const Controller&);
    virtual void onFrame(const Controller&);
};

void SampleListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
    //  std::cout << "Frame available" << std::endl;
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

float viewX;
float viewY;
float viewZ;




float pitch;
float yaw;
float roll;


float modelX;
float modelY;
float modelZ;
Vector palmTranslation;
glm::mat4 palmRotation;
float modelRotX;
float palmAngle;
float handRoll;
bool CameraMove = false;
Leap::Matrix rotationMatrix;
Vector thumbTranslation;
float mixAmount = 0.5;
Vector palmNormal;
//forward declarations
void update();
void render();

// The MAIN function, from here we start the application and run the game loop
int main()
{
    
    controller.setPolicy(Leap::Controller::POLICY_IMAGES);
    controller.addListener(listener);
    
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    //usedfor double buffering
    glfwSwapInterval(1);
    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    
    // Set this to true so GLEW knows to use a modern approach to retrieving fsunction pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();
    glEnable(GL_DEPTH_TEST);
    
    int windowWidth, windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight); // Mac specific, used for fixing NDC
    
    // Define the viewport dimensions
    glViewport(0, 0, windowWidth, windowHeight);
    
    // Build and compile our shader program
    Shader ourShader("Resources/Shaders/VertexShader.vert", "Resources/Shaders/FragmentShader.frag");

    
    Model suitModel("Resources/Models/nanosuit/nanosuit.obj");
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), //hand
        glm::vec3(-1.0f,  0.0f, -0.25f), //thumb
        glm::vec3(-0.5f,  0.0f, -1.0f), //index finger
        glm::vec3( 0.0f,  0.0f, -1.0f), //middle
        glm::vec3( 0.5f,  0.0f, -1.0f), //ring finger
        glm::vec3( 1.0f,  0.0f, -0.80f), //pinky
        
    };
    
    GLfloat letterVerticies[] = {
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f
    };
    
    

    
    
    GLuint VBOs[2], VAOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    
    
   
    
    //Crate-cube
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0); // Unbind VAO
    
    
    //letters
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letterVerticies), letterVerticies, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0); // Unbind VAO
    

    int width, height;
    
    unsigned char* image = SOIL_load_image("Resources/Textures/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    SOIL_free_image_data(image);

    TextureLoader myTexture;
    myTexture.Generate(width, height, image);

    image = SOIL_load_image("Resources/Textures/awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
    SOIL_free_image_data(image);

    TextureLoader myTexture2;
    myTexture2.Generate(width, height, image);
    
    viewX = 0.0;
    viewY = 0.0;
    viewZ = -3.0;
    modelX = 0.0;
    modelY = 0.0;
    modelZ = 0.0;
    
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        // Bind Texture
        glActiveTexture(GL_TEXTURE0);
        myTexture.Bind();
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
        
        glActiveTexture(GL_TEXTURE1);
        myTexture2.Bind();
        glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
        
        // Activate shader
        ourShader.Use();
        
        GLint mixUniformLocation = glGetUniformLocation(ourShader.Program, "mixVal");
        glUniform1f(mixUniformLocation, mixAmount);
        
        //resets matricies to identity.
        glm::mat4 trans;
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        
        projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
        
        //Define camera uniforms for shader.
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
        
        view = glm::translate(view, glm::vec3(viewX, viewY, viewZ));
        leapTest();
        
        // Draw container
        glBindVertexArray(VAOs[0]);
        for(GLuint i = 0; i < 6; i++)
        {
            glm::mat4 model; //resets model matrix to identify matrix
            model = glm::translate(model, cubePositions[i]);
            
            if(i == 0){  // Translations done to palm
                model = glm::translate(model, glm::vec3(modelX, modelY, modelZ));
                model = glm::rotate(model,  -yaw, glm::vec3(0, 1, 0));
                model = glm::rotate(model, roll , glm::vec3(0, 0, 1));
                model = glm::rotate(model, pitch, glm::vec3(1, 0, 0));
                model = glm::scale(model, glm::vec3(1, 0.25, 1));
            }else if(i == 1){  //Translations done to thumb
                model = glm::translate(model, glm::vec3(modelX, modelY, modelZ));
                model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
            }
            else{  //Translations done to other fingers
                model = glm::translate(model, glm::vec3(modelX, modelY, modelZ));
                model = glm::scale(model, glm::vec3(0.25, 0.25, 0.25));
            }
            
            //Send uniforms to shader
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);
        
        
        suitModel.Draw(ourShader);
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void render(){
    
}


void update(){
    
    
    
    
}
void leapTest(){
    
    controller.setPolicyFlags(Controller::POLICY_IMAGES);
    
    Frame frame = controller.frame();
    Frame previousFrame = controller.frame(1);
    HandList hands = frame.hands();
    
    Hand firstHand = hands[0];
    
    palmTranslation = firstHand.translation(previousFrame);
    palmRotation = firstHand.rotationMatrix(previousFrame).toMatrix4x4<glm::mat4>();
    
    // handRoll = firstHand.rotationAngle(previousFrame);
    handRoll += firstHand.rotationAngle(previousFrame, Vector(0, 0, 1));
    
    palmNormal = firstHand.palmNormal();
    
    pitch = firstHand.direction().pitch();
    yaw = firstHand.direction().yaw();
    roll = firstHand.palmNormal().roll();
    
    std::cout << pitch << std::endl;
    
    //std::cout << palmNormal << std::endl;
    modelX += palmTranslation.x / 10;
    modelY += palmTranslation.y / 10;
    modelZ += palmTranslation.z / 10;
    
}


// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
      
    }

}