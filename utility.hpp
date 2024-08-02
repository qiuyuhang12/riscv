//
// Created by qiuyuhang on 24-7-27.
//

#ifndef CODE_UTILITY_HPP
#define CODE_UTILITY_HPP
//#define debug
//#define detail
//#define regshow
#ifndef debug
//#define fileread
#endif
#define single
//#define showShoot
#include <cassert>
#include <iostream>
#include <string>

//#include "interpreter.hpp"
//using namespace std;
extern int clo;

//extern Interpreter interpreter;
int sext(int imm, int len);

#ifndef single
const int CdbCapacity = 1 << 5;
const int MemoryCapacity = 1 << 18;
const int PredictorCapacity = 1 << 5;
const int RsCapacity = 1 << 4;
const int capacity = 1 << 4;//queue
#endif
#ifdef single
const int CdbCapacity = 1 << 2;
const int MemoryCapacity = 1 << 19;
const int PredictorCapacity = 1 << 5;
const int RsCapacity = 1;
const int capacity = 1;//queue
#endif
const std::string filePath = "/run/media/qiuyuhang/data/ppca/riscv/testcases/bulgarian.data";

template<typename T>
class CircularQueue {
private:
    T arr[capacity];

    int currentSize = 0;
public:
    void clear() {
        for (int i = 0; i < capacity; ++i) {
            arr[i] = T();
        }
        frontIndex = 0;
        rearIndex = -1;
        currentSize = 0;
    }

    int frontIndex = 0;
    int rearIndex = -1;

    CircularQueue();

    bool enqueue(const T &value);

    int getNextRearIndex();

    int size() const;

    bool dequeue();

    T &front();

    T &rear();

    bool isEmpty() const;

    bool isFull() const;

    //随机访问,我希望[frontIndex]是front
    T &operator[](int index);

    CircularQueue &operator=(const CircularQueue &rhs);

    void cut(int index);

    bool isInQueue(int index) {
        //注意front>rear的循环情况
        if (frontIndex > rearIndex) {
            return index >= frontIndex || index <= rearIndex;
        } else {
            return index >= frontIndex && index <= rearIndex;
        }
    }
};

#endif //CODE_UTILITY_HPP
