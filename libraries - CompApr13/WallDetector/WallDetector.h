/***********************************************
 * 
 * Probotics - Princeton University
 * 
 * IrSensor: converts an IR light reading to
 * wall detection
 *
 * NOTE: with more time, this would actually be
 * a robust, useful class
 * 
 ***********************************************/
 

#ifndef WallDetector_h
#define WallDetector_h

#include <Arduino.h>
#include <IrSensor.h>

class WallDetector{
  private:
    IrSensor left, mid, right;
  
  public:
    WallDetector(int pins[3], double thresh);
	
	void walls(bool b[4], int direction);
};

#endif