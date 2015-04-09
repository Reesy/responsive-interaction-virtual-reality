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

#include "SceneObjects.h"
#include "KeyObjects.h"

//#include "Model.h"

using namespace Leap;

//forward declarations
void update();
void render();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void collision_detection(KeyObjects objA, SceneObjects objB);
void leapUpdate();
void update();
void generateScene(Shader ourShader);
float normalise(float currentRangeA, float currentRangeB, float newRangeA, float newRangeB, float inputValue);
float worldToScreen(float inVal);
glm::mat4 rotationbyquat(float x, float y, float z);
glm::vec3 properHandPosition(Leap::Vector inputCoords);
glm::vec3 properPosition(Leap::Vector inputCoords);
glm::quat CreateQuat(float pitch, float yaw, float roll);



//Leap motion listener

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

//variable declarations

const GLuint WIDTH = 800, HEIGHT = 600;
SampleListener listener;
Controller controller;
ImageList images;
Image LeftCam;
Image RightCam;
Frame frame;

GLuint left_texture;
GLuint right_texture;

glm::vec3 EulerAngles;
glm::mat4 rotMat;
glm::mat4 palmRotation;

float viewX, viewY, viewZ;
float pitch, yaw, roll, handX, handY, handZ;
float thumbPitch, thumbYaw;
float finger1Pitch, finger1Yaw;
float finger2Pitch, finger2Yaw;
float finger3Pitch, finger3Yaw;
float finger4Pitch, finger4Yaw;
float thumbX, thumbY, thumbZ;
float indexX, indexY, indexZ;
float middleX, middleY, middleZ;
float modelRotX, palmAngle, handRoll;
float testx, testy, testz;

bool CameraMove = false;
Leap::Matrix rotationMatrix;
Vector thumbTranslation;
Vector palmNormal;
Vector palmTranslation;

SceneObjects handObj;
SceneObjects thumbObj;
SceneObjects finger1Obj;
SceneObjects finger2Obj;
SceneObjects finger3Obj;
SceneObjects finger4Obj;

KeyObjects keyA, keyB, keyC, keyD, keyE, keyF, keyG, keyH, keyI, keyJ, keyK, keyL,
keyM, keyN, keyO,keyP, keyQ, keyR, keyS, keyT, keyU, keyV, keyW, keyX, keyY, keyZ;

KeyObjects KeyBoard[26] = {keyA, keyB, keyC, keyD, keyE, keyF, keyG, keyH, keyI, keyJ, keyK, keyL,
    keyM, keyN, keyO,keyP, keyQ, keyR, keyS, keyT, keyU, keyV, keyW, keyX, keyY, keyZ};

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
    
    // creats models
    Model palm("Resources/Models/hand/palm.obj");
    Model finger("Resources/Models/hand/bone.obj");
    Model fingerTip("Resources/Models/hand/boneTip.obj");
    
    // Build and compile our shader program
    Shader ourShader("Resources/Shaders/VertexShader.vert", "Resources/Shaders/FragmentShader.frag");

    HandModelLoader custom(ourShader);
    generateScene(ourShader);
    
    //this sets the camera
    viewX = 0.0;
    viewY = 0.0;
    viewZ = -3.0;
    
    // Creates an identity quaternion (no rotation)
    glm::quat MyQuaternion;
    // Define camera uniforms for shader.
    GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
    GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
    GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
    glm::mat4 test;
   
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        ourShader.Use();
     
        //resets matricies to identity.
        
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        glm::vec4 worldCoords;
        
        projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
  
        view = glm::translate(view, glm::vec3(viewX, viewY, viewZ));
       
        
        update();
       
        
        for(GLuint i = 0; i < 6; i++)
        {
            glm::mat4 model; //resets model matrix to identify matrix

            if(i == 0){  // Translations done to palm
                model = glm::translate(model, handObj.getPosition());
            //    model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
           
                worldCoords = model * glm::vec4(0, 0, 0, 1);
             //   std::cout << worldCoords.x << std::endl;
                model = model * glm::toMat4(CreateQuat(-yaw, -pitch, -roll));
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                
                palm.Draw(ourShader);
                fingerTip.Draw(ourShader);
            }else if(i == 1){  //Translations done to thumb
                model = glm::translate(model, thumbObj.getPosition());
          //      model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
                model = model * glm::toMat4(CreateQuat(-thumbYaw, -thumbPitch, -roll));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                fingerTip.Draw(ourShader);
            }else if(i == 2){
                model = glm::translate(model, finger1Obj.getPosition());
              //  model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
                model = model * glm::toMat4(CreateQuat(-finger1Yaw, -finger1Pitch, -roll));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                fingerTip.Draw(ourShader);
            }else if ( i == 3 ){
                model = glm::translate(model, finger2Obj.getPosition());
               // model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
                model = model * glm::toMat4(CreateQuat(-finger2Yaw, -finger2Pitch, -roll));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                fingerTip.Draw(ourShader);
            }else if(i == 4){
                model = glm::translate(model, finger3Obj.getPosition());
               // model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
                model = model * glm::toMat4(CreateQuat(-finger3Yaw, -finger3Pitch, -roll));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                fingerTip.Draw(ourShader);
            }else if(i == 5){
                model = glm::translate(model, finger4Obj.getPosition());
               // model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
                model = model * glm::toMat4(CreateQuat(-finger4Yaw, -finger4Pitch, -roll));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                fingerTip.Draw(ourShader);
            }
        }
        //keyA.setPosition(glm::vec3(1, 1, 1));
        keyA.Draw(view, projection);
      //  std::cout << keyA.getTestPosition().x << " " << keyA.getTestPosition().y << " " << keyA.getTestPosition().z <<  std::endl;
        //  keyB.Draw(view, projection);
       // keyC.Draw(view, projection);
        //keyX.Draw();
        
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
    
    //calls leapMotion update
    leapUpdate();

    collision_detection(keyA, handObj);

}
/* This checks to see if the object of the first parameter collides with the object of the second parameter */
void collision_detection(KeyObjects objA, SceneObjects objB){
    
    
    //obja is keyA //objb us handObj
    
    if(objB.getPosition().x > objA.getAABB().xmin && objB.getPosition().x < objA.getAABB().xmax &&
       objB.getPosition().y > objA.getAABB().ymin && objB.getPosition().y < objA.getAABB().ymax &&
       objB.getPosition().z > objA.getAABB().zmin && objB.getPosition().z < objA.getAABB().zmax)
    {
        std::cout << objA.getKey() << std::endl;
    }else{
       // std::cout << "OUTSIDE " << std::endl;
        std::cout << objA.getKey() << std::endl;
      
    }
    
}

//This returns a value from the rift and maps it to the screen space
float worldToScreen(float xLeap){
    float leapStart = -200.0;
    float leapEnd = 200.0;
    float AppStart = -2.0;
    float AppEnd = 2.0;
    float leapRange = leapEnd - leapStart;
    float AppRange  = AppEnd - AppStart;

    float firstResult = xLeap - leapStart;
    float secondResult = AppRange / leapRange ;
    
    float finalResult = firstResult * secondResult + AppStart;

    return finalResult;
}

void leapUpdate(){
    
    controller.setPolicyFlags(Controller::POLICY_IMAGES);
    
    Frame frame = controller.frame();
    Frame previousFrame = controller.frame(1);
    InteractionBox myBox = frame.interactionBox();
    HandList hands = frame.hands();
    
    Hand firstHand = hands[0];
    
    //used for rotation/orientation of the hand
    pitch = firstHand.direction().pitch();
    yaw = firstHand.direction().yaw();
    roll = firstHand.palmNormal().roll();
    
    thumbPitch = firstHand.fingers()[0].direction().pitch();
    thumbYaw = firstHand.fingers()[0].direction().yaw();
    //Use this code for finger orientation, perhaps abstract to class
    
    finger1Pitch = firstHand.fingers()[1].direction().pitch();
    finger1Yaw = firstHand.fingers()[1].direction().yaw();

    finger2Pitch = firstHand.fingers()[2].direction().pitch();
    finger2Yaw = firstHand.fingers()[2].direction().yaw();
    
    finger3Pitch = firstHand.fingers()[3].direction().pitch();
    finger3Yaw = firstHand.fingers()[3].direction().yaw();
    
    finger4Pitch = firstHand.fingers()[4].direction().pitch();
    finger4Yaw = firstHand.fingers()[4].direction().yaw();
    
    handObj.setPosition(glm::vec3(worldToScreen(firstHand.palmPosition().x), worldToScreen(firstHand.palmPosition().y - 200), worldToScreen(firstHand.palmPosition().z)));
    
    thumbObj.setPosition(glm::vec3(worldToScreen(firstHand.fingers()[0].tipPosition().x), worldToScreen(firstHand.fingers()[0].tipPosition().y - 200), worldToScreen(firstHand.fingers()[0].tipPosition().z)));
    
    finger1Obj.setPosition(glm::vec3(worldToScreen(firstHand.fingers()[1].tipPosition().x), worldToScreen(firstHand.fingers()[1].tipPosition().y - 200), worldToScreen(firstHand.fingers()[1].tipPosition().z)));
    
    finger2Obj.setPosition(glm::vec3(worldToScreen(firstHand.fingers()[2].tipPosition().x), worldToScreen(firstHand.fingers()[2].tipPosition().y - 200), worldToScreen(firstHand.fingers()[2].tipPosition().z)));
    
    finger3Obj.setPosition(glm::vec3(worldToScreen(firstHand.fingers()[3].tipPosition().x), worldToScreen(firstHand.fingers()[3].tipPosition().y - 200), worldToScreen(firstHand.fingers()[3].tipPosition().z)));
    
    finger4Obj.setPosition(glm::vec3(worldToScreen(firstHand.fingers()[4].tipPosition().x), worldToScreen(firstHand.fingers()[4].tipPosition().y - 200), worldToScreen(firstHand.fingers()[4].tipPosition().z)));
    
    //this resets the AABB to the current position of the hand.
    handObj.setAABB(1);
    
}

glm::quat CreateQuat(float inPitch, float inYaw, float inRoll){
    
    float fSinPitch(sin(inPitch*0.5f));
    float fCosPitch(cos(inPitch*0.5f));
    float fSinYaw(sin(inYaw*0.5f));
    float fCosYaw(cos(inYaw*0.5f));
    float fSinRoll(sin(inRoll*0.5f));
    float fCosRoll(cos(inRoll*0.5f));
    float fCosPitchCosYaw(fCosPitch*fCosYaw);
    float fSinPitchSinYaw(fSinPitch*fSinYaw);
    
    float X = fSinRoll * fCosPitchCosYaw     - fCosRoll * fSinPitchSinYaw;
    float Y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
    float Z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
    float W = fCosRoll * fCosPitchCosYaw     + fSinRoll * fSinPitchSinYaw;
    
    glm::quat myQuat(X, Y, Z, W);
    
    return myQuat;
    
}

void generateScene(Shader ourShader){

    //generates keyboards
    keyA.setShaderUniforms(ourShader);
    keyA.setPosition(glm::vec3(1, 1, -3));
    keyA.setAABB(1); //Must be called after position;
    keyA.setRotation(glm::vec3(1, 0, 0));
    keyA.setScale(glm::vec3(1, 1, 1));
    keyA.setKey('a', 'A');
    keyA.setCase(true);
    keyA.setLowerImage("Resources/Textures/letterTexture/alphanum_lowercase-letter-a_simple-black_512x512.png");
    keyA.setUpperImage("Resources/Textures/letterTexture/alphanum_uppercase-letter-a_simple-black_512x512.png");
    keyA.generate();
    
   /*
    keyB.setShaderUniforms(ourShader);
    
   // keyB.setPosition(glm::vec3(-0.7, 0, -8));
    keyB.setRotation(glm::vec3(1, 0, 0));
    keyB.setScale(glm::vec3(0.6, 0.6, 0.6));
    keyB.generate("Resources/Textures/letterb.png");
    
  //  keyC.setShaderUniforms(ourShader);
    keyC.setPosition(glm::vec3(0.7, 0, -8));
    keyC.setRotation(glm::vec3(1, 0, 0));
    keyC.setScale(glm::vec3(0.6, 0.6, 0.6));
    keyC.generate("Resources/Textures/letterb.png");
    
    keyX.setShaderUniforms(ourShader);
    
    keyX.setPosition(glm::vec3(0, 0, -8));
    keyX.setRotation(glm::vec3(1, 0, 0));
    keyX.setScale(glm::vec3(0.6, 0.6, 0.6));
    keyX.generate("Resources/Textures/letterx.png");
    
    */
    
    
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
        keyA.isUpper = !keyA.isUpper;
         keyA.update();
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        testz += 0.1;
        keyA.setPosition(glm::vec3(keyA.getPosition().x, keyA.getPosition().y, keyA.getPosition().z + testz));
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        testz -= 0.1;
        keyA.setPosition(glm::vec3(keyA.getPosition().x, keyA.getPosition().y, keyA.getPosition().z + testz));
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        
    }
}