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

struct AABB{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    

};

class SceneObjects{
    
    glm::vec3 rotation;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec4 testPosition;
    
    AABB boundingBox;
    
    float pitch;
    float roll;
    float yaw;
    
public:
    SceneObjects();
    ~SceneObjects();
    
    //getters and setters
    void setRotation(glm::vec3);
    void setPosition(glm::vec3);
    void setScale(glm::vec3);
    void setYaw(float);
    void setPitch(float);
    void setRoll(float);
    void setCollidable(bool);
    void setTestPosition(glm::vec4);
    void setAABB(float, float, float, float, float, float);
    
    glm::vec3 getPosition();
    glm::vec3 getScale();
    glm::vec3 getRotation();
    glm::vec3 getYawPitchRoll();
    glm::vec4 getTestPosition();
    AABB getAABB();
    
    
    float getYaw();
    float getPitch();
    float getRoll();
    
    //move object returns a position via a vec3, given the vector to move towards it's will interploate to that point based on time.
    glm::vec3 movePosition(glm::vec3, double time);
    
    //checks if the object is collidable or not
    
    bool collidable;
    
};






#endif /* defined(__RIVR__SceneObjects__) */
