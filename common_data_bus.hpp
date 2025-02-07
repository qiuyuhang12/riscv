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
//        USED
    };

    struct unit {
        bool take = false;//立即修改
        State state = LEISURE;
        int value = -1;
        int entry = -1;
        bool memAddr = false;
        void init() {
            take = false;
            state = LEISURE;
            value = -1;
            entry = -1;
            memAddr = false;
        }

        void print() {
            std::cout << "state: " << state << " value: " << value << " entry: " << entry << " memAddr: " << memAddr
                      << std::endl;
        }

    };

    struct jaUnit {
        bool take = false;//立即修改
        State state = LEISURE;
        int valuePC = 0;
        int value = 0;
        int entry = 0;

        jaUnit &operator=(const jaUnit &other) {
            state = other.state;
            value = other.value;
            valuePC = other.valuePC;
            entry = other.entry;
            return *this;
        }

        void init() {
            take = false;
            state = LEISURE;
            value = 0;
            valuePC = 0;
            entry = 0;
        }
    } ju, juNext;

    struct brUnit {
        bool take = false;//立即修改
        State state = LEISURE;
        int pc = -1;
        bool rsl = false;
        int entry = -1;

        void init() {
            take = false;
            state = LEISURE;
            pc = -1;
            rsl = false;
            entry = -1;
        }

        void print() {
            std::cout << "state: " << state << " pc: " << pc << " rsl: " << rsl << " entry: " << entry << std::endl;
        }
    } br[capacity], brNext[capacity];

    unit cdb[capacity];
    unit cdbNext[capacity];

    //erase executed entry
    void erase(int entry, bool memAddr = false);

    void eraseBr(int entry);

public:
    void flush();

//    bool storePanic = false;
    bool jalrPanic = false;
//    bool stop = false;
    bool pcFrozen = false;
    bool clearUp = false;

    void writeBr(int pc, bool rsl, int entry);

    void write(int value, int entry, bool memAddr = false);

    std::pair<int, int> get(int entry, bool memAddr = false, bool isJ = false);

    std::pair<int, std::pair<int, int> > getBr(int entry);

    void writeJal(int value, int entry, int valuePC);

    std::pair<int, int> getJal(int entry);

    void clear() {
        for (int i = 0; i < capacity; i++) {
            cdbNext[i].init();
            brNext[i].init();
            cdb[i].init();
            br[i].init();
        }
        juNext.init();
        ju.init();
        jalrPanic = false;
        clearUp = false;
        pcFrozen = false;
    }
};

#endif //MYPROJECT_COMMON_DATA_BUS_HPP
