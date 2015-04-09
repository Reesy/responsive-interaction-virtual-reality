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
#include <GL/glew.h>
#include "TextureLoader.h"
#include <string>
#include "Shader.h"
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class KeyObjects : public SceneObjects{

  public:
    const char* textureFilePath;
    const char* lowerImagePath;
    const char* upperImagePath;
    
    bool isUpper;
    
    char key, key2;
    
    GLuint VAO, VBO;
    GLint modelLocation;
    GLint viewLocation;
    GLint projectionLocation;
    // glm::mat4 model;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    
    
    TextureLoader keyTexture;
    KeyObjects();
    void init();
    void generate();
    void Draw(glm::mat4 view, glm::mat4 projection);
    void setShaderUniforms(Shader myShader);
    void setKey(char inLetter, char inLetter2);
    char getKey();
    
    void setCase(bool letterCase);
    void setUpperImage(char* image1);
    void setLowerImage(char* image2);
        
        
        
    
};
#endif /* defined(__RIVR__KeyObjects__) */
