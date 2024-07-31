//
// Created by qiuyuhang on 24-7-27.
//

#ifndef CODE_UTILITY_HPP
#define CODE_UTILITY_HPP
#define debug
//#define detail
#include <cassert>
#include <iostream>
#include <string>
//#include "interpreter.hpp"
//using namespace std;
extern int clo;
//extern Interpreter interpreter;
int sext(int imm, int len) ;

//const int CdbCapacity = 1 << 5;
//const int MemoryCapacity = 1 << 18;
//const int PredictorCapacity = 1 << 5;
//const int RsCapacity = 1 << 4;
const int CdbCapacity = 1 << 2;
const int MemoryCapacity = 1 << 19;
const int PredictorCapacity = 1;
const int RsCapacity = 1;
const int capacity = 1;//queue
const std::string filePath = "/run/media/qiuyuhang/data/ppca/riscv/sample/sample.data";

template<typename T>
class CircularQueue {
private:
    T arr[capacity];

    int currentSize=0;
public:
    int frontIndex=0;
    int rearIndex=-1;

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
};

#endif //CODE_UTILITY_HPP
