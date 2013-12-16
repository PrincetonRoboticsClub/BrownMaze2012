#include <Arduino.h>
#include <PositionTracker.h>
#include <Encoder.h>

PositionTracker::PositionTracker(int pins[4], bool inverts[2], double r, double cpr, int t, double wb)
        : leftEncoder(Encoder(pins[0], pins[1], r, cpr, t, inverts[0])), rightEncoder(Encoder(pins[2], pins[3], r, cpr, t, inverts[1])){
  wheelBase = wb;
  reset();
}
PositionTracker::PositionTracker(int pins[4], bool inverts[2], double distancePerCount, int t, double wb)
        : leftEncoder(Encoder(pins[0], pins[1], distancePerCount, t, inverts[0])), rightEncoder(Encoder(pins[2], pins[3], distancePerCount, t, inverts[1])){
  wheelBase = wb;
  reset();
}

void PositionTracker::compute(){
  int nL = leftEncoder.getCount() - lastLeft;
  int nR = rightEncoder.getCount() - lastRight;
  lastLeft += nL;
  lastRight += nR;
  
  double ds = leftEncoder.getDistancePerCount() * (nR+nL)/2;
  x += ds*cos(angle);
  y += ds*sin(angle);
  if(nL != nR){
    angle += leftEncoder.getDistancePerCount()/wheelBase * (nL-nR);
  }
}

double PositionTracker::getXCoord(){ return x; }
double PositionTracker::getYCoord(){ return y; }
double PositionTracker::getAngle(){ return angle; }

void PositionTracker::reset(){
  x = 0;
  y = 0;
  angle = 0;
  lastLeft = 0;
  lastRight = 0;
  leftEncoder.reset();
  rightEncoder.reset();
}
void PositionTracker::mod(){
  //while(angle >= 2*PI){ angle -= 2*PI; }
  //while(angle < 0){ angle += 2*PI; }
}

void PositionTracker::leftEncoderEventA() {
  leftEncoder.encoderEvent(true);
}
void PositionTracker::leftEncoderEventB() {
  leftEncoder.encoderEvent(false);
}
void PositionTracker::rightEncoderEventA() {
  rightEncoder.encoderEvent(true);
}
void PositionTracker::rightEncoderEventB() {
  rightEncoder.encoderEvent(false);
}

double PositionTracker::getSpeed(){
  return (leftEncoder.getSpeed() + rightEncoder.getSpeed()) / 2;
}
double PositionTracker::getAngularSpeed(){
  return (rightEncoder.getSpeed() - leftEncoder.getSpeed()) / wheelBase;
}