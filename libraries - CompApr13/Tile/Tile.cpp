#include <Arduino.h>
#include <Tile.h>


Tile::Tile(int a, int b, double s){
  x = a;
  y = b;
  length = s;
  visited = false;
}
Tile::Tile(){}

int Tile::getX(){ return x; }
int Tile::getY(){ return y; }
double Tile::getXCoord(){ return ((double) x)*length; }
double Tile::getYCoord(){ return ((double) y)*length; }

void Tile::setWalls(bool w[4]){
  walls[0] = w[0];
  walls[1] = w[1];
  walls[2] = w[2];
  walls[3] = w[3];
  visited = true;
}
bool Tile::hasWall(int dir){
  return walls[dir];
}
bool Tile::explored(){ return visited; }

double Tile::angleTo(Tile that){
  double dx = that.getX() - x;
  double dy = that.getY() - y;

  if(dx = 0){
    if(dy < 0){ return -90; }
    return 0;
  }

  double angle = atan((float)(dy/dx));

  if(dx < 0){
    if(dy > 0) { angle -= 180; }
    else { angle += 180; }
  }

  return angle;
}
double Tile::distanceTo(Tile that){
  double dx = that.getX() - x;
  double dy = that.getY() - y;

  return sqrt(dx*dx + dy*dy) * length;
}