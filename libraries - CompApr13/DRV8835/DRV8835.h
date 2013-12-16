/**********************************************
 *                                            
 * Probotics - Princeton University           
 *                                            
 * DRV8835: converts arcade drive and     
 *   tank drive values to motor pwm output 
 *   usable by the DRV8835 motor driver 
 *                                            
 **********************************************/


#ifndef DRV8835_h
#define DRV8835_h

#include <Arduino.h>

class DRV8835{
  public:
    DRV8835(int pins[4], bool invert[2]);
	
    void arcade(double straight, double turn);
    void tank(double left, double right);
    void stop();
    double getLeft();
    double getRight();
  private:
    double leftOut, rightOut;
	
	bool invertLeft, invertRight;
	
    int pinLeftDirection, pinRightDirection;
    int pinLeftPWM,       pinRightPWM;
};

#endif 