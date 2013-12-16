#include <Arduino.h>
#include <MotorOutput.h>

MotorOutput::MotorOutput(int leftPins[3], int rightPins[3], bool invert[2]) : 
    pinLeftA(leftPins[0]),   pinLeftB(leftPins[1]),   pinLeftSpeed(leftPins[2]), 
	pinRightA(rightPins[0]), pinRightB(rightPins[1]), pinRightSpeed(rightPins[2]), 
	leftOut(0), rightOut(0)
{
  pinMode(pinLeftA,      OUTPUT);
  pinMode(pinLeftB,      OUTPUT);
  pinMode(pinLeftSpeed,  OUTPUT);
  
  pinMode(pinRightA,     OUTPUT);
  pinMode(pinRightB,     OUTPUT);
  pinMode(pinRightSpeed, OUTPUT);
}

void MotorOutput::arcade(double straight, double turn){
  double y = abs(straight);
  double x = abs(turn);
  
  double right;
  double left;
  
  if (turn >= 0){
    left  = y - x;
	right = max(x, y);
  }
  else{
    left  = max(x, y);
	right = y - x;
  }
  
  if(straight < 0){
    left  *= -1;
	right *= -1;
  }
  
  tank(left, right);
}

void MotorOutput::tank(double left, double right){
  leftOut = left;
  rightOut = right;
  analogWrite(pinLeftSpeed,  abs(left*255));
  analogWrite(pinRightSpeed, abs(right*255));
  
  if(left < 0 ^ invertLeft){ 
    digitalWrite(pinLeftA, HIGH); 
	digitalWrite(pinLeftB, LOW);
  }
  else{ 
    digitalWrite(pinLeftA, LOW); 
	digitalWrite(pinLeftB, HIGH); 
  }
  
  if(right < 0 ^ invertRight){ 
    digitalWrite(pinRightA, HIGH); 
	digitalWrite(pinRightB, LOW);
  }
  else{ 
    digitalWrite(pinRightA, LOW); 
	digitalWrite(pinRightB, HIGH); 
  }
}

void MotorOutput::test(double spd){
  analogWrite(pinLeftSpeed,  abs(spd*255));
  analogWrite(pinRightSpeed, abs(spd*255));

  digitalWrite(pinLeftA, HIGH); 
  digitalWrite(pinLeftB, LOW);
  
  digitalWrite(pinRightA, HIGH); 
  digitalWrite(pinRightB, LOW);
}

void MotorOutput::stop(){
  tank(0, 0);
}

double MotorOutput::getLeft(){
  return leftOut;
}
double MotorOutput::getRight(){
  return rightOut;
}