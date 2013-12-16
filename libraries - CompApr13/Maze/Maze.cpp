#include <arduino.h>
#include <Tile.h>
#include <Maze.h>

Maze::Maze(int s, double l){
  size = s;
  maze[s*s];
  
  for(int n=0; n<s*s; n++){
    maze[n] = Tile(n/s, n%s, l);
  }
}

int Maze::toIndex(int tx, int ty){
  return tx + ty*size;
}

void Maze::update(int tx, int ty, bool walls[]){
  update(toIndex(tx, ty), walls);
}
void Maze::update(int n, bool walls[]){
  maze[n].setWalls(walls);
}
Tile Maze::getTile(int tx, int ty){
  return getTile(toIndex(tx, ty));
}
Tile Maze::getTile(int n){
  return maze[n];
}
int Maze::length(){ return size; }

bool Maze::hasWall(int tx, int ty, int direction){
  return getTile(tx, ty).hasWall(direction);
}
Tile Maze::getNeighbor(int tx, int ty, int direction){
  int x = tx;
  int y = ty;
  
  if(direction == 0){ x++; }
  if(direction == 1){ y++; }
  if(direction == 2){ x--; }
  if(direction == 3){ y--; }
  
  return getTile(x, y);
}