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
#include <glm/gtc/quaternion.hpp>
#include <GLM/gtx/quaternion.hpp>
#include <LEAP/Leap.h>
#include <LEAP/LeapMath.h>
#include "TextureLoader.h"
#include "Model.h"
#include "HandModelLoader.h"
#include "KeyboardModelLoader.h"
#include "SceneObjects.h"


//#include "Model.h"

using namespace Leap;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void collision_detection();
void leapTest();
float normalise(float currentRangeA, float currentRangeB, float newRangeA, float newRangeB, float inputValue);
const GLuint WIDTH = 800, HEIGHT = 600;

glm::mat4 rotationbyquat(float x, float y, float z);
glm::vec3 properHandPosition(Leap::Vector inputCoords);
glm::vec3 properPosition(Leap::Vector inputCoords);


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

float viewX, viewY, viewZ;

glm::vec3 EulerAngles;
glm::mat4 rotMat;

float pitch, yaw, roll, handX, handY, handZ;
float thumbX, thumbY, thumbZ;
float indexX, indexY, indexZ;

//used for normalization of leap co-ords into scene co-ords
int lowerPos = -200;        // leap lower range
int higherPos = 200;        // leap upper range
int lowerRange = -1;        //normalized lower range
int higherRange = 1;        //normalized upper range

float inValNorm, aUpper, normPos, bUpperNorm, bValNorm;

Vector palmTranslation;
glm::mat4 palmRotation;

float modelRotX, palmAngle, handRoll;
bool CameraMove = false;
Leap::Matrix rotationMatrix;
Vector thumbTranslation;
float mixAmount = 0.5;
Vector palmNormal;
//forward declarations
void update();
void render();
SceneObjects handObj;

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
    
    
    
    handObj.setPosition(glm::vec3(0, 0, 0));
    
    Model palm("Resources/Models/hand/palm.obj");
    Model finger("Resources/Models/hand/bone.obj");
    Model fingerTip("Resources/Models/hand/boneTip.obj");
    
    
    // Build and compile our shader program
    Shader ourShader("Resources/Shaders/VertexShader.vert", "Resources/Shaders/FragmentShader.frag");
    
    Shader letterShader("Resources/Shaders/KeyVertexShader.vert", "Resources/Shaders/KeyFragmentShader.frag");
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
         };
    
    HandModelLoader custom(vertices, ourShader);
    KeyBoardModelLoader keyboard(vertices, letterShader);
   
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
    
    // Creates an identity quaternion (no rotation)
    glm::quat MyQuaternion;
    
    
    handObj.setScale(glm::vec3(1, 0.25, 1));
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
        
        // Activate shader
        ourShader.Use();
     
        //resets matricies to identity.
        
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        
        projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
        
        // Define camera uniforms for shader.
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
      
        
        view = glm::translate(view, glm::vec3(viewX, viewY, viewZ));
        leapTest();
        
        // Draw container
        
        for(GLuint i = 0; i < 3; i++)
        {
            glm::mat4 model; //resets model matrix to identify matrix
            model = glm::translate(model, handObj.getPosition());
            if(i == 0){  // Translations done to palm
                
                model = model * rotationbyquat(handObj.getRotation().x, handObj.getRotation().y, handObj.getRotation().z);
            
            
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                
                
                palm.Draw(ourShader);
            
            
            
            
            
            
            }else if(i == 1){  //Translations done to thumb
                model = glm::translate(model, glm::vec3(thumbX, thumbY, thumbZ));
                
                
                
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                
                
                fingerTip.Draw(ourShader);
            
            }else if(i == 2){
                model = glm::translate(model, glm::vec3(indexX, indexY, indexZ));
               
                
                
                
                
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                
                
                fingerTip.Draw(ourShader);
           
            }
            else{
               
            }
        
            
        }
        
        keyboard.Draw();
        
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
   
 
  //  Bone firstBone = firstHand.fingers()[0].bone(boneType);
    
    palmRotation = firstHand.rotationMatrix(previousFrame).toMatrix4x4<glm::mat4>();
    
    //used for rotation/orientation of the hand
    pitch = firstHand.direction().pitch();
    yaw = firstHand.direction().yaw();
    roll = firstHand.palmNormal().roll();
    
    handObj.setPosition(glm::vec3(properHandPosition(firstHand.palmPosition()).x, properHandPosition(firstHand.palmPosition()).y, properHandPosition(firstHand.palmPosition()).z));
    
    handObj.setRotation(glm::vec3(pitch, -yaw, roll));
    thumbX = properPosition(firstHand.fingers()[0].tipPosition()).x;
    thumbY = properPosition(firstHand.fingers()[0].tipPosition()).y;
    thumbZ = properPosition(firstHand.fingers()[0].tipPosition()).z;
    
    
    
    
    
    
    
}

glm::vec3 properHandPosition(Leap::Vector inputCoords){
    return glm::vec3(normalise(lowerPos, higherPos, lowerRange, higherRange, inputCoords.x) * 10, normalise(lowerPos, higherPos, lowerRange, higherRange, inputCoords.y - 200) * 10, normalise(lowerPos, higherPos, lowerRange, higherRange, inputCoords.z) * 10);
}
glm::vec3 properPosition(Leap::Vector inputCoords){
    return glm::vec3(normalise(-100, 100, lowerRange, higherRange, inputCoords.x), normalise(-100, 100, lowerRange, higherRange, inputCoords.y - 200), normalise(-100, 100, lowerRange, higherRange, inputCoords.z));
}
float normalise(float currentRangeA, float currentRangeB, float newRangeA, float newRangeB, float inputValue){
    inValNorm = inputValue - currentRangeA;
    aUpper = currentRangeB - currentRangeA;
    normPos = inValNorm / aUpper;
    
    bUpperNorm = newRangeB - newRangeA;
    bValNorm = normPos * bUpperNorm;
    
    return newRangeA + bValNorm;
}
glm::mat4 rotationbyquat(float x, float y, float z){
    
    // Creates an identity quaternion (no rotation)
    glm::quat MyQuaternion;
    
    // Conversion from Euler angles (in radians) to Quaternion
    EulerAngles.x = x;
    EulerAngles.y = y;
    EulerAngles.z = z;

    MyQuaternion = glm::quat(EulerAngles);
    rotMat = glm::toMat4(MyQuaternion);
    
    return rotMat;
}

void collisionDetection(){
    
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
      
    }
}