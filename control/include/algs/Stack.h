#include "coord.h"
#include "Tile.h"
/*
class Stack{
    private: 
        uint8_t mark;
        coord stack[kMaxStackSize];
    public:
        Stack(){
            mark = 0;
            for(int i = 0; i < kMaxStackSize; i++){
                stack[i] = kInvalidPosition;
            }
        }
        void push(coord value){
            if (mark < kMaxStackSize && mark >= 0){
                stack[mark] = value;
                mark++;
            } else {
                // Handle stack overflow error
                // You can add an error message or handle it as needed
                printf("Stack overflow\n");
            }
        }
        void pop(){
            if(mark >= 0 && mark < kMaxStackSize){
                stack[mark] = kInvalidPosition;
                mark--;
            }else{
                // Handle stack underflow error
                // You can add an error message or handle it as needed
                printf("Stack underflow\n");
            }
        }
        coord top(){
            if(mark>=0 && mark < kMaxStackSize){
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
*/
// implementing pointers and nodes for dynamic memory allocation and implementing a linked list.

#include "coord.h"
#include "Tile.h"
// #include <iostream>

struct Node {
    coord data;
    Node* next;
};

class Stack {
private:
    Node* topNode;
    uint8_t mark;

public:
    Stack() : topNode(nullptr), mark(0) {}

    ~Stack() {
        while (!empty()) {
            pop();
        }
    }

    void push(coord value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = topNode;
        topNode = newNode;
        mark++;
    }

    void pop() {
        if (empty()) {
            // std::cout << "Stack underflow\n";
            return;
        }
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
        mark--;
    }

    coord top() {
        if (empty()) {
            return kInvalidPosition;
        }
        return topNode->data;
    }

    bool empty() {
        return topNode == nullptr;
    }

    uint8_t getSize() {
        return mark;
    }

    void clear() {
        while (!empty()) {
            pop();
        }
    }
};
