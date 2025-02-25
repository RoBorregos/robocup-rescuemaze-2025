// CustomArray.h
template <typename T>
class arrCustom {
private:
    int i = 0;
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
        for(int i = 0; i < size; i++){
            positions[i] = value;
        }
    }
    void set(size_t index, T value) {
        if (index < size && index >= 0) {
            positions[index] = value;
        }
    }
    int getIndex(T value){
        for(int i = 0; i < size; i++){
            if(positions[i] == value){
                return i;
            }
        }
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
    void push_back(T position){
        this->positions[i] = position;
        i++;
    }
};

