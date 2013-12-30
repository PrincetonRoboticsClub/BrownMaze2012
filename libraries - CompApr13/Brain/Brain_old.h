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

using namespace std;

class Brain{
  private:
    Maze maze;
    double turnCost, straightCost;
    double tileLength;

    queue<double> xInstructions, yInstructions;

	double xInDir(int tx, int direction);
	double yInDir(int ty, int direction);
  
    class Node{
      private:
        void init(int tx, int ty, int gx, int gy, Node* p, Brain* b);

      public:
        int x, y;
        Node* previousPtr;
		Brain* parent;

        double moveCost;
        double distanceCost;
        double straightCount;

        Node(int tx, int ty, int gx, int gy, Node* p, Brain* b);
        Node(int tx, int ty, int gx, int gy,          Brain* b);
        Node(int tx, int ty,                 Node* p, Brain* b);
        Node(int tx, int ty,                          Brain* b);
        Node();

        queue<Node> neighbors();
		queue<Node> neighbors(int gx, int gy);
        double cost();

        bool operator()(const Node& a, const Node& b);
    };
	
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
    Brain(int mazeSize, int tLength, double tc, double sc);
    
    stack<Brain::Location> pathBetween(int tx, int ty, int gx, int gy, int startDirection);
    stack<Brain::Location> pathToUnexplored(int tx, int ty, int startDirection);

    bool nextIsExplored(int tx, int ty, int direction);
    bool hasWall(int tx, int ty, int direction);

    double nextXInstruction();
	double nextYInstruction();
    bool instructionsEmpty();
	
    void generateExploreInstruction(int tx, int ty, int direction);
    void generateRouteToInstruction(int tx, int ty, int gx, int gy, int direction);

    void updateMaze(int tx, int ty, bool walls[4]);
    void updateMaze(int n, bool walls[4]);
	
	double minCost(int startX, int startY, int goalX, int goalY);
	double getTurnCost();
	double getStraightCost();
};

#endif