//
//  KeyboardModelLoader.h
//  RIVR
//
//  Created by James Rees on 13/03/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#ifndef RIVR_KeyboardModelLoader_h
#define RIVR_KeyboardModelLoader_h
#include "Shader.h"

class KeyBoardModelLoader{
private:
    GLuint VAO, VBO;

    
    void generate_buffers()
    {
        GLfloat vertices[] = {
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,    1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
            
      
        };
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        
        glBindVertexArray(0); // Unbind VAO
    }
    void handle_shaders(Shader testShader){
        
        
        
        
        
    }
    
    
    
public:
   // Shader *myShader;
    
    
    KeyBoardModelLoader(GLfloat vertices[], Shader myShader){
        this->generate_buffers();
        this->handle_shaders(myShader);
    
    }
    
    
    
    void Draw(){
        
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
    
    
    
    
    
    
    
    
    
};


#endif
