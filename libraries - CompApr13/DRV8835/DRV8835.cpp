#include <Arduino.h>
#include <DRV8835.h>

DRV8835::DRV8835(int pins[4], bool invert[2]) : 
    pinLeftDirection(pins[0]),  pinLeftPWM(pins[1]), 
	pinRightDirection(pins[2]), pinRightPWM(pins[3]), 
	invertLeft(invert[0]), invertRight(invert[1]), 
	leftOut(0), rightOut(0)
{
  pinMode(pinLeftDirection,  OUTPUT);
  pinMode(pinLeftPWM,        OUTPUT);
  pinMode(pinRightDirection, OUTPUT);
  pinMode(pinRightPWM,       OUTPUT);
  
  stop();
}

void DRV8835::arcade(double straight, double turn){
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

void DRV8835::tank(double left, double right){
  leftOut = left;
  rightOut = right;
  analogWrite(pinLeftPWM, (int) abs(leftOut*255));
  analogWrite(pinRightPWM, (int) abs(rightOut*255));
  
  if(leftOut < 0 ^ invertLeft){ digitalWrite(pinLeftDirection, HIGH); }
  else                        { digitalWrite(pinLeftDirection, LOW);  }
  
  if(rightOut < 0 ^ invertRight){ digitalWrite(pinRightDirection, HIGH); }
  else                          { digitalWrite(pinRightDirection, LOW);  }
}

void DRV8835::stop(){
  tank(0, 0);
}

double DRV8835::getLeft(){
  return leftOut;
}
double DRV8835::getRight(){
  return rightOut;
}