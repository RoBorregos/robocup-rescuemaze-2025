#ifndef COORD_H  // Check if COORD_H is already defined
#define COORD_H
#include "Arduino.h"

struct coord{
    uint8_t x;
    uint8_t y;
    uint8_t z;
    bool const operator==(const coord &o) const {
        return x == o.x && y == o.y && z == o.z;
    }
    bool const operator!=(const coord &o) const {
        return x != o.x || y != o.y || z != o.z;
    }
    bool const operator<(const coord &o) const {
        return x < o.x || (x == o.x && y < o.y) || (x == o.x && y == o.y && z < o.z);
    }
};
#endif