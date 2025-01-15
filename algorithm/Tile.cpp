#include <iostream>
#include <map>
using namespace std;

#include "Tile.h"

Tile::Tile(){
    this->position_ = InvalidPosition;

    for(int i =0; i < NumDirections; ++i){
        adjacentTiles_[i] = NULL;
        weights_[i] = 0;
    }
    //all bits are 0
    this -> data_ = '\0';

}
Tile::Tile(const coord& position){
    this->position_ = position;

    for(int i =0; i < NumDirections; ++i){
        adjacentTiles_[i] = NULL;
        weights_[i] = 0;
    }
    // all bits are 0
    this -> data_ = '\0';
}

// or | for joining bits
// and & for checking bits in both of them(in this case, checking data_)

void Tile::setWall(const Directions direction, const bool wall){
    if(wall){
        this -> data_ |= (1 << static_cast<int>(direction));
    }
}
bool Tile::hasWall(const Directions direction) const{
    // check if the bit is set
    return this -> data_ & (1 << static_cast<int>(direction));
}
void Tile::setViction(){
    this -> data_ |= (1 << VictimTileB);
}
bool Tile::hasVictim(){
    return this -> data_ & (1 << VictimTileB);
}
void Tile::setObstacle(){
    this -> data_ |= (1 << ObstacleTileB);
}
bool Tile::hasObstacle(){
    return this -> data_ & (1 << ObstacleTileB);
}
void Tile::setBlackTile(){
    this -> data_ |= (1 << BlackTileB);
}
bool Tile::hasBlackTile(){
    return this -> data_ & (1 << BlackTileB);
}   
void Tile::setCheckpoint(){
    this -> data_ |= (1 << CheckPointTileB);
}
bool Tile::hasCheckpoint(){
    return this -> data_ & (1 << CheckPointTileB);
}

void Tile::addAdjacentTile(const Directions direction, Tile *tile, const bool wall){
    adjacentTiles_[static_cast<int>(direction)] =tile;
    weights_[static_cast<int>(direction)] = MinWeight;
    this -> setWall(direction, wall);

}

void Tile::setPosition(const coord& position){
    this -> position_ = position;
}