/**********************************************
 *                                            
 * Probotics - Princeton University           
 *                                            
 * MotorOutput: converts arcade drive and     
 *   tank drive values to motor pwm output  
 *
 * Compatible with the SN751440 H-Bridge
 *                                            
 **********************************************/


#ifndef MotorOutput_h
#define MotorOutput_h

#include <Arduino.h>

class MotorOutput{
  public:
    MotorOutput(int leftPins[3], int rightPins[3], bool invert[2]);
    void arcade(double straight, double turn);
    void tank(double left, double right);
    void stop();
    double getLeft();
    double getRight();
	
	void test(double spd);
	
  private:
    double leftOut, rightOut;
	
	bool invertLeft, invertRight;
	
    int pinLeftA,  pinLeftB,  pinLeftSpeed;
    int pinRightA, pinRightB, pinRightSpeed;
};

#endif 