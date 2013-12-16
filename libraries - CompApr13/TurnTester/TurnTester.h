#ifndef TurnTester_h
#define TurnTester_h

#include <Arduino.h>

class TurnTester{
	private:
		double power;
	
	public:
		TurnTester();
		double turn(double angleTo, double speed, double precision);
};

#endif