//
//  Model.h
//  RIVR
//
//  Created by James Rees on 09/03/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//  This class creates a texture object and calls from the resource manager the data.

#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>

class Model
{
public:
   

    Model();
    // Generates texture from image data
    void Draw();
    void LoadImage();
 
};

#endif

