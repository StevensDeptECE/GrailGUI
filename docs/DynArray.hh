#include <iostream>

template<typename T> 
class DynArray {
private:
    T* data;
    uint32_t size;
    uint32_t capacity;
    void checkgrow() {
        if (size == capacity) {
            capacity *= 2;
            T* old = data;
            data = new T[capacity];
            // TODO: this is horribly inefficient, shame on Codeium!
            for (uint32_t i = 0; i < size; i++) {
                newData[i] = data[i];
            }
            delete[] old;
        }
    }
public:
    DynArray() : data(nullptr), size(0), capacity(0) {}
    ~DynArray() {
        delete[] data;
    }
    DynArray(const DynArray<T>& orig) : data(nullptr), size(0), capacity(0) {
        data = new T[orig.capacity];
        for (uint32_t i = 0; i < orig.size; i++) {
            data[i] = orig.data[i];
        }
        size = orig.size;
        capacity = orig.capacity;
    }
    DynArray<T>& operator=(DynArray<T> copy) {
        swap(data, copy.data);
        size = copy.size;
        capacity = copy.capacity;
        return *this;
    }
        
    T& operator[](uint32_t index) {
        return data[index];
    }
    void addEnd(const T& value) {
        checkgrow();
        data[size++] = value;
    }
    void send_additions(uint32_t index) {
        
    }
};