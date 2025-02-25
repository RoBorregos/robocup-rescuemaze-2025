template<typename T, int CAPACITY>
class heap {
private:
    T array[CAPACITY];
    int size;

public:
    heap() : size(0) {}

    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }

    void heapify(int i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && array[left] < array[smallest]) {
            smallest = left;
        }
        if (right < size && array[right] < array[smallest]) {
            smallest = right;
        }
        if (smallest != i) {
            swap(array[i], array[smallest]);
            heapify(smallest);
        }
    }

    void buildHeap(const T* arr, int arrSize) {
        if (arrSize > CAPACITY) {
            throw std::overflow_error("Input array size exceeds heap capacity");
        }
        size = arrSize;
        for (int i = 0; i < size; ++i) {
            array[i] = arr[i];
        }
        for (int i = (size - 1) / 2; i >= 0; --i) {
            heapify(i);
        }
    }

    void insertNode(T value) {
        if (size == CAPACITY) {
            throw std::overflow_error("Heap is full");
        }
        int i = size++;
        array[i] = value;

        while (i != 0 && array[(i - 1) / 2] > array[i]) {
            swap(array[i], array[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    T getMin() const {
        if (size <= 0) {
            throw std::underflow_error("Heap is empty");
        }
        return array[0];
    }

    // ✅ Extracts and Removes the Minimum Element
    T extractMin() {
        if (size <= 0) {
            throw std::underflow_error("Heap is empty");
        }
        if (size == 1) {
            return array[--size];
        }
        T root = array[0];
        array[0] = array[--size];
        heapify(0);
        return root;
    }

    // ✅ New: Removes the Minimum Element (Pop) Without Returning
    void pop() {
        if (size <= 0) {
            throw std::underflow_error("Heap is empty");
        }
        if (size == 1) {
            size--;
            return;
        }
        array[0] = array[--size];
        heapify(0);
    }

    void deleteNode(T key) {
        int index = -1;
        for (int i = 0; i < size; ++i) {
            if (array[i] == key) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            printf("Key not found\n");
            return;
        }
        array[index] = array[--size];
        heapify(index);
    }

    bool isEmpty() const {
        return size == 0;
    }
};
