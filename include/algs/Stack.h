#include "coord.h"
#include "Tile.h"
class Stack{
    private: 
        uint8_t mark;
        coord stack[50];
    public:
        Stack(){
            mark = 0;
            for(int i = 0; i < 50; i++){
                stack[i] = kInvalidPosition;
            }
        }
        void push(coord value){
            stack[mark] = value;
            mark++;
        }
        void pop(){
            if(mark >= 0){
                stack[mark] = kInvalidPosition;
                mark--;
            }
        }
        coord top(){
            if(mark>=0){
                return stack[mark - 1];
            }
            return kInvalidPosition;
        }
        bool empty(){
            return mark <= 0;
        }
        uint8_t getSize(){
            return mark;
        }
        void clear(){
            mark = 0;
        }
};