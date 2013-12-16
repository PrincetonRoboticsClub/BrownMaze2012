#include <TurnTester.h>

#include <Arduino.h>

TurnTester::TurnTester() : power(0) {}

double turn(double angleTo, double speed, double precision){
	
	if(abs(angleTo) > precision){
		if(speed < 0.005){ power += 0.002; }
		else             { power -= 0.004; }
	}
	else if(abs(angleTo) < precision){
		if(speed > 0.005){ power -= 0.002; }
		else             { power += 0.004; }
	}
	else{
		power = 0;
	}
	return power;
}