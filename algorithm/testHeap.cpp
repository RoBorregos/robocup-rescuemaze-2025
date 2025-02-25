// C++ Program for implementing Min Heap
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// Template for representing MinHeap
template <typename T>
class MinHeap {
private:
    vector<T> array;  
    int size;         
    int capacity;     

public:
    // Constructor to set the heap's initial capacity
    MinHeap(int capacity) {
        this->size = 0;
        this->capacity = capacity;
        this->array.resize(capacity);
    }
    
    // Function to restore heap order at index i
    void heapify(int i) {
        // Initialize smallest as root
        int smallest = i;    
        //  Find the Left child index
        int left = 2 * i + 1;    
         // Find the Right child index
        int right = 2 * i + 2;   
        
        // If left child is smaller than root
        if (left < size && array[left] < array[smallest])
            smallest = left;

        // If right child is smaller than the smallest so far
        if (right < size && array[right] < array[smallest])
            smallest = right;

        // If smallest is not root
        if (smallest != i) {
            swap(array[i], array[smallest]);  
            heapify(smallest);                
        }
    }

    // Function to create a min heap from a given array
    void buildHeap(const vector<T>& arr) {
        capacity = arr.size();
        size = capacity;
        array = arr;
        
        // Heapify the (n-1)/2 nodes
        for (int i = (size - 1) / 2; i >= 0; i--) {
            heapify(i);
        }
    }

    // Function to insert a new node into the min heap
    void insertNode(T value) {
        if (size == capacity) {
            // Resize the heap if necessary
            capacity *= 2;
            array.resize(capacity);
        }

        size++;
        int i = size - 1;
        array[i] = value;

        while (i != 0 && array[(i - 1) / 2] > array[i]) {
            swap(array[i], array[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    // Function to get the topmost value from the min heap
    T peek() {
        if (size <= 0)
            return -1;  // Indicates that the heap is empty

        return array[0];
    }

    // Function to remove and return the minimum value from the heap
    T extractMin() {
        if (size <= 0)
            return -1;  // Indicates that the heap is empty
        if (size == 1) {
            size--;
            return array[0];
        }

        // Store the minimum value, and remove it
        T root = array[0];
        array[0] = array[size - 1];
        size--;
        // Heapify the root node after deletion
        heapify(0);  
        return root;
    }

    // Function to delete a specific node from the heap
    void DeleteNode(T key) {
        // Find the index of the key
        int index = -1;
        for (int i = 0; i < size; ++i) {
            if (array[i] == key) {
                index = i;
                break;
            }
        }
        // If key is not found, return
        if (index == -1) {
            cout << "Key not found" << endl;
            return;
        }

        if (index == size - 1) {
            size--;
            return;
        }

        // Move the last element to the index of the key to be deleted
        array[index] = array[size - 1];
        size--;

        // Heapify down to heapify the rest of the min heap
        heapify(index);
    }

    // Function to print the values of  the  min heap
    void printHeap() const {
        for (int i = 0; i < size; ++i)
            cout << array[i] << " ";
        cout << endl;
    }
};

int main() {
    // Initialize a MinHeap with initial size of 6
    MinHeap<int> minHeap(6);
    vector<int> arr = {15, 10, 5, 4, 3, 2};

    // Build the heap from the array
    minHeap.buildHeap(arr);

    // Print the min heap
    cout<<"Min Heap built from the array: ";
    minHeap.printHeap();

    // Insert a node into the heap
    minHeap.insertNode(1);
    cout << "Min Heap After inserting the node 1: ";
    minHeap.printHeap();

    // Get the minimum value from the min heap
    cout << "Topmost value of min Heap: " << minHeap.peek() << endl;

    // Delete the root node of the min heap
    cout << "Minimum Extracted value fro Min Heap: " << minHeap.extractMin() << endl;
    cout << "After extracting Min Heap: ";
    minHeap.printHeap();

    // Deleting  a specific value from the min heap
    minHeap.DeleteNode(4);
    cout << "After deleting the node 4 from the Min Heap: ";
    minHeap.printHeap();

    return 0;
}
