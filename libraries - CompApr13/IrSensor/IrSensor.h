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
 

#ifndef IrSensor_h
#define IrSensor_h

#include <Arduino.h>

class IrSensor{
  private:
    int pin;
	double thresh;
  
  public:
    IrSensor(int p, double t);
	
	bool state();
};

#endif