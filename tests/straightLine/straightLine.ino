/********************************
 *
 *    Drive Straight Forever
 *
 ********************************/

#include <Encoder.h>
#include <PositionTracker.h>
#include <MotorOutput.h>

/********************************
 *
 *    Variable Initialization
 *
 ********************************/
 
// pins and inverts

int encoderPins[4] = {51, 50, 53, 52};
bool encoderInverts[2] = {true, false};

int motorPinsLeft[3]  = { 9,  8, 10};
int motorPinsRight[3] = {12, 11, 13};
bool motorInverts[2] = {false, false};

// physical constants

double dpc = 1.0/3.5;
int t = 100;
double wb = 7.2;
double db = 0.1;

// objects

PositionTracker wheels (encoderPins, encoderInverts, dpc, t, wb);
MotorOutput motors (motorPinsLeft, motorPinsRight, motorInverts, db);

// --------- TEST VARIABLES ----------

double power = 0.6;        // straight power
double trim = -0.0;       // turn power required at full power.12
double turnP = 1 / (PI) * power;   // if off angle, turn to mid

double e = 8.0;


/********************************
 *
 *    Main Code Functions
 *
 ********************************/
 
void setup(){
  motors.stop();
  
  Serial.begin(9600);
  
  attachInterrupt(encoderPins[0],  leftEventA,  CHANGE);
  attachInterrupt(encoderPins[1],  leftEventB,  CHANGE);
  attachInterrupt(encoderPins[2], rightEventA,  CHANGE);
  attachInterrupt(encoderPins[3], rightEventB,  CHANGE);
  
  delay(2000);
  
  wheels.reset();
}

void loop(){
  wheels.compute();
  print();
  
  // drive straight
  
  double s, t;
  
  s = power;
  t = turnP * angleTo(getTX(), getTY()) + s*trim;
  
  motors.arcade(s, t);
}


/********************************
 *
 *    Control Loop
 *
 ********************************/

double getTX(){
  return wheels.getXCoord() + e;
}
double getTY(){
  return 0;
}
double getTD(){
  return 10.0;
}

double angleTo(double x, double y){
  double dx = x - wheels.getXCoord();
  double dy = y - wheels.getYCoord();
  double a;
  if(dx == 0){
    if(dy >= 0){ a =  PI/2; }
    else       { a = -PI/2; }
  }
  else if(dx > 0){ a = atan(dy/dx); }
  else           { a = atan(dy/dx) + PI; }
  return angleBetween(wheels.getAngle(), a);
}
double angleBetween(double a, double t){
  double angle = t - a;
  while(angle >   PI){ angle -= 2*PI; }
  while(angle <= -PI){ angle += 2*PI; }
  return angle;
}

void print(){
  Serial.print(" c: (");
  Serial.print(wheels.getXCoord());
  Serial.print(", ");
  Serial.print(wheels.getYCoord());
  Serial.print(") ");
  Serial.print(" angle: ");
  Serial.print(wheels.getAngle() * 180/PI);
  
  Serial.println();
}

/********************************
 *
 *    Interrupt Service Routines
 *
 ********************************/
 
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
