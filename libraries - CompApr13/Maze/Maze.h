/***********************************************
 * 
 * Probotics - Princeton University
 * 
 * Maze: container array of tiles
 *   tiles have walls, location, visited
 * 
 ***********************************************/


#ifndef Maze_h
#define Maze_h

#include <arduino.h>
#include <Tile.h>

class Maze{
  private:
    int size;
    Tile maze[];
    int toIndex(int tx, int ty);

    Tile goal;

  public:
    Maze(int s, double l);

    void update(int tx, int ty, bool walls[]);
    void update(int n, bool walls[]);
    Tile getTile(int tx, int ty);
    Tile getTile(int n);
    int length();

    bool explored();
    bool hasWall(int tx, int ty, int direction);
    Tile getNeighbor(int tx, int ty, int direction);
};

#endif