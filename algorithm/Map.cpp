#include "coord.h"
#include <vector>
using namespace std;
class Map{
    public:
        Map();
        int getIndex(const coord& position) const;
        vector<coord> positions;
        const int kInvalidIndex = -1;
};
Map::Map(){
    // do nothing
};
int Map::getIndex(const coord &position) const {
    for (uint8_t i = 0; i < this->positions.size(); ++i) {
        if (this->positions[i] == position) {
            return i;
        }
    }
    return kInvalidIndex;
}
