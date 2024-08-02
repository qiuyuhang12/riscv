//
// Created by qiuyuhang on 24-7-30.
//
#include "predictor.hpp"

int hash(int _pc) {
    return _pc & 0x1f;
}

predict Predictor::getPredict(int _pc) {
//    return jump;
//    int tmp = hash(_pc);
    switch (result[hash(_pc)]) {
        case 0b00:
        case 0b01:
            return jump;
        case 0b10:
        case 0b11:
            return noJump;
        default:
            assert(0);
    }
}

void Predictor::update(int _pc, bool correctness) {
    shoot++;
    if (correctness) {
        got++;
    }
//    return;
    switch (result[hash(_pc)]) {
        case 0b00:
            if (correctness) {
                result[hash(_pc)] = 0b00;
            } else {
                result[hash(_pc)] = 0b01;
            }
            break;
        case 0b01:
            if (correctness) {
                result[hash(_pc)] = 0b00;
            } else {
                result[hash(_pc)] = 0b10;
            }
            break;
        case 0b10:
            if (correctness) {
                result[hash(_pc)] = 0b11;
            } else {
                result[hash(_pc)] = 0b01;
            }
            break;
        case 0b11:
            if (correctness) {
                result[hash(_pc)] = 0b11;
            } else {
                result[hash(_pc)] = 0b10;
            }
            break;
    }
}