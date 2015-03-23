//
//  KeyObjects.cpp
//  RIVR
//
//  Created by James Rees on 23/03/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#include "KeyObjects.h"
#include "TextureLoader.h"

KeyObjects::KeyObjects(){
    
}


void KeyObjects::generate(char* filePath, GLfloat vertices[]){
    this->textureFilePath = filePath;
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
    
    
    int width, height;
    unsigned char* image;
    
    
    //Textues are loaded here.
    
    image = SOIL_load_image(this->textureFilePath, &width, &height, 0, SOIL_LOAD_RGB);
    SOIL_free_image_data(image);
    this->keyTexture.Generate(width, height, image);

    

}

void KeyObjects::Draw(){
    
    this->keyTexture.Bind();
    
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(this->getPosition().x, this->getPosition().y , this->getPosition().z));
    model = glm::scale(model, glm::vec3(this->getScale().x, this->getScale().y, this->getScale().z));
    
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    
    

}

void KeyObjects::setShaderUniforms(Shader ourShader){
    modelLocation = glGetUniformLocation(ourShader.Program, "model");
    viewLocation = glGetUniformLocation(ourShader.Program, "view");
    projectionLocation = glGetUniformLocation(ourShader.Program, "projection");
    
}