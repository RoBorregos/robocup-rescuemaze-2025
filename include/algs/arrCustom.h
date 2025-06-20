template <typename T>
class arrCustom {
private:
    uint8_t i = 0;
    size_t size;
    T* positions;

public:
    // Constructor
    arrCustom(size_t size, T value){
        this->size = size;
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
        }else{
            Serial.println("Index out of bounds");
        }
    }
    uint8_t getIndex(T value){
        for(uint8_t j = 0; j < i; j++){ // ← usa i en lugar de size
            if(positions[j] == value){
                return j;
            }
        }
        return 255;
    }
    T& getValue(size_t index) const {
        
        if (index < size && index >= 0) {
            return positions[index];
        }
        else{
            Serial.println("Index out of bounds");
            return positions[size-1]; // o maneja el error de otra manera
        }
        
    }
    size_t getSize() const {
        return size;
    }
    //just for som arrays using push_back
    void push_back(T position) {
        if (i >= size) {
            // correct this
            T* temp = new T[size + 1];
            for (uint8_t j = 0; j < size; j++) {
                temp[j] = positions[j];
            }
            delete[] positions;
            positions = temp;
            size++;
        }
        positions[i] = position;
        i++;
    }
    void reset() {
        i = 0;
        for (uint8_t j = 0; j < size; j++) {
            positions[j] = T();
        }        
    }
};