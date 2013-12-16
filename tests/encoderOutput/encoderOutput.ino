#include <Encoder.h>

int pinA = 53;
int pinB = 52;
bool invert = false;

Encoder encoder(pinA, pinB, 1, 48, 100, invert);

void setup(){
  attachInterrupt(pinA, eventA, CHANGE);
  attachInterrupt(pinB, eventB, CHANGE);
  
  Serial.begin(9600);
  
}

void loop(){
  Serial.print("count: ");
  Serial.print(encoder.getCount());
  Serial.print("  dist: ");
  Serial.print(encoder.getDistance());
  Serial.print("  speed: ");
  Serial.print(encoder.getSpeed());
  Serial.println();
}


void eventA() { encoder.encoderEvent(false); }
void eventB() { encoder.encoderEvent(true);  }
