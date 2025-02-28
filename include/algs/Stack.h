
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
