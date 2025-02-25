#include "coord.h"
#include "Tile.h"
class Stack{
    private: 
        int mark;
        coord stack[20];
    public:
        Stack(){
            mark = 0;
            for(int i = 0; i < 20; i++){
                stack[i] = kInvalidPosition;
            }
        }
        void push(coord value){
            if (mark < 20) {
                stack[mark] = value;
                mark++;
            } else {
                if(mark > 0){
                    throw std::overflow_error("Stack overflow: cannot push to a full stack");
                }
            }
        }
        void pop(){
            if(mark > 0){
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
        int getSize(){
            return mark;
        }
        void clear(){
            mark = 0;
        }
};