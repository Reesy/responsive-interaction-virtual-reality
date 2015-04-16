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
   
     //   usleep(1000000);
       // std::string s = std::to_string(pin1);
       // std::cout << s << std::endl;
   // }
 //   mySerial.serialport_write(port, s.c_str());

    //   usleep(1000000);
        std::string s1 = std::to_string(pin1);
        std::string s2 = std::to_string(pin2);
        std::string s3 = std::to_string(pin3);
    
        std::string message = s1 + "," + s2  + "," + s3 + ":";
    
      //  mySerial.serialport_write(port, s1.c_str() << s2.c_str());
       // mySerial.serialport_write(port, ",");
        //mySerial.serialport_write(port, s2.c_str());
    mySerial.serialport_write(port, message.c_str());
    
}

void GloveController::close(){
     mySerial.serialport_close(port);
}