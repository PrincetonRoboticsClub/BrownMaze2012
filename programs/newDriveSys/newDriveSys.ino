#include <Encoder.h>
#include <PositionTracker.h>
#include <MotorOutput.h>
#include <PID.h>
#include <DriveSystem.h>


int encoderPins[4] = {2, 3, 4, 5};//51, 50, 53, 52
bool encoderInverts[2] = {false, true};

int motorPinsLeft[3] = {8, 9, 10};
int motorPinsRight[3] = {11, 12, 13};
bool motorInverts[2] = {false, false};

double distancePerCount = 1.0/3.5; // cm
int timeout        = 100;  // ms
double wheelBase   = 7.2;    // cm  // increase wheelbase = turn more

double kStraight[6] = {1.0/12.0,   0.15,   0, 
                       0.35/(PI/3),   0.000,    0};//0.36
double kTurn[3]     = {0.47/(PI/3),   0.085,    0};// 0.7/(PI/3)
double kFineTurn[3] = {1/(6*PI),   3,     0};//0.7

double offset = 8.0;
double errors[3] = {0.6, PI/8, PI/120}; // 5deg = PI/36

PositionTracker wheels (encoderPins, encoderInverts, distancePerCount, timeout, wheelBase);
MotorOutput motors (motorPinsLeft, motorPinsRight, motorInverts);

DriveSystem drive(motors, wheels, offset, errors, kStraight, kTurn, kFineTurn);

void setup(){
  motors.stop();
  drive.reset();
  
  Serial.begin(9600);
  
  attachInterrupt(encoderPins[0],  leftEventA,  CHANGE);
  attachInterrupt(encoderPins[1],  leftEventB,  CHANGE);
  attachInterrupt(encoderPins[2], rightEventA,  CHANGE);
  attachInterrupt(encoderPins[3], rightEventB,  CHANGE);
  
  drive.wait(2000);
  drive.setMaxSpeed(0.46);//0.46
  
  /*
   // square
    drive.driveTo(16.8, 0);
    drive.driveTo(16.8, 16.8);
    drive.driveTo(0, 16.8);
    drive.driveTo(0, 0);
  */
  /*
   // left square
    drive.driveTo(16.8, 0);
    drive.driveTo(16.8, -16.8);
    drive.driveTo(0, -16.8);
    drive.driveTo(0, 0);
  */
  /*
   // S 2x2
    drive.driveTo(16.8, 0);
    drive.driveTo(16.8, 16.8);
    drive.driveTo(0, 16.8);
    drive.driveTo(0, 16.8*2);
    drive.driveTo(16.8, 16.8*2);
  */
   // spiral 3x3
    drive.driveTo(16.8, 0);
    drive.driveTo(16.8*2, 0);
    drive.driveTo(16.8*2, 16.8);
    drive.driveTo(16.8*2, 16.8*2);
    drive.driveTo(16.8, 16.8*2);
    drive.driveTo(0, 16.8*2);
    drive.driveTo(0, 16.8);
    drive.driveTo(16.8, 16.8);
    drive.wait(350);
    drive.turnAngle(PI/2);
    drive.driveTo(0, 16.8);
    drive.driveTo(0, 16.8*2);
    drive.driveTo(16.8, 16.8*2);
    drive.driveTo(16.8*2, 16.8*2);
    drive.driveTo(16.8*2, 16.8);
    drive.driveTo(16.8*2, 0);
    drive.driveTo(16.8, 0);
    drive.driveTo(0, 0);
}

void loop(){
//  drive.turnTo(100, 0);
//  drive.wait(500);
//  drive.driveTo(100, 0);
//  drive.wait(500);
  //drive.turnTo(0, 0);
  //drive.wait(500);
  //drive.driveTo(0, 0);
  //drive.wait(500);
  
  
  /*
  drive.turnTo(PI/2);
  drive.wait(500);
  drive.turnTo(PI);
  drive.wait(500);
  drive.turnTo(3*PI/2);
  drive.wait(500);
  drive.turnTo(0);
  drive.wait(500);
  */
  
}


void leftEventA() { 
  drive.leftEncoderEventA();
}
void leftEventB() { 
  drive.leftEncoderEventB();
}

void rightEventA(){ 
  drive.rightEncoderEventA(); 
}
void rightEventB(){ 
  drive.rightEncoderEventB();
}

