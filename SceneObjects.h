//
//  SceneObjects.h
//  RIVR
//
//  Created by James Rees on 16/03/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#ifndef __RIVR__SceneObjects__
#define __RIVR__SceneObjects__

#include <stdio.h>
#include <glm/glm.hpp>


class SceneObjects{
    
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    
    
public:
    SceneObjects();
    ~SceneObjects();
    
    //getters and setters
    void setPosition(glm::vec3);
    void setScale(glm::vec3);
    void setRotation(glm::vec3);
    void setCollidable(bool);
    
    glm::vec3 getPosition();
    glm::vec3 getScale();
    glm::vec3 getRotation();
    
    //move object returns a position via a vec3, given the vector to move towards it's will interploate to that point based on time.
    glm::vec3 movePosition(glm::vec3, double time);
    
    //checks if the object is collidable or not
    
    bool collidable;
    
};






#endif /* defined(__RIVR__SceneObjects__) */
