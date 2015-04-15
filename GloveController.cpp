//
//  GloveController.cpp
//  RIVR
//
//  Created by James Rees on 12/04/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#include "GloveController.h"

GloveController::GloveController(){
    port = mySerial.serialport_init("/dev/cu.usbmodem1421", 9600);
}

void GloveController::write(int arduinoNumber, int pin, int value){
    sleep(2);
    mySerial.serialport_write(port, ">");
    
    for(int i = 0; i < 10; i++){
        usleep(1000000);
        std::string s = std::to_string(i);
        std::cout << s << std::endl;
        mySerial.serialport_write(port, s.c_str());
        
    }
  
}

void GloveController::close(){
     mySerial.serialport_close(port);
}