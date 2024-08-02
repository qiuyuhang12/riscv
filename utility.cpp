#include "utility.hpp"
#include "load_store_buffer.hpp"
#include "reorder_buffer.hpp"

int sext(int imm, int len) {
    if (len == 32) {
        return imm;
    }
    if ((imm >> (len - 1)) & 1) {
        long long tmp = (long long) imm | (long long) (0xFFFFFFFF << len);
        return tmp;
    }
    return imm;
}

extern int clo;

template<typename T>
CircularQueue<T>::CircularQueue() : frontIndex(0), rearIndex(-1), currentSize(0) {}

template<typename T>
bool CircularQueue<T>::enqueue(const T &value) {
    if (isFull()) {
        return false;
    }
    rearIndex = (rearIndex + 1) % capacity;
    arr[rearIndex] = value;
    currentSize++;
    return true;
}

template<typename T>
int CircularQueue<T>::getNextRearIndex() {
    return (rearIndex + 1) % capacity;
}

template<typename T>
int CircularQueue<T>::size() const {
    return currentSize;
}

template<typename T>
bool CircularQueue<T>::dequeue() {
    if (isEmpty()) {
        return false;
    }
    front() = T();
    frontIndex = (frontIndex + 1) % capacity;
    currentSize--;
    return true;
}

template<typename T>
T &CircularQueue<T>::front() {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    return arr[frontIndex];
}

template<typename T>
T &CircularQueue<T>::rear() {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty");
    }
    return arr[rearIndex];
}

template<typename T>
bool CircularQueue<T>::isEmpty() const {
    return currentSize == 0;
}

template<typename T>
bool CircularQueue<T>::isFull() const {
    return currentSize == capacity;
}

template<typename T>
T &CircularQueue<T>::operator[](int index) {
    if (index < 0 || index >= capacity) {
        throw std::runtime_error("Index out of range");
    }
    return arr[index];
}

template<typename T>
CircularQueue<T> &CircularQueue<T>::operator=(const CircularQueue<T> &rhs) {
    frontIndex = rhs.frontIndex;
    rearIndex = rhs.rearIndex;
    currentSize = rhs.currentSize;
    for (int i = 0; i < capacity; i++) {
        arr[i] = rhs.arr[i];
    }
    return *this;
}

template<typename T>
void CircularQueue<T>::cut(int index) {
    rearIndex = index;
}

// Explicit template instantiation
template
class CircularQueue<Lsb::unit>;

template
class CircularQueue<Rob::Unit>;

