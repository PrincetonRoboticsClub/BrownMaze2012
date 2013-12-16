#include <Arduino.h>
#include <PID.h>

void PID::init(double p, double i, double d, double lo, double hi){
  kp=p; ki=i; kd=d;
  outMin=lo; outMax=hi;
  reset();
}


PID::PID(double p,double i,double d,double lo,double hi){
  init(p, i, d, lo, hi);
}
PID::PID(double p, double i, double d, double range){
  init(p, i, d, -range, range);
}
PID::PID(double p,double i,double d){
  init(p, i, d, -1.0, 1.0);
}
PID::PID(double k[3],double range[2]){
  init(k[0], k[1], k[2], range[0], range[1]);
}
PID::PID(double k[3],double range){
  init(k[0], k[1], k[2], -range, range);
}
PID::PID(double k[3]){
  init(k[0], k[1], k[2], -1.0, 1.0);
}
  
double PID::compute(double input, double setPoint){
  double error = setPoint - input;
  int temp = millis();
  double dt=(double) (temp-lastT);    
  
  iTerm += (ki * error * dt/1000.0);
  
  if(iTerm > outMax){ iTerm= outMax; }
  else { if(iTerm < outMin){ iTerm= outMin; } }
  
  double dInput = input - lastInput;

  double output = kp*error + iTerm - kd*dInput;     
  if(output > outMax){ output = outMax; }
  else{ if(output < outMin){ output = outMin; } }
  
  lastInput = input;
  lastT = temp;

  return output;
}
double PID::compute(double input){
  return compute(input, 0);
}

void PID::reset(){
  iTerm=0;
  lastT=millis();
  lastInput=0;
}
void PID::setConstants(double p,double i,double d){
  kp=p; ki=i; kd=d;
}