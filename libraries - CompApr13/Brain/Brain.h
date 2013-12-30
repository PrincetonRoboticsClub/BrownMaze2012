/***********************************************
 * 
 * Probotics - Princeton University
 * 
 * Brain: stores a maze and calculates shortest
 *   paths between tiles (runs A*)
 * 
 ***********************************************/


#ifndef Brain_h
#define Brain_h

#include <Arduino.h>
#include <Tile.h>
#include <Maze.h>

#include <queue>
#include <stack>

using namespace std; // to get priority queue

class Brain{
  private:
    Maze maze; // the set of tiles that make up the maze
    double turnCost, straightCost; // movement costs of turning / going straight
    double tileLength; // physical length of a tile

    queue<double> xInstructions, yInstructions; // instruction queues

	// get x coordinate in a direction
	double xInDir(int tx, int direction);
	// get y coordinate in a direction
	double yInDir(int ty, int direction);
  
    // class used by A* / breadth first search
    class Node{
      private:
	    // used by constructors
        void init(int tx, int ty, int gx, int gy, Node* p, Brain* b);

      public:
        int x, y; // node location
        Node* previousPtr; // pointer to previous node in algorithm
		Brain* parent; // the brain that this belongs to

        double moveCost; // cost of moving from current location to here
        double distanceCost; // cost of min path to goal
        double straightCount; // number of tiles moved straight

		// constructors... some use a goal location / parent node
        Node(int tx, int ty, int gx, int gy, Node* p, Brain* b);
        Node(int tx, int ty, int gx, int gy,          Brain* b);
        Node(int tx, int ty,                 Node* p, Brain* b);
        Node(int tx, int ty,                          Brain* b);
        Node();

		// give empty neighbor set (?)
        queue<Node> neighbors();
		// give a set of neighboring nodes that can be reached from this node
		queue<Node> neighbors(int gx, int gy);
		// calculate the total heuristic cost of this node
        double cost();

		// comparison operator between 2 nodes
        bool operator()(const Node& a, const Node& b);
    };
	
	// Location class... probably unnecessary
	class Location{
      private:
        double x, y;
  
      public:
        Location(double dx, double dy);
	    Location();
	
        double getX();
	    double getY();
    };

  public:
    // constructor
    Brain(int mazeSize, int tLength, double tc, double sc);
    
	// get the path between start(tx, ty) and goal(gx, gy) tiles
    stack<Brain::Location> pathBetween(int tx, int ty, int gx, int gy, int startDirection);
    // get the path from start(tx, ty) to the nearest unexplored tile
	stack<Brain::Location> pathToUnexplored(int tx, int ty, int startDirection);

	// is the next tile in a direction explored
    bool nextIsExplored(int tx, int ty, int direction);
	// is there a wall in a direction
    bool hasWall(int tx, int ty, int direction);

	// get the x-coordinate of the next instruction
    double nextXInstruction();
	// get the y-coordinate of the next instruction
	double nextYInstruction();
	// are there no more instructions currently queued
    bool instructionsEmpty();
	
	// create a new instruction set that explores a tile in the maze
    void generateExploreInstruction(int tx, int ty, int direction);
	// create a new instruction set that goes from start(tx, ty) to goal(gx, gy)
    void generateRouteToInstruction(int tx, int ty, int gx, int gy, int direction);

	// update the wall locations of a newly explored tile
    void updateMaze(int tx, int ty, bool walls[4]);
    void updateMaze(int n, bool walls[4]);
	
	// for calculating heuristic distance cost
	double minCost(int startX, int startY, int goalX, int goalY);
	double getTurnCost();
	double getStraightCost();
};

#endif