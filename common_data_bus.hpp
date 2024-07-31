//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_COMMON_DATA_BUS_HPP
#define MYPROJECT_COMMON_DATA_BUS_HPP

#include "utility.hpp"
#include "decode.hpp"

class Cdb {
public:
    static const int capacity = CdbCapacity;
    enum State {
        LEISURE,
        ACTIVE,
        USED
    };
    struct unit {
        State state = LEISURE;
        int value = -1;
        int entry = -1;
        bool memAddr= false;
//        unit& operator=(const unit& other) {
//            state = other.state;
//            value = other.value;
//            entry = other.entry;
//            memAddr = other.memAddr;
//            return *this;
//        }
        void init() {
            state = LEISURE;
            value = -1;
            entry = -1;
            memAddr = false;
        }
        void print() {
            std::cout<<"state: "<<state<<" value: "<<value<<" entry: "<<entry<<" memAddr: "<<memAddr<<std::endl;
        }

    };
    struct jaUnit {
        State state = LEISURE;
        int valuePC = 0;
        int value = 0;
        int entry = 0;
        jaUnit& operator=(const jaUnit& other) {
            state = other.state;
            value = other.value;
            valuePC = other.valuePC;
            entry = other.entry;
            return *this;
        }
    }ju, juNext;
    struct brUnit {
        State state = LEISURE;
        int pc = -1;
        bool rsl = false;
        int entry = -1;
        void init() {
            state = LEISURE;
            pc = -1;
            rsl = false;
            entry = -1;
        }
        void print() {
            std::cout<<"state: "<<state<<" pc: "<<pc<<" rsl: "<<rsl<<" entry: "<<entry<<std::endl;
        }
//        brUnit &operator=(const brUnit &other) {
//            state = other.state;
//            pc = other.pc;
//            rsl = other.rsl;
//            entry = other.entry;
//            return *this;
//        }

    }br[capacity], brNext[capacity];
    unit cdb[capacity];
    unit cdbNext[capacity];

    //erase executed entry
    void erase(int entry, bool memAddr = false) ;
    void eraseBr(int entry) ;
public:
    void flush() ;
//    bool storePanic = false;
    bool jalrPanic = false;
//    bool stop = false;
    bool pcFrozen = false;
    void writeBr(int pc, bool rsl, int entry);
    void write(int value, int entry, bool memAddr = false) ;
    std::pair<int, int> get(int entry, bool memAddr = false) ;
    std::pair<int, std::pair<int,int > > getBr(int entry) ;
    void writeJal(int value, int entry, int valuePC) ;
    std::pair<int, int> getJal(int entry) ;
};

#endif //MYPROJECT_COMMON_DATA_BUS_HPP
