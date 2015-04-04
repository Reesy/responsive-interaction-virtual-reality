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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void collision_detection();
void leapTest();
void update();
void generateScene(Shader ourShader);
float normalise(float currentRangeA, float currentRangeB, float newRangeA, float newRangeB, float inputValue);
Vector leapToWorld(Vector leapPoint, InteractionBox iBox);
float worldToScreen(float inVal);
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
float thumbPitch, thumbYaw;
float finger1Pitch, finger1Yaw;
float thumbX, thumbY, thumbZ;
float indexX, indexY, indexZ;
float middleX, middleY, middleZ;

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
SceneObjects thumbObj;
SceneObjects finger1Obj;


float testx, testy, testz;

KeyObjects keyA, keyB, keyC, keyD, keyE, keyF, keyG, keyH, keyI, keyJ, keyK, keyL,
keyM, keyN, keyO,keyP, keyQ, keyR, keyS, keyT, keyU, keyV, keyW, keyX, keyY, keyZ;

glm::quat CreateQuat(float pitch, float yaw, float roll);

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
    
    
    
    handObj.setPosition(glm::vec3(testx, testy, testz));
  
    Model palm("Resources/Models/hand/palm.obj");
    Model finger("Resources/Models/hand/bone.obj");
    Model fingerTip("Resources/Models/hand/boneTip.obj");
    
    
    // Build and compile our shader program
    Shader ourShader("Resources/Shaders/VertexShader.vert", "Resources/Shaders/FragmentShader.frag");
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,   0.0f, -0.0f,
        0.5f, -0.5f, -0.5f,    1.0f, -0.0f,
        0.5f,  0.5f, -0.5f,    1.0f, -1.0f,
        0.5f,  0.5f, -0.5f,    1.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, -0.0f,
    
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


    HandModelLoader custom(vertices, ourShader);
    generateScene(ourShader);
    
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
    
    test.length();
    
   // std::cout << test[1] << std::endl;
    handObj.setScale(glm::vec3(1, 0.25, 1));
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
        glm::vec4 myTest(0, 0, 0, 1);
        glm::vec4 worldCoords;
        glm::vec4 myView(viewX, viewY, viewZ, 1);
        
        projection = glm::perspective(45.0f, (GLfloat)800 / (GLfloat)600, 0.1f, 100.0f);
  
        view = glm::translate(view, glm::vec3(viewX, viewY, viewZ));
       // leapTest();
        
        update();
      //  collision_detection();
        /*
        for(GLuint i = 0; i < 4; i++)
        {
            glm::mat4 model; //resets model matrix to identify matrix
         //   model = glm::translate(model, handObj.getPosition());
           // glm::mat4 worldCoords;
          
            
            
            
            if(i == 0){  // Translations done to palm
                model = glm::translate(model, handObj.getPosition());
              //  worldCoords = myView * model;
             //   worldCoords = model * myView;
                worldCoords = model * myTest;
                model = model * glm::toMat4(CreateQuat(-yaw, -pitch, -roll));
                
               
                std::cout << worldCoords.x << " " << worldCoords.y << " " << worldCoords.z << std::endl;
               // std::cout << worldCoords << std::endl;
              
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                
                palm.Draw(ourShader);
                fingerTip.Draw(ourShader);

            }else if(i == 1){  //Translations done to thumb
                model = glm::translate(model, thumbObj.getPosition());
                
                model = model * glm::toMat4(CreateQuat(-thumbYaw, -thumbPitch, -roll));
                
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
                
                fingerTip.Draw(ourShader);
            
            }else if(i == 2){
                glm::mat4 model;
                model = glm::translate(model, glm::vec3(indexX, indexY, indexZ));
                
                model = model * glm::toMat4(CreateQuat(-finger1Yaw, -finger1Pitch, -roll));
                
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
           
                fingerTip.Draw(ourShader);
           
            }
            else if ( i == 3 ){
                glm::mat4 model;
                model = glm::translate(model, glm::vec3(middleX, middleY, middleZ));

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
                fingerTip.Draw(ourShader);
                
            }
           
        }
             
        */
        //keyA.setPosition(glm::vec3(1, 1, 1));
        keyA.Draw(view, projection);
        std::cout << keyA.getTestPosition().x << " " << keyA.getTestPosition().y << " " << keyA.getTestPosition().z <<  std::endl;
        keyB.Draw(view, projection);
        keyC.Draw(view, projection);
        //keyX.Draw();
     //   keyboard.Draw();
        
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
        //this wil be used temporarily to test for collision
     // std::cout << finger1Obj.getPosition().x << " " << finger1Obj.getPosition().y <<  " " << finger1Obj.getPosition().z << std::endl;
    
   // std::cout << keyA.getPosition().x << " " << finger1Obj.getPosition().x << std::endl;
   // std::cout << leapWorld(-210.0) << std::endl;
    

}

//This returns a value from the rift and maps it to the screen space
float worldToScreen(float xLeap){
    float leapStart = -200.0;
    float leapEnd = 200.0;
    float AppStart = -1.0;
    float AppEnd = 1.0;
    float leapRange = leapEnd - leapStart;
    float AppRange  = AppEnd - AppStart;

    float firstResult = xLeap - leapStart;
    float secondResult = AppRange / leapRange ;
    
    float finalResult = firstResult * secondResult + AppStart;
    
    if(finalResult > AppEnd){
       return AppEnd;
    }else if(finalResult < AppStart){
      return AppStart;
    }
    
    return finalResult;
    
    /*
     Xapp = (xleap - LeapStart) * (Leaprange / AppRange + AppStart)
     LeapRange = LeapEnd - LeapStart
     AppRange = AppEnd - AppStart
     */
}

Vector leapToWorld(Vector leapPoint, InteractionBox iBox)
{
    leapPoint.z *= -1.0; //right-hand to left-hand rule
    Vector normalized = iBox.normalizePoint(leapPoint, false);
    normalized += Vector(0.5, 0, 0.5); //recenter origin
    return normalized * 100.0; //scale
}
void leapTest(){
    
    controller.setPolicyFlags(Controller::POLICY_IMAGES);
    
    Frame frame = controller.frame();
    Frame previousFrame = controller.frame(1);
    InteractionBox myBox = frame.interactionBox();
    HandList hands = frame.hands();
    
    Hand firstHand = hands[0];
    
    
 
  //  Bone firstBone = firstHand.fingers()[0].bone(boneType);
    
    palmRotation = firstHand.rotationMatrix(previousFrame).toMatrix4x4<glm::mat4>();
    
    //used for rotation/orientation of the hand
    pitch = firstHand.direction().pitch();
    yaw = firstHand.direction().yaw();
    roll = firstHand.palmNormal().roll();
    
    thumbPitch = firstHand.fingers()[0].direction().pitch();
    thumbYaw = firstHand.fingers()[0].direction().yaw();
    //Use this code for finger orientation, perhaps abstract to class
    
    finger1Pitch = firstHand.fingers()[1].direction().pitch();
    finger1Yaw = firstHand.fingers()[1].direction().yaw();

    //std::cout << worldToScreen(firstHand.palmPosition().x) << std::endl;
    
    
    
    handObj.setPosition(glm::vec3(worldToScreen(firstHand.palmPosition().x), worldToScreen(firstHand.palmPosition().y - 200), worldToScreen(firstHand.palmPosition().z)));
   // thumbX = properPosition(firstHand.fingers()[0].tipPosition()).x;
    //thumbY = properPosition(firstHand.fingers()[0].tipPosition()).y;
   // thumbZ = properPosition(firstHand.fingers()[0].tipPosition()).z;
    
    //thumbObj.setPosition(glm::vec3(thumbX, thumbY, thumbZ));
    
    
    
    //indexX = properPosition(firstHand.fingers()[1].tipPosition()).x;
    //indexY = properPosition(firstHand.fingers()[1].tipPosition()).y;
    //indexZ = properPosition(firstHand.fingers()[1].tipPosition()).z;
    
   // finger1Obj.setPosition(glm::vec3(indexX, indexY, indexZ));
    //middleX = properPosition(firstHand.fingers()[2].tipPosition()).x;
   // middleY = properPosition(firstHand.fingers()[2].tipPosition()).y;
    //middleZ = properPosition(firstHand.fingers()[2].tipPosition()).z;
    
    
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
    
    keyA.setPosition(glm::vec3(0, 0, -8));
    keyA.setRotation(glm::vec3(1, 0, 0));
    keyA.setScale(glm::vec3(0.6, 0.6, 0.6));
    keyA.generate("Resources/Textures/lettera.png");
    
    
    keyB.setShaderUniforms(ourShader);
    
    keyB.setPosition(glm::vec3(-0.7, 0, -8));
    keyB.setRotation(glm::vec3(1, 0, 0));
    keyB.setScale(glm::vec3(0.6, 0.6, 0.6));
    keyB.generate("Resources/Textures/letterb.png");
    
    keyC.setShaderUniforms(ourShader);
    keyC.setPosition(glm::vec3(0.7, 0, -8));
    keyC.setRotation(glm::vec3(1, 0, 0));
    keyC.setScale(glm::vec3(0.6, 0.6, 0.6));
    keyC.generate("Resources/Textures/letterb.png");
    
    keyX.setShaderUniforms(ourShader);
    
    keyX.setPosition(glm::vec3(0, 0, -8));
    keyX.setRotation(glm::vec3(1, 0, 0));
    keyX.setScale(glm::vec3(0.6, 0.6, 0.6));
    keyX.generate("Resources/Textures/letterx.png");
    
    
    
    
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS){
       
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