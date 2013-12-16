/**********************************************
 *                                            
 * Probotics - Princeton University           
 *                                            
 * DriveSystem: allows a differential drive robot
 * with an H-Bridge and quadrature encoders to 
 * travel to coordinates
 *
 * PID loops use position data to calculate
 * motor output
 *                                            
 **********************************************/
 

#ifndef DriveSystem_h
#define DriveSystem_h

#include <arduino.h>
#include <Encoder.h>
#include <PositionTracker.h>
#include <MotorOutput.h>
#include <PID.h>

class DriveSystem{
  private:
    MotorOutput     motors;
	PositionTracker wheels;
	
	double maxSpeed;
	
	double* kStraight, *kTurn, *kFineTurn;
	double* errors;
	
	double offset;
	
	double lastTargetX, lastTargetY;
	
	int sign(double x);
  
  public:
    DriveSystem(MotorOutput m, PositionTracker p, double os, double err[3], double ks[6], double kt[3], double kft[3]);
	
	void turnAngle(double angle);
	void turnTo(double angle);
	void turnTo(double x, double y);
	
	void driveTo(double x, double y);
	void driveDistance(double d);
	
	void wait(double mil);
	
	double angleTo(double x, double y);
	double angleTo(double a);
	double angleBetween(double a, double t);
	double distanceTo(double x, double y);
	double distanceTo(double x, double y, double m);
	
	void setMaxSpeed(double speed);
	void update();
	void reset();
	
	void leftEncoderEventA();
	void leftEncoderEventB();
	void rightEncoderEventA();
	void rightEncoderEventB();
};

#endif