/***********************************************
 * 
 * Probotics - Princeton University
 * 
 * Robot: the main class for the maze solving
 *   robot. Uses Brain, PositionTracker, 
 *   DriveSystem, Maze, and WallDetector
 * 
 ***********************************************/

#ifndef Robot_h
#define Robot_h

#include <arduino.h>


#include <StandardCplusplus.h>
#include <queue>
#include <stack>

#include <Brain.h>
#include <Encoder.h>
#include <PositionTracker.h>
#include <MotorOutput.h>
#include <DriveSystem.h>
#include <IrSensor.h>
#include <WallDetector.h>
#include <Tile.h>
#include <Maze.h>

using namespace std;


class Robot{
  private:
    PositionTracker wheels;
    DriveSystem drive;
    WallDetector walls;
    Brain brain;

  public:
    Robot(PositionTracker p, DriveSystem d, WallDetector w, int size);

    void executeInstruction();
    void executeAllInstructions();
    void moveToStart();
    void moveToGoal();

    boolean canExploreAdjacent();
    void getCurrentTile();
    void getCurrentDirection();
    
    boolean mazeExplored();
    void setMaxSpeed(double speed);
};

#endif