/***********************************************
 * 
 * Probotics - Princeton University
 * Tile: stores location and walls (maze element)
 * 
 ***********************************************/


#ifndef Tile_h
#define Tile_h

#include <Arduino.h>


class Tile{
  private:
    int x;
    int y;
    int length;
    bool walls[4];
    bool visited;

  public:
    Tile(int a, int b, double s);
	Tile();

    int getX();
    int getY();
    double getXCoord();
    double getYCoord();
    
    void setWalls(bool w[]);
    bool hasWall(int dir);
    bool explored();

    double angleTo(Tile that);
    double distanceTo(Tile that);
};

#endif