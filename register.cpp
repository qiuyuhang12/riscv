//
// Created by qiuyuhang on 24-7-30.
//
#include "register.hpp"

void Reg::unit::init() {
    value = 0;
    busy = false;
    entry = -1;
}

void Reg::flush() {
    reg[0].init();
    regNext[0].init();
    for (int i = 1; i < 32; i++) {
        reg[i] = regNext[i];
    }
}

Reg::Reg(int &_pcReg, int &_nextPCReg) : pcReg(_pcReg), nextPCReg(_nextPCReg) {
    for (int i = 0; i < 32; i++) {
        reg[i].init();
        regNext[i].init();
    }
}