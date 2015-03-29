//
//  SceneObjects.cpp
//  RIVR
//
//  Created by James Rees on 16/03/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#include "SceneObjects.h"

SceneObjects::SceneObjects(){
    
}

SceneObjects::~SceneObjects(){
    
}

void SceneObjects::setPosition(glm::vec3 inPosition){
    this->position = inPosition;
}

void SceneObjects::setScale(glm::vec3 inScale){
    this->scale = inScale;
}

void SceneObjects::setYaw(float inYaw){
    this->yaw = inYaw;
    
}
void SceneObjects::setPitch(float inPitch){
    this ->yaw = inPitch;
}
void SceneObjects::setRoll(float inRoll){
    this->roll = inRoll;
}
void SceneObjects::setCollidable(bool isCollidable){
    
}
void SceneObjects::setRotation(glm::vec3 inRotation){
    this->rotation = inRotation;
}

glm::vec3 SceneObjects::getPosition(){
    return this->position;
}

glm::vec3 SceneObjects::getYawPitchRoll(){
    return glm::vec3(this->yaw, this->pitch, this->roll);
}
glm::vec3 SceneObjects::getRotation(){
    return this->rotation;
}

glm::vec3 SceneObjects::getScale(){
    return this->scale;
}

glm::vec3 SceneObjects::movePosition(glm::vec3 startPosition, double time){
    return glm::vec3(0, 0, 0);
}

