#include <Encoder.h>
#include <PositionTracker.h>
#include <MotorOutput.h>

int encoderPins[4] = {2, 3, 4, 5};
bool encoderInverts[2] = {false, true};

int mL[3] = {8, 9, 10};
int mR[3] = {11, 12, 13};
bool motorInverts[2] = {false, false};

double distancePerCount = 1.0/3.5;  // cm
int timeout             = 100;      // ms
double wheelBase        = 7;    // cm ..480..

PositionTracker wheels (encoderPins, encoderInverts, distancePerCount, timeout, wheelBase);
MotorOutput motors (mL, mR, motorInverts);

void setup(){
  Serial.begin(9600);
  
  attachInterrupt(encoderPins[0],  leftEventA,  CHANGE);
  attachInterrupt(encoderPins[1],  leftEventB,  CHANGE);
  attachInterrupt(encoderPins[2], rightEventA,  CHANGE);
  attachInterrupt(encoderPins[3], rightEventB,  CHANGE);
  
  //digitalWrite(encoderPins[0], HIGH);
  //digitalWrite(encoderPins[1], HIGH);
  //digitalWrite(encoderPins[2], HIGH);
  //digitalWrite(encoderPins[3], HIGH);
  
  wheels.reset();
}

void loop(){
  
  wheels.compute();
  
  /*Serial.print("x:  ");
  Serial.print(wheels.getXCoord());
  Serial.print(" y:  ");
  Serial.print(wheels.getYCoord());*/
  Serial.print(" a: ");
  Serial.print(wheels.getAngle() * 180/PI);
  Serial.print(" as: ");
  Serial.println(wheels.getAngularSpeed());
  
  //Serial.print(" -> ");
  
  double angle = PI/2 - wheels.getAngle();
  
  while(angle >   PI){ angle -= 2*PI; }
  while(angle <= -PI){ angle += 2*PI; }
  
  //Serial.println(angle*180/PI);

  //motors.tank(-1, -1);
  motors.arcade(0.25, 0);
}


void leftEventA() { 
  wheels.leftEncoderEventA();
}
void leftEventB() { 
  wheels.leftEncoderEventB();
}

void rightEventA(){ 
  wheels.rightEncoderEventA(); 
}
void rightEventB(){ 
  wheels.rightEncoderEventB();
}

