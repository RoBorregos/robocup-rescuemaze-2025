#ifndef TILE_H
#define TILE_H

#include <iostream>
using namespace std; 
#include "coord.h"

enum class Directions{
    up = 0,
    down = 1,
    left = 2,
    right = 3,
    none
};
// bits for the data_ member
const int VictimTileB =4;
const int ObstacleTileB = 5;
const int BlackTileB = 6;
const int CheckPointTileB = 7;

const int MinWeight = 1; 
const int NumDirections = 4;
constexpr coord InvalidPosition = coord{1000,1000,1000};

class Tile{
    public: 
        coord position_;
        int weights_[NumDirections];
        char data_;
        Tile *adjacentTiles_[NumDirections];
        Tile(const coord& position);
        Tile();
        void addAdjacentTile(const Directions direction, Tile *tile, const bool wall);
        void setPosition(const coord& position);
        void setWall(const Directions direction, const bool wall);
        bool hasWall(const Directions direction) const;
        bool hasVictim();
        void setViction();
        bool hasObstacle();
        void setObstacle();
        bool hasBlackTile();
        void setBlackTile();
        bool hasCheckpoint();
        void setCheckpoint();
};

#endif