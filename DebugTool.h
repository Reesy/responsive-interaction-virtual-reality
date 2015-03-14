//
//  DebugViewer.h
//  RIVR
//
//  Created by James Rees on 12/03/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#ifndef __RIVR__DebugTool__
#define __RIVR__DebugTool__

#include <stdio.h>
#include <string>

#include "Shader.h"



struct drawMessage{
    std::string varName;
    std::string outputData;
};

struct object{
    int ID;
  //  GLfloat Verticies;
   // GLuint Elements[];
    
    
    
};
class DebugTool{
    public:
        void addLog(std::string, int);
        void addLog(std::string, float);
        void addLog(std::string, bool);
        void RenderLog(int x, int y);
        void addRenderObject(int ID, GLfloat[], Shader);
    
};


#endif /* defined(__RIVR__DebugViewer__) */
