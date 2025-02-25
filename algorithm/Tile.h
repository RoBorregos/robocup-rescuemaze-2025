#ifndef Tile_h
#define Tile_h

#include "coord.h"
#include <cstdint>
#include "TileDirection.h"

// Bits 0-3 are reserved for the walls. 
constexpr int kVictimBit = 4;
constexpr int kObstacleBit = 5;
constexpr int kBlackTileBit = 6;
constexpr int kCheckpointBit = 7;

constexpr int kNumberOfDirections = 4;

constexpr int kWallTileWeight = 100;
constexpr int kWhiteTileWeight = 1;// min weight
constexpr int kBlueTileWeight = 3; // 5 seconds
constexpr int kRampWeight = 5;

constexpr coord kInvalidPosition = coord{1000,1000,1000};

class Tile{
    public:
        // TODO: SAVE RAMP INFORMATION.
        coord position_;
        Tile *adjacentTiles_[kNumberOfDirections];
        int weights_[kNumberOfDirections];
        char data_;
        Tile();
        Tile(const coord& position);
        void addAdjacentTile(const TileDirection direction, Tile *tile, const bool wall);
        void setPosition(const coord& position);
        void setWall(const TileDirection direction, const bool wall);
        bool hasWall(const TileDirection direction) const;
        bool hasVictim() const;
        void setVictim();
        bool hasObstacle() const;
        void setObstacle();
        bool hasBlackTile() const;
        void setBlackTile();
        bool hasCheckpoint() const;
        void setCheckpoint();
};

#endif