/**********************************************
 * 
 * Probotics - Princeton University
 * 
 * PID: pid loop calculates output in a range
 *   of values using input and a setpoint
 * 
 **********************************************/

#ifndef PID_h
#define PID_h

#include <Arduino.h>

class PID{
  public:
    PID(double p, double i, double d, double lo, double hi);
	PID(double p, double i, double d, double range);
    PID(double p,double i,double d);
	PID(double k[3],double range[2]);
	PID(double k[3],double range);
    PID(double k[3]);
    double compute(double input, double setPoint);
	double compute(double error);
    void reset();
    void setConstants(double p,double i,double d);

  private:
    double kp,ki,kd, outMin,outMax;
    double iTerm;
    unsigned long lastT;
    double lastInput;
	
	void init(double p, double i, double d, double lo, double hi);
};

#endif