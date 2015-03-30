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
    void generate(const char* filePath);
    void Draw();
    void setShaderUniforms(Shader myShader);
};




#endif /* defined(__RIVR__KeyObjects__) */
