#include <Arduino.h>
#include <Tile.h>
#include <Maze.h>

#include <queue>
#include <stack>

#include <Brain.h>


/*---------------------------------------------------------
  Brain functions - control instruction queue
---------------------------------------------------------*/

// get x coordinate in a direction
double Brain::xInDir(int tx, int direction){
  if(direction == 0){
    tx ++;
  }
  if(direction == 2){
    tx --;
  }
  return (double) tx * tileLength;
}
// get y coordinate in a direction
double Brain::yInDir(int ty, int direction){
  if(direction == 1){
    ty ++;
  }
  if(direction == 3){
    ty --;
  }
  return (double) ty * tileLength;
}

// constructor
Brain::Brain(int mazeSize, int tLength, double tc, double sc) 
  : maze(mazeSize, tLength), turnCost(tc), straightCost(sc), tileLength(tLength){}

//---------------------------------------------------------
  
// get the path between start(tx, ty) and goal(gx, gy) tiles
stack<Brain::Location> Brain::pathBetween(int tx, int ty, int gx, int gy, int startDirection){
  stack<Location> path;

  // for A* algorithm
  priority_queue<Node, vector<Node>, Node> pq;
  Brain::Node current (tx, ty, gx, gy, this);

  do{
    // add all neighbors of current node to open list
    queue<Brain::Node> n = current.neighbors(gx, gy);
    while(!n.empty()){
      pq.push(n.front());
	  n.pop();
    }
    // make current node the node with lowest heuristic score in open list
    current = pq.top();
	pq.pop();
  }while(!(current.x == gx && current.y == gy));
  
  // build the path by following the previous ptrs from goal to start
  path.push(Location(current.x * tileLength, current.y * tileLength));
  while(current.previousPtr != 0){
    current = *current.previousPtr;
    path.push(Location(current.x * tileLength, current.y * tileLength));
  }

  return path;
}
// get the path from start(tx, ty) to the nearest unexplored tile
stack<Brain::Location> Brain::pathToUnexplored(int tx, int ty, int startDirection){
  stack<Location> path;

  // for breadth first search (can't use A* because no goal node specified)
  priority_queue<Node, vector<Node>, Node> pq;
  Node current (tx, ty, this);

  do{
    // add all neighbors of current node to open list
    queue<Node> n = current.neighbors();
    while(!n.empty()){
      pq.push(n.front());
	  n.pop();
    }
    // make current node the node with lowest heuristic score in open list
    current = pq.top();
	pq.pop();
  }while(!maze.getTile(current.x, current.y).explored());

  // build the path by following the previous ptrs from goal to start
  current = *current.previousPtr;
  path.push(Location(current.x * tileLength, current.y * tileLength));
  
  while(current.previousPtr != 0){
    current = *current.previousPtr;
    path.push(Location(current.x * tileLength, current.y * tileLength));
  }

  return path;
}

//---------------------------------------------------------

// is the next tile in a direction explored
bool Brain::nextIsExplored(int tx, int ty, int direction){
  if(hasWall(tx, ty, direction)){ return false; }

  return maze.getTile(tx, ty).explored();
}
// is there a wall in a direction
bool Brain::hasWall(int tx, int ty, int direction){
  return maze.getTile(tx, ty).hasWall(direction);
}

// get the x-coordinate of the next instruction
double Brain::nextXInstruction(){
  double x = xInstructions.front();
  xInstructions.pop();
  return x;
}
// get the y-coordinate of the next instruction
double Brain::nextYInstruction(){
  double y = yInstructions.front();
  yInstructions.pop();
  return y;
}
// are there no more instructions currently queued
bool Brain::instructionsEmpty(){
  return xInstructions.empty();
}

//---------------------------------------------------------

// create a new instruction set that explores a tile in the maze
void Brain::generateExploreInstruction(int tx, int ty, int direction){
  // explore tile in front if possible and unexplored
  if(!nextIsExplored(tx, ty, direction)){
    xInstructions.push(xInDir(tx, direction));
    yInstructions.push(yInDir(ty, direction));
    return;
  }
  // explore tile to the right if possible and unexplored
  int dir = (direction + 1)%4;
  if(!nextIsExplored(tx, ty, dir)){
    xInstructions.push(xInDir(tx, dir));
    yInstructions.push(yInDir(ty, dir));
    return;
  }
  // explore tile to the left if possible and unexplored
  dir = (direction + 3)%4;
  if(!nextIsExplored(tx, ty, dir)){
    xInstructions.push(xInDir(tx, dir));
    yInstructions.push(yInDir(ty, dir));
    return;
  }
  
  // find the next closest unexplored tile and add the set of
  // all tiles to the instruction queue
  stack<Location> path = pathToUnexplored(tx, ty, direction);
  
  while(!path.empty()){
    xInstructions.push(path.top().getX());
    yInstructions.push(path.top().getY());
	path.pop();
  }
}
// create a new instruction set that goes from start(tx, ty) to goal(gx, gy)
void Brain::generateRouteToInstruction(int tx, int ty, int gx, int gy, int direction){
  // use A*...
  stack<Location> path = pathBetween(tx, ty, gx, gy, direction);
  
  while(!path.empty()){
    xInstructions.push(path.top().getX());
    yInstructions.push(path.top().getY());
	path.pop();
  }
}

//---------------------------------------------------------

// update the wall locations of a newly explored tile
void Brain::updateMaze(int tx, int ty, bool walls[4]){
  maze.getTile(tx, ty).setWalls(walls);
}
// update the wall locations of a newly explored tile
void Brain::updateMaze(int n, bool walls[4]){
  maze.getTile(n).setWalls(walls);
}

// not sure what I was going for, but probably unnecessary
double Brain::minCost(int startX, int startY, int goalX, int goalY){
  return 0;  // NOT DONE
}
double Brain::getTurnCost(){ return turnCost; }
double Brain::getStraightCost(){ return straightCost; }


/*---------------------------------------------------------
  Node functions - elements of A* algorithm
---------------------------------------------------------*/

// used by constructor
void Brain::Node::init(int tx, int ty, int gx, int gy, Node* p, Brain* b){
  x = tx;
  y = ty;
  previousPtr = p;
  parent = b;
  
  moveCost = (*p).moveCost + (*b).getStraightCost();
  if((*p).previousPtr != 0 && p != 0){
    if(   (*p).x - x != (*(*p).previousPtr).x - (*p).x
	   && (*p).y - y != (*(*p).previousPtr).y - (*p).y){
	  moveCost += (*b).getTurnCost();
	}
  }
  
  if(gx < 0 || gy < 0){
    distanceCost = 0;
  }
  else{
    distanceCost = (*b).getStraightCost()*(abs(gx - tx) + abs(gy - ty)) 
	     + 2 * (*b).getTurnCost();
  }
  // update move cost
}

// constructors... some use a goal location / parent node
Brain::Node::Node(int tx, int ty, int gx, int gy, Node* p, Brain* b){
  init(tx, ty, gx, gy, p, b);
}
Brain::Node::Node(int tx, int ty, int gx, int gy, Brain* b){
  init(tx, ty, gx, gy, 0, b);
}
Brain::Node::Node(int tx, int ty, Node* p, Brain* b){
  init(tx, ty, -1, -1, p, b);
}
Brain::Node::Node(int tx, int ty, Brain* b){
  init(tx, ty, -1, -1, 0, b);
}
Brain::Node::Node(){}

// give empty neighbor set
queue<Brain::Node> Brain::Node::neighbors(){
  return neighbors(-1, -1);
}
// give a set of neighboring nodes that can be reached from this node
queue<Brain::Node> Brain::Node::neighbors(int gx, int gy){
  queue<Brain::Node> neighbors;
  
  if(!(*parent).hasWall(x, y, 0)){ neighbors.push(Node(x + 1, y, gx, gy, this, parent)); }
  if(!(*parent).hasWall(x, y, 1)){ neighbors.push(Node(x, y + 1, gx, gy, this, parent)); }
  if(!(*parent).hasWall(x, y, 2)){ neighbors.push(Node(x - 1, y, gx, gy, this, parent)); }
  if(!(*parent).hasWall(x, y, 3)){ neighbors.push(Node(x, y - 1, gx, gy, this, parent)); }
  
  return neighbors;
}
// calculate the total heuristic cost of this node
double Brain::Node::cost(){
  return moveCost + distanceCost;
}

// comparison operator between 2 nodes
bool Brain::Node::operator()(const Node& a, const Node& b){
  return a.moveCost + a.distanceCost < b.moveCost + b.distanceCost;
}


// Location functions (not necessary)

Brain::Location::Location(double dx, double dy) : x(dx), y(dy) {}
Brain::Location::Location(){}

double Brain::Location::getX(){ return x; }
double Brain::Location::getY(){ return y; }