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
#include <iostream>
#include <unistd.h>
#include "arduino-serial-lib.h"
class GloveController{
    
  
    public:
    arduinoSerialLib mySerial;
    int port;
   
    //initialise the arduino
    GloveController();
    
    void flush();
    void write(int pin1, int pin2, int pin3, int pin4, int pin5);

    void close();
    
};

#endif /* defined(__RIVR__GloveController__) */
