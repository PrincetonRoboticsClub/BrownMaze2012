/***********************************************
 * 
 * Probotics - Princeton University
 * 
 * Encoder: uses 4x decoding to decode 2 signals
 *   keeps track of speed and distance
 * 
 ***********************************************/


#ifndef Encoder_h
#define Encoder_h

#include <Arduino.h>

class Encoder{
  private:
    volatile double speed;
    volatile int count;
    volatile int lastTime;
    volatile int p1, p2, p3, p4, p5;
	volatile bool invert;
  double lastCount;
    double distancePerCount;
    double radius, countPerRev;
    int pinA, pinB, timeout;

  public:
    Encoder(int pA, int pB, double r, double cpr, int t, bool invertDir);
    Encoder(int pA, int pB, double r, double cpr, bool invertDir);
	Encoder(int pa, int pB, double distPerCount, int t, bool invertDir);
	Encoder(int pa, int pB, double distPerCount, bool invertDir);
  
    double getDistance();
    double getSpeed();
    double getCount();
    void setTimeout(int t);
    void reset();
    double getDistancePerCount();
    bool stopped();
    int direction();

    void encoderEvent(bool mode);
};

#endif