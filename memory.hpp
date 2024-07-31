//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_MEMORY_HPP
#define MYPROJECT_MEMORY_HPP

#include "utility.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class Memory {
//int memory[1 << 13] = {0};
public:
    int memory[MemoryCapacity] = {0};
//    int iR = -1;
    const string filePath = "/run/media/qiuyuhang/data/ppca/riscv/testcases/tak.data";

    void loadCode();

    void cinCode();

    int workCount = 0;
    bool working = false;

    int workCountNext = 0;
    bool workingNext = false;

    void startWork();

public:
    Memory();

    void flush();

    void step();

    explicit Memory(const string &_file);

    int get8();

    int getiR(int pc);

    void store(int addr, int value, int length);

    int loadU(int addr, int length);

    int load(int addr, int length);
};


#endif //MYPROJECT_MEMORY_HPP
