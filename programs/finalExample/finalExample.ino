#include <lotsa libraries>
// NOTE: no ir sensor capability yet...

// Define Maze Parameters
 int mazeSize = 16;
 double tileSize = 16.9;


// Define Pins
 int encPinLeftA = 1;
 int encPinLeftB = 2;
 int encPinRightA = 3;
 int encPinRightB = 4;

 int irPinLeft = 5;
 int irPinForward = 6;
 int irPinRight = 7;

 int motorPinLeft = 8;
 int motorPinRight = 9;


// Define PID Constants
 double[] kStraight = {1, 0.1, 0};
 double[] kTurn     = {1, 0.1, 0};
 
 double[] errors    = {2.5, PI/12};
 
 
// Define Robot Properties
 int countPerRev = 24;
 double wheelRadius = 1.5;
 double wheelBase   = 9.0;
 
 double slowSpeed = 0.50;
 double medSpeed  = 0.75;
 double fastSpeed = 1.00;
 
 int irThreshold = 500;

 
// Create Sensor/Drive Components
 PositionTracker wheels = PositionTracker();
 DriveSystem drive = DriveSystem();
 WallDetector walls = WallDetector(irPinLeft, irPinForward, irPinRight, irThreshold);


// Create Robot
 Robot robot = Robot(wheels, drive, mazeSize, tileSize);

void setup(){
  attachInterrupt(encPinLeftA, wheels.leftEncoderEvent(), CHANGE);
  attachInterrupt(encPinRightA, wheels.rightEncoderEvent(), CHANGE);
}

void loop(){
  // maybe wait for a button to be pressed
  while(!robot.mazeExplored()){
    if(robot.exploring()){
      robot.setMaxSpeed(slowSpeed);
    }
    else {
      robot.setMaxSpeed(medSpeed);
    }
    robot.executeInstruction();
  }
  // maybe wait for another button press
  while(true){
    robot.setMaxSpeed(medSpeed);
    robot.moveToStart();
    robot.setMaxSpeed(fastSpeed);
    robot.moveToGoal();
  }
  // maybe end after a certain amount of time
  // or pause when a button is pressed
}
 
