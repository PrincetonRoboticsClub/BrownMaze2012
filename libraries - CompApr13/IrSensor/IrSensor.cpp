#include <Arduino.h>
#include <IrSensor.h>

IrSensor::IrSensor(int p, double t) : pin(p), thresh(t){
  pinMode(p, INPUT);
}

bool IrSensor::state(){
  return analogRead(pin) > thresh*1024.0;
}