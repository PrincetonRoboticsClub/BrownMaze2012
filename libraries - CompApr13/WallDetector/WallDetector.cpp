#include <Arduino.h>
#include <IrSensor.h>
#include <WallDetector.h>

WallDetector::WallDetector(int pins[3], double thresh) : 
    left(pins[0], thresh), mid(pins[1], thresh), right(pins[2], thresh) {}

void WallDetector::walls(bool b[4], int direction){
  b[     direction     ] = mid.state();
  b[(direction + 1) % 4] = right.state();
  b[(direction + 3) % 4] = left.state();
  b[(direction + 2) % 4] = true;
}