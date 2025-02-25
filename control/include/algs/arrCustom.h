// CustomArray.h
template <typename T>
class arrCustom {
private:
    uint8_t i = 0;
    size_t size;
    T absValue;
    T* positions;

public:
    // Constructor
    arrCustom(size_t size, T value) : size(size), absValue(value) {
        if (size <= 0) {
            this->size = 0;
            // positions = nullptr;
            positions = new T[0];
            return;
        }
        positions = new T[size]; 
        for(uint8_t i = 0; i < size; i++){
            positions[i] = value;
        }
    }
    void set(size_t index, T value) {
        if (index < size && index >= 0) {
            positions[index] = value;
        }
    }
    uint8_t getIndex(T value){
        for(uint8_t i = 0; i < size; i++){
            if(positions[i] == value){
                return i;
            }
        }
        return 255;
    }
    T& getValue(size_t index) const {
        if (index < size && index >= 0) {
            return positions[index];
        }
    }
    size_t getSize() const {
        return size;
    }
    //just for som arrays using push_back
    void push_back(T position) {
        if (i >= size) {

            // correct this
            size_t newSize = size == 0 ? 1 : size * 2;
            T* newPositions = new T[newSize];
            for (size_t j = 0; j < size; j++) {
                newPositions[j] = positions[j];
            }
            delete[] positions;
            positions = newPositions;
            size = newSize;
        }
        positions[i] = position;
        i++;
    }
};