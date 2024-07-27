//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_MEMORY_HPP
#define MYPROJECT_MEMORY_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
//int memory[1 << 13] = {0};
int memory[1 << 20] = {0};
int pc = 0;
int iR;
const string filePath = "/run/media/qiuyuhang/data/ppca/riscv/testcases/tak.data";

int get8() {
    int tmp = 0;
    for (int i = 0; i < 4; i++) {
        tmp += memory[8 + i] << (i * 8);
    }
    return tmp;
}

void step() {
    iR = 0;
    for (int i = 0; i < 4; i++) {
        iR += memory[pc + i] << (i * 8);
    }
}

void addPC() {
    pc += 4;
    if (pc >= 1 << 20) {
        exit(0);
    }
};

void loadCode() {
    ifstream file(filePath, ios::in);
    if (!file) {
        cerr << "Unable to open file: " << filePath << endl;
        return;
    }
    // 读取文件内容到字符串
    stringstream buffer;
    buffer << file.rdbuf();
    string line;
    int pointer = 0;
    while (getline(buffer, line)) {
        if (line[0] == '@') {
            pointer = stoi(line.substr(1), nullptr, 16); // Assuming the address is in hex
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

#endif //MYPROJECT_MEMORY_HPP
