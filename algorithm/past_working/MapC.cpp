#include "coord.h"
class MapC{
    public:
        MapC();
        int i = 0;
        void push_back(coord position);
        int getIndex(const coord& position) const;
        coord positions[256];
        const int kInvalidIndex = -1;
};
MapC::MapC(){
    // do nothing
};
int MapC::getIndex(const coord &position) const {
    for (uint8_t i = 0; i < 256; ++i) {
        if (this->positions[i] == position) {
            return i;
        }
    }
    return kInvalidIndex;
}
void MapC::push_back(coord position){
    this->positions[i] = position;
    i++;
}
