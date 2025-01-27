#include "Tile.cpp"
class vectorT{
    public:
        vectorT();
        int i = 0;
        void push_back(Tile tile);
        int getIndex(const Tile& tile) const;
        Tile tiles[256];
        const int kInvalidIndex = -1;
};
vectorT::vectorT(){
    // do nothing
};
int vectorT::getIndex(const Tile &tile) const {
    for (uint8_t i = 0; i < 256; ++i) {
        if (this->tiles[i].position_ == tile.position_) {
            return i;
        }
    }
    return kInvalidIndex;
}
void vectorT::push_back(Tile tile){
    this->tiles[i] = tile;
    i++;
}
