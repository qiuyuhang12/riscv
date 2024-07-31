//
// Created by qiuyuhang on 24-7-30.
//
#include "predictor.hpp"

predict Predictor::getPredict(int _pc) {
    return jump;
    for (int i = 0; i < size; i++) {
        if (pc[i] == _pc) {
            switch (result[i]) {
                case 0b00:
                case 0b01:
                    return jump;
                case 0b10:
                case 0b11:
                    return noJump;
            }
        }
    }
    return noJump;
}

void Predictor::update(int _pc, bool correctness) {
    return;
    for (int i = 0; i < size; i++) {
        if (pc[i] == _pc) {
            switch (result[i]) {
                case 0b00:
                    if (correctness) {
                        result[i] = 0b00;
                    } else {
                        result[i] = 0b01;
                    }
                    break;
                case 0b01:
                    if (correctness) {
                        result[i] = 0b00;
                    } else {
                        result[i] = 0b10;
                    }
                    break;
                case 0b10:
                    if (correctness) {
                        result[i] = 0b11;
                    } else {
                        result[i] = 0b01;
                    }
                    break;
                case 0b11:
                    if (correctness) {
                        result[i] = 0b11;
                    } else {
                        result[i] = 0b10;
                    }
                    break;
            }
        }
    }
}