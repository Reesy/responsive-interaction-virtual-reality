//
//  GloveController.h
//  RIVR
//
//  Created by James Rees on 12/04/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#ifndef __RIVR__GloveController__
#define __RIVR__GloveController__

#include <stdio.h>

class GloveController{
    
    //initialise the arduino
    GloveController();
    
    
    void write(int arduinoNumber, int pin, int value);
    
    
    
};





#endif /* defined(__RIVR__GloveController__) */
