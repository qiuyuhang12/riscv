//
// Created by qiuyuhang on 24-7-30.
//
#include "memory.hpp"
#include <cassert>
#include <ctime>

void Memory::loadCode() {
    ifstream file(filePath, ios::in);
    if (!file) {
        cerr << "Unable to open file: " << filePath << endl;
        return;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string line;
    int pointer = 0;
    while (getline(buffer, line)) {
        if (line[0] == '@') {
            pointer = stoi(line.substr(1), nullptr, 16);
            continue;
        }
        istringstream iss(line);
        unsigned int byteValue;
        while (iss >> hex >> byteValue) {
            memory[pointer] = byteValue;
            pointer++;
        }
    }
}

void Memory::cinCode() {
    string line;
    int pointer = 0;
    int tm = time(0);
    while (getline(cin, line)) {
        if (tm < time(0) - 3) {
            break;
        }
        if (line[0] == '@') {
            pointer = stoi(line.substr(1), nullptr, 16);
            continue;
        }
        istringstream iss(line);
        unsigned int byteValue;
        while (iss >> hex >> byteValue) {
            memory[pointer] = byteValue;
            pointer++;
        }
    }
}

void Memory::startWork() {
    workingNext = true;
    workCountNext = 3;
}

Memory::Memory() {
    cinCode();
}

void Memory::flush() {
    working = workingNext;
    workCount = workCountNext;
}

void Memory::step() {
    if (working) {
#ifdef debug
        cout << "memory working , count "<<workCount<<"->"<<workCount-1 << endl;
#endif
        if (workCount != 0)workCountNext--;
        if (workCount == 0) {
            workingNext = false;
        }
    }
}

Memory::Memory(const string &_file) : filePath(_file) {
    loadCode();
}

int Memory::get8() {
    int tmp = 0;
    for (int i = 0; i < 4; i++) {
        tmp += memory[8 + i] << (i * 8);
    }
    return tmp;
}

int Memory::getiR(int pc) {
    int iR = 0;
    for (int i = 0; i < 4; i++) {
        iR += memory[pc + i] << (i * 8);
    }
    return iR;
}

void Memory::store(int addr, int value, int length) {
    assert(length == 1 || length == 2 || length == 4);
    for (int i = 0; i < length; i++) {
        memory[addr + i] = (value >> (i * 8)) & 0xFF;
    }
}

int Memory::loadU(int addr, int length) {
    assert(length == 1 || length == 2 || length == 4);
    int tmp = 0;
    for (int i = 0; i < length; i++) {
        tmp += memory[addr + i] << (i * 8);
    }
    return tmp;
}

int Memory::load(int addr, int length) {
    assert(length == 1 || length == 2 || length == 4);
    return sext(loadU(addr, length), length * 8);
}