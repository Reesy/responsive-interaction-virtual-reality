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
    sleep(2);
    mySerial.serialport_write(port, ">");
}

void GloveController::write(int pin1, int pin2, int pin3, int pin4, int pin5){
   
        std::string s1 = std::to_string(pin1);
        std::string s2 = std::to_string(pin2);
        std::string s3 = std::to_string(pin3);
        std::string s4 = std::to_string(pin4);
        std::string s5 = std::to_string(pin5);
    
        std::string message = s1 + "," + s2  + "," + s3 + "," + s4 + "," + s5 + ":";
        mySerial.serialport_write(port, message.c_str());
}

void GloveController::close(){
     mySerial.serialport_close(port);
}

void GloveController::flush(){
    mySerial.serialport_flush(port);
    
    
}