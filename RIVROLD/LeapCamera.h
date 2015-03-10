#ifndef __LeapCamera_H__
#define __LeapCamera_H__

#include <LEAP/Leap.h>

class LeapCamera{


public:
	


	LeapCamera();
	~LeapCamera();
	void run();
	void render();
	void update();
	void CreateTestImage();
   


};


#endif