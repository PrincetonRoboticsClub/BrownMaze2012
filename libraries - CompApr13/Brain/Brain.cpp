#include <arduino.h>
#include <Tile.h>
#include <Maze.h>

#include <queue>
#include <stack>

#include <Brain.h>


// brain functions

double Brain::xInDir(int tx, int direction){
  if(direction == 0){
    tx ++;
  }
  if(direction == 2){
    tx --;
  }
  return (double) tx * tileLength;
}
double Brain::yInDir(int ty, int direction){
  if(direction == 1){
    ty ++;
  }
  if(direction == 3){
    ty --;
  }
  return (double) ty * tileLength;
}


Brain::Brain(int mazeSize, int tLength, double tc, double sc) 
  : maze(mazeSize, tLength), turnCost(tc), straightCost(sc), tileLength(tLength){}

stack<Brain::Location> Brain::pathBetween(int tx, int ty, int gx, int gy, int startDirection){
  stack<Location> path;

  priority_queue<Node, vector<Node>, Node> pq;

  Brain::Node current (tx, ty, gx, gy, this);

  do{
    queue<Brain::Node> n = current.neighbors(gx, gy);
    while(!n.empty()){
      pq.push(n.front());
	  n.pop();
    }
    current = pq.top();
	pq.pop();
  }while(!(current.x == gx && current.y == gy));
  
  path.push(Location(current.x * tileLength, current.y * tileLength));
  while(current.previousPtr != 0){
    current = *current.previousPtr;
    path.push(Location(current.x * tileLength, current.y * tileLength));
  }

  return path;
}
stack<Brain::Location> Brain::pathToUnexplored(int tx, int ty, int startDirection){
  stack<Location> path;

  priority_queue<Node, vector<Node>, Node> pq;

  Node current (tx, ty, this);

  do{
    queue<Node> n = current.neighbors();
    while(!n.empty()){
      pq.push(n.front());
	  n.pop();
    }
    current = pq.top();
	pq.pop();
  }while(!maze.getTile(current.x, current.y).explored());

  current = *current.previousPtr;
  path.push(Location(current.x * tileLength, current.y * tileLength));
  
  while(current.previousPtr != 0){
    current = *current.previousPtr;
    path.push(Location(current.x * tileLength, current.y * tileLength));
  }

  return path;
}

bool Brain::nextIsExplored(int tx, int ty, int direction){
  if(hasWall(tx, ty, direction)){ return false; }

  return maze.getTile(tx, ty).explored();
}
bool Brain::hasWall(int tx, int ty, int direction){
  return maze.getTile(tx, ty).hasWall(direction);
}

double Brain::nextXInstruction(){
  double x = xInstructions.front();
  xInstructions.pop();
  return x;
}
double Brain::nextYInstruction(){
  double y = yInstructions.front();
  yInstructions.pop();
  return y;
}
bool Brain::instructionsEmpty(){
  return xInstructions.empty();
}

void Brain::generateExploreInstruction(int tx, int ty, int direction){
  if(!nextIsExplored(tx, ty, direction)){
    xInstructions.push(xInDir(tx, direction));
    yInstructions.push(yInDir(ty, direction));
    return;
  }
  int dir = (direction + 1)%4;
  if(!nextIsExplored(tx, ty, dir)){
    xInstructions.push(xInDir(tx, dir));
    yInstructions.push(yInDir(ty, dir));
    return;
  }
  dir = (direction + 3)%4;
  if(!nextIsExplored(tx, ty, dir)){
    xInstructions.push(xInDir(tx, dir));
    yInstructions.push(yInDir(ty, dir));
    return;
  }
  
  stack<Location> path = pathToUnexplored(tx, ty, direction);
  
  while(!path.empty()){
    xInstructions.push(path.top().getX());
    yInstructions.push(path.top().getY());
	path.pop();
  }
}
void Brain::generateRouteToInstruction(int tx, int ty, int gx, int gy, int direction){
  stack<Location> path = pathBetween(tx, ty, gx, gy, direction);
  
  while(!path.empty()){
    xInstructions.push(path.top().getX());
    yInstructions.push(path.top().getY());
	path.pop();
  }
}

void Brain::updateMaze(int tx, int ty, bool walls[4]){
  maze.getTile(tx, ty).setWalls(walls);
}
void Brain::updateMaze(int n, bool walls[4]){
  maze.getTile(n).setWalls(walls);
}

double Brain::minCost(int startX, int startY, int goalX, int goalY){
  return 0;  // NOT DONE
}
double Brain::getTurnCost(){ return turnCost; }
double Brain::getStraightCost(){ return straightCost; }


// Node functions

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

queue<Brain::Node> Brain::Node::neighbors(){
  return neighbors(-1, -1);
}
queue<Brain::Node> Brain::Node::neighbors(int gx, int gy){
  queue<Brain::Node> neighbors;
  
  if(!(*parent).hasWall(x, y, 0)){ neighbors.push(Node(x + 1, y, gx, gy, this, parent)); }
  if(!(*parent).hasWall(x, y, 1)){ neighbors.push(Node(x, y + 1, gx, gy, this, parent)); }
  if(!(*parent).hasWall(x, y, 2)){ neighbors.push(Node(x - 1, y, gx, gy, this, parent)); }
  if(!(*parent).hasWall(x, y, 3)){ neighbors.push(Node(x, y - 1, gx, gy, this, parent)); }
  
  return neighbors;
}
double Brain::Node::cost(){
  return moveCost + distanceCost;
}

bool Brain::Node::operator()(const Node& a, const Node& b){
  return a.moveCost + a.distanceCost < b.moveCost + b.distanceCost;
}


// Location functions

Brain::Location::Location(double dx, double dy) : x(dx), y(dy) {}
Brain::Location::Location(){}

double Brain::Location::getX(){ return x; }
double Brain::Location::getY(){ return y; }