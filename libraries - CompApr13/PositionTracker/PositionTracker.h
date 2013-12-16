/***************************************************
 * 
 * Probotics - Princeton University
 * 
 * PositionTracker: uses 2 encoders to keep
 *   track of position, angle, velocity, and
 *   angular velocity
 * 
 * note:
 *   leftEncoderEvent() and rightEncoderEvent()
 *     must be attached to interrupts
 *   compute() should be called frequently
 * 
 ***************************************************/

#ifndef PositionTracker_h
#define PositionTracker_h

#include <Arduino.h>
#include <Encoder.h>

class PositionTracker{
  public:
    PositionTracker(int pins[4], bool inverts[2], 
                    double r, double cpr, int t, double wb);
	PositionTracker(int pins[4], bool inverts[2], 
                    double distancePerCount, int t, double wb);
					
    void compute();
    
    double getXCoord();
    double getYCoord();
    double getAngle();
    
    void reset();
    void mod();

    double getSpeed();
    double getAngularSpeed();

    void leftEncoderEventA();
	void leftEncoderEventB();
    void rightEncoderEventA();
	void rightEncoderEventB();

  private:
    double wheelBase;
    Encoder leftEncoder;
    Encoder rightEncoder;
    double x,y,angle;
    int lastLeft, lastRight;
	bool leftInvert, rightInvert;
};

#endif