//
//  KeyObjects.h
//  RIVR
//
//  Created by James Rees on 23/03/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#ifndef __RIVR__KeyObjects__
#define __RIVR__KeyObjects__

#include <stdio.h>
#include "SceneObjects.h"
#include <glad/glad.h>
#include "TextureLoader.h"
#include <string>
#include "Shader.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class KeyObjects : public SceneObjects{

  public:
    const char* textureFilePath;
    const char* lowerImagePath;
    const char* upperImagePath;
    
    bool isUpper;
    bool isColliding;
    bool collideLocked;
    char key, key2;
    
    GLuint VAO, VBO;
    GLint modelLocation;
    GLint viewLocation;
    GLint projectionLocation;
    
    GLint collideLocation;
    // glm::mat4 model;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
  

    TextureLoader keyTexture;
    KeyObjects();
    
    void init();
    void generate();
    void update();
    void Draw(glm::mat4 view, glm::mat4 projection, Shader ourShader);
    void setShaderUniforms(Shader myShader);
    void setKey(char inLetter, char inLetter2);
    char getKey();
    
    bool getColliding();
    bool getLockState();

    void setColliding(bool inCollision);
    void setCase(bool letterCase);
    void setUpperImage(const char* image1);
    void setLowerImage(const char* image2);
        
        
        
    
};
#endif /* defined(__RIVR__KeyObjects__) */
