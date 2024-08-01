//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_REGISTER_HPP
#define MYPROJECT_REGISTER_HPP

#include "utility.hpp"

class Reg {
public:
public:
    struct unit {
        int value = 0;
        bool busy = false;
        int entry = -1;

        void init() ;
    };

    unit reg[32];
    unit regNext[32];
    int &pcReg;
    int &nextPCReg;

    void flush() ;

//    int reg[32] = {0};
//    bool isBusy[32] = {false};
//    int entry[32] = {0};
    Reg(int &_pcReg, int &_nextPCReg) ;
    void print(int m=0,int n=32) {
        for (int i = m; i < n; i++) {
            std::cout << "reg[" << i << "]:     \t  " << reg[i].value << "\t  busy: " << reg[i].busy << "\t  entry: " << reg[i].entry << std::endl;
            std::cout << "regNext[" << i << "]: \t  " << regNext[i].value << "\t  busy: " << regNext[i].busy << "\t  entry: " << regNext[i].entry << std::endl;
        }
    }
    void clear() {
        for (int i = 0; i < 32; i++) {
            regNext[i].busy = false;
            regNext[i].entry = -1;
            reg[i].busy = false;
            reg[i].entry = -1;
        }
    }
};

#endif //MYPROJECT_REGISTER_HPP
