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

stack<double[2]> Brain::pathBetween(int tx, int ty, int gx, int gy, int startDirection){
  stack<double[2]> path;

  priority_queue<Brain::Node> pq;

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
  
  double coords[2] = {current.x * tileLength, current.y * tileLength};
  path.push(coords);
  while(current.previousPtr != 0){
    current = *current.previousPtr;
	coords = {current.x * tileLength, current.y * tileLength};
    path.push(coords);
  }

  return path;
}

stack<double[2]> Brain::pathToUnexplored(int tx, int ty, int startDirection){
  stack<double[2]> path;

  priority_queue<Node> pq;

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

  double coords[2] = {current.x * tileLength, current.y * tileLength};
  path.push(coords);
  while(current.previousPtr != 0){
    current = *current.previousPtr;
	coords = {current.x * tileLength, current.y * tileLength};
    path.push(coords);
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

void Brain::nextInstruction(double p[2]){
  p[0] = xInstructions.front();
  xInstructions.pop();
  p[1] = yInstructions.front();
  xInstructions.pop();
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
  
  stack<double[2]> path = pathToUnexplored(tx, ty, direction);
  
  while(!path.empty()){
    xInstructions.push(path.top()[0]);
    yInstructions.push(path.top()[1]);
	path.pop();
  }
}
void Brain::generateRouteToInstruction(int tx, int ty, int gx, int gy, int direction){
  stack<double[2]> path = pathBetween(tx, ty, gx, gy, direction);
  
  while(!path.empty()){
    xInstructions.push(path.top()[0]);
    yInstructions.push(path.top()[1]);
	path.pop();
  }
}

void Brain::updateMaze(int tx, int ty, bool walls[]){
  maze.getTile(tx, ty).setWalls(walls);
}
void Brain::updateMaze(int n, bool walls[]){
  maze.getTile(n).setWalls(walls);
}



// Node functions

void Brain::Node::init(int tx, int ty, int gx, int gy, Node* p, Brain* b){
  x = tx;
  y = ty;
  previousPtr = p;
  parent = b;

  if(gx < 0 || gy < 0){
    distanceCost = 0;
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

bool Brain::Node::operator()(Node a, Node b) const{
  return a.cost() < b.cost();
}
bool Brain::Node::operator<(Node that){
  return cost() < that.cost();
}


// Location functions

Brain::Location::Location(int tx, int ty) : x(tx), y(ty) {}
Brain::Location::Location(){}

Brain::Location::getX(){ return x; }
Brain::Location::getY(){ return y; }