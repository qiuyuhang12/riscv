//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_REORDER_BUFFER_HPP
#define MYPROJECT_REORDER_BUFFER_HPP

#include "utility.hpp"
#include "decode.hpp"

#include "register.hpp"
#include "memory.hpp"
#include "predictor.hpp"
#include "reserve_station.hpp"
#include "load_store_buffer.hpp"
#include "common_data_bus.hpp"


class Rob {
public:
    Reg *reg;
    Lsb *lsb;
    Cdb *cdb;
    Memory *memory;
    Rs *rs;
    Predictor *predictor;

    enum State {
        UNKNOWN,
        ISSUE,
        WRITE,
        COMMIT,
        TODELETECDB,
    };
    struct Unit {
        int entry;
        bool busy = false;
        instruction inst;
        State state = UNKNOWN;
        int dest = -1;
        int value = 0;
        int pc = 0;//debug
        void print();
        bool operator==(const Unit &rhs) const{
            return entry == rhs.entry&&busy == rhs.busy&&inst == rhs.inst&&state == rhs.state&&dest == rhs.dest&&value == rhs.value&&pc == rhs.pc;
        }
    };
    CircularQueue<Unit> queue;
    CircularQueue<Unit> queueNext;

    void commit();

    void wrongPredicted(int entry, int pc);

    void receiveBroadcast();

    void check(int pc);

public:
    Rob();

    Rob(Reg *_reg, Lsb *_lsb, Cdb *_cdb, Memory *_memory, Rs *_rs, Predictor *_predictor) ;

    void init(Reg *_reg, Lsb *_lsb, Cdb *_cdb, Memory *_memory, Rs *_rs, Predictor *_predictor);

    int getNextRearIndex();

    void flush();

    void step();

    void issue(int iR);

    bool isFull();

};

#endif //MYPROJECT_REORDER_BUFFER_HPP