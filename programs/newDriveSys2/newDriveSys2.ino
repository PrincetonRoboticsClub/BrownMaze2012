#include <Encoder.h>
#include <PositionTracker.h>
#include <MotorOutput.h>
#include <PID.h>
#include <DriveSystem.h>
#include <IrSensor.h>


int tileX = 0;
int tileY = 0;
int length = 16.65;

int pinA = 4;
int pinB = 3;
int pinC = 2;

IrSensor left  (pinA, 345.0 / 1024.0);
IrSensor mid   (pinB, 400.0 / 1024.0);
IrSensor right (pinC, 400.0 / 1024.0);

int encoderPins[4] = {51, 50, 53, 52};//51, 50, 53, 52
bool encoderInverts[2] = {true, false};

int motorPinsLeft[3] = {8, 9, 10};
int motorPinsRight[3] = {11, 12, 13};
bool motorInverts[2] = {false, false};

double distancePerCount = 1.0/3.5; // cm
int timeout        = 100;  // ms
double wheelBase   = 7.2;    // cm  // increase wheelbase = turn more

double kStraight[6] = {1.0/12.0,   0.15,   0, 
                       0.35
                     
                   
                 
                                                                                                 +(PI/3),   0.000,    0};//0.337
double kTurn[3]     = {0.38/(PI/3),   0.058,    0};// 0.7/(PI/3)
double kFineTurn[3] = {1/(6*PI),   2,     0};//0.7

double offset = 8.0;
double errors[3] = {2.3, PI/8, PI/120}; // 5deg = PI/36

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
  drive.setMaxSpeed(0.48);//0.46 // 0.52
  
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
 /*
    drive.driveTo(length, 0);
    drive.driveTo(length*2, 0);
    drive.driveTo(length*2, length);
    drive.driveTo(length*2, length*2);
    drive.driveTo(length, length*2);
    drive.driveTo(0, length*2);
    drive.driveTo(0, length);
    drive.driveTo(length, length);
    drive.wait(350);
    drive.turnAngle(PI/2);
    drive.driveTo(0, length);
    drive.driveTo(0, 16.7*2);
    drive.driveTo(16.7, 16.7*2);
    drive.driveTo(16.7*2, 16.7*2);
    drive.driveTo(16.7*2, 16.7);
    drive.driveTo(16.7*2, 0);
    drive.driveTo(16.7, 0);
    drive.driveTo(0, 0);
  */
}

void loop(){
  if(!mid.state()){
    getDir(0);
  }
  else if(!right.state()){
    getDir(3);
  }
  else if(!left.state()){
    getDir(1);
  }
  else{
    getDir(2);
    drive.turnAngle(PI/2);
    drive.wait(350);
  }
  Serial.print(tileX);
  Serial.print("  ");
  Serial.println(tileY);
  drive.driveTo(tileX*length, tileY*length);
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



void getDir(int dir){
  if(abs(drive.angleTo(0 + dir*PI/2))     < PI/4){ 
    tileX++; 
    return;
  }
  else if(abs(drive.angleTo(PI/2 + dir*PI/2))  < PI/4){ 
    tileY++;
    return;
  }
  else if(abs(drive.angleTo(PI + dir*PI/2))    < PI/4){ 
    tileX--; 
    return;
  }
  else if(abs(drive.angleTo(-PI/2 + dir*PI/2)) < PI/4){ 
    tileY--;
    return;
  }
}

