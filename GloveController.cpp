//
//  GloveController.cpp
//  RIVR
//
//  Created by James Rees on 12/04/2015.
//  Copyright (c) 2015 James Rees. All rights reserved.
//

#include "GloveController.h"


GloveController::GloveController(){
}

void GloveController::write(int arduinoNumber, int pin, int value){
   // std::cout << "The file is: " << file << std::endl;
    fprintf(this->file,"%d", 9); //Writing to the file
}

void GloveController::open(){
    this->file = fopen("/dev/cu.usbmodem1411","w");  //Opening device file
}

void GloveController::close(){
    fclose(this->file);
}