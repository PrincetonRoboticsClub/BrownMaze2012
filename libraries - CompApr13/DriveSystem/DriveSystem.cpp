#include <arduino.h>
#include <Encoder.h>
#include <PositionTracker.h>
#include <MotorOutput.h>
#include <PID.h>
#include <DriveSystem.h>

int DriveSystem::sign(double x){
	if(x > 0){ return  1; }
	if(x < 0){ return -1; }
	return 0;
}


DriveSystem::DriveSystem(MotorOutput m, PositionTracker p, double os, double err[3], double ks[6], double kt[3], double kft[3]) : 
     motors(m), wheels(p), offset(os), errors(err), kStraight(ks), kTurn(kt), kFineTurn(kft), maxSpeed(1.0), lastTargetX(0), lastTargetY(0) {}

void DriveSystem::turnTo(double angle){
  PID turn(kTurn, maxSpeed);
  while(abs(angleTo(angle)) > errors[1]){
	wheels.compute();
	double turnPower = turn.compute(angleTo(angle));
	motors.arcade(0, turnPower);
  }
  motors.stop();
  
  PID fine(kFineTurn, maxSpeed);
  while(abs(angleTo(angle)) > errors[2] && abs(wheels.getAngularSpeed()) > 0.000){
	wheels.compute();
	double turnPower = fine.compute(angleTo(angle));
	  
	motors.arcade(0, turnPower);
  }
  motors.stop();
}
void DriveSystem::turnTo(double x, double y){
  turnAngle(angleTo(x, y));
}
void DriveSystem::turnAngle(double angle){
  turnTo(wheels.getAngle() + angle);
}

void DriveSystem::driveTo(double x_t, double y_t){
  wait(450);
  turnTo(x_t, y_t);
  wait(450);
  
  double x_s = /*lastTargetX;*/ wheels.getXCoord();
  double y_s = /*lastTargetY;*/ wheels.getYCoord();
  double e = offset;
  
  PID straight(kStraight[0], kStraight[1], kStraight[2], maxSpeed);
  PID turn    (kStraight[3], kStraight[4], kStraight[5], 0.30);//0.28
  
  double dist = 2*errors[0];
  double s = 0.34;// 0.34
     // turn straight kp!!!! = 0.34/(PI/3) 
  while(dist > errors[0]){
    wheels.compute();
	double x_r = wheels.getXCoord();
	double y_r = wheels.getYCoord();
	
	double x_e = (((x_r - x_s) * (x_t - x_s) + (y_r - y_s) * (y_t - y_s)) * (x_t - x_s) / ((x_t - x_s) * (x_t - x_s) + (y_t - y_s) * (y_t - y_s))) + 
            ((e * (x_t - x_s)) / sqrt((x_t - x_s) * (x_t - x_s) + (y_t - y_s) * (y_t - y_s))) + x_s;
			
    double y_e = (((x_r - x_s) * (x_t - x_s) + (y_r - y_s) * (y_t - y_s)) * (y_t - y_s) / ((x_t - x_s) * (x_t - x_s) + (y_t - y_s) * (y_t - y_s))) + 
            ((e * (y_t - y_s)) / (sqrt((x_t - x_s) * (x_t - x_s) + (y_t - y_s) * (y_t - y_s)))) + y_s;
	
	dist = (sqrt((x_t - x_s) * (x_t - x_s) + (y_t - y_s) * (y_t - y_s))) - (((x_r - x_s) * (x_t - x_s) + (y_r - y_s) * (y_t - y_s)) 
            / (sqrt((x_t - x_s) * (x_t - x_s) + (y_t - y_s) * (y_t - y_s))));
	
	double t = turn.compute(-angleTo(x_e, y_e));
	
	if(abs(wheels.getSpeed()) <= 2.0){ s += 0.005; }
	else                        { s = 0.30; }
	if(s > 0.70) { s = 0.70; }
	if(s < 0.07) { s = 0.07; }
	
	Serial.print("dist: ");
	Serial.print(dist);
	Serial.print(" pos: (");
	Serial.print(x_r);
	Serial.print(", ");
	Serial.print(y_r);
	Serial.print(") target: (");
	Serial.print(x_e);
	Serial.print(", ");
	Serial.print(y_e);
	Serial.print(") a: ");
	Serial.print(angleTo(x_e, y_e) * 180/PI);
	Serial.print(" ");
	
	Serial.print("s: ");
	Serial.print(s);
	Serial.print(" t: ");
	Serial.println(t);
	
	motors.arcade(-s, t);
  }
  motors.stop();
  //Serial.println("DONE!!!!!!");
  lastTargetX = x_t;
  lastTargetY = y_t;
}
void DriveSystem::driveDistance(double distance){
  wheels.mod();
  double angle = wheels.getAngle();
  
  double x = distance * cos(angle);
  double y = distance * sin(angle);
  
  driveTo(x, y);
}

void DriveSystem::wait(double mil){
  int startTime = millis();
  while(millis() - startTime < mil){}
}

double DriveSystem::angleTo(double x, double y){
  double dx = x - wheels.getXCoord();
  double dy = y - wheels.getYCoord();
  
  double a;
  
  if(dx == 0){
    if(dy >= 0){ a =  PI/2; }
    else       { a = -PI/2; }
  }
  else if(dx > 0){
    a = atan(dy/dx);
  }
  else {
    a = atan(dy/dx) + PI;
  }
  
  return angleTo(a);
}
double DriveSystem::angleTo(double a){
  return angleBetween(wheels.getAngle(), a);
}
double DriveSystem::angleBetween(double a, double t){
  double angle = t - a;
  
  while(angle >   PI){ angle -= 2*PI; }
  while(angle <= -PI){ angle += 2*PI; }
  
  return angle;
}
double DriveSystem::distanceTo(double x, double y){
  double dx = x - wheels.getXCoord();
  double dy = y - wheels.getYCoord();
  return sqrt(dx*dx + dy*dy);
}
double DriveSystem::distanceTo(double x, double y, double m){
	double rx = wheels.getXCoord();
	double ry = wheels.getYCoord();
	
	double qx = (m*m*x - m*y + m*ry + rx) / (1 + m*m);
	double qy = y + m*(qx - x);
	
	return sqrt((qx-rx)*(qx-rx) + (qy-ry)*(qy-ry));
}

void DriveSystem::setMaxSpeed(double speed){
  maxSpeed = speed;
}
void DriveSystem::update(){
  wheels.compute();
}
void DriveSystem::reset(){
  wheels.reset();
}

void DriveSystem::leftEncoderEventA() { 
  wheels.leftEncoderEventA();
}
void DriveSystem::leftEncoderEventB() { 
  wheels.leftEncoderEventB();
}
void DriveSystem::rightEncoderEventA(){ 
  wheels.rightEncoderEventA(); 
}
void DriveSystem::rightEncoderEventB(){ 
  wheels.rightEncoderEventB();
}