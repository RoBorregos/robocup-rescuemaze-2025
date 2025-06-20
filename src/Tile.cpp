#include "algs/Tile.h"

Tile::Tile(){
    this->position_ = kInvalidPosition;

    for(uint8_t i = 0; i < kNumberOfDirections; ++i){
        adjacentTiles_[i] = nullptr;
        weights_[i] = 0;    
    }

    this->data_ = '\0';
}

Tile::Tile(const coord& position) {
    this->position_ = position;
    for(uint8_t i = 0; i < kNumberOfDirections; ++i){
        adjacentTiles_[i] = nullptr;
        weights_[i] = 0;    
    }

    this->data_ = '\0';
}

void Tile::setWall(const TileDirection direction, const bool wall) {
    if (wall) {
        this->data_ |= (1 << static_cast<int>(direction));
    }
}

bool Tile::hasWall(const TileDirection direction) const {
    return this->data_ & (1 << static_cast<int>(direction));
}

void Tile::setVictim() {
    this->data_ |= (1 << kVictimBit);
}

bool Tile::hasVictim() const {
    return this->data_ & (1 << kVictimBit);
}

void Tile::setObstacle() {
    this->data_ |= (1 << kObstacleBit);
}

bool Tile::hasObstacle() const {
    return this->data_ & (1 << kObstacleBit);
}

void Tile::setBlackTile() {
    this->data_ |= (1 << kBlackTileBit);
}

bool Tile::hasBlackTile() const {
    return this->data_ & (1 << kBlackTileBit);
}

void Tile::setCheckpoint() {
    this->data_ |= (1 << kCheckpointBit);
}

bool Tile::hasCheckpoint() const {
    return this->data_ & (1 << kCheckpointBit);
}

void Tile::addAdjacentTile(const TileDirection direction, Tile *tile, const bool wall, const bool blue) {
    adjacentTiles_[static_cast<int>(direction)] = tile;
    this->setWall(direction, wall);
    if(wall){
        weights_[static_cast<int>(direction)] = kWallTileWeight;
    } else if(blue){
        weights_[static_cast<int>(direction)] = kBlueTileWeight;
    }else {
        weights_[static_cast<int>(direction)] = kWhiteTileWeight;
    }
}

void Tile::setPosition(const coord& position) {
    this->position_ = position;
}