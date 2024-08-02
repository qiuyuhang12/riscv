//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_RESERVE_STATION_HPP
#define MYPROJECT_RESERVE_STATION_HPP

#include "utility.hpp"
#include "decode.hpp"

//#include "register.hpp"
//#include "load_store_buffer.hpp"
//#include "common_data_bus.hpp"
#include "algorithm_logic_unit.hpp"

class Rob;

class Reg;

class Lsb;

class Cdb;

class Rs {
public:
    static const int capacity = RsCapacity;
    Reg *reg;
    Lsb *lsb;
    Rob *rob;
    Cdb *cdb;
    Alu *alu;

    struct unit {
        bool busy = false;
        opcode op;
        int rs1 = -1;
        int rs2 = -1;
        int imm = 0;
        int entry1 = -1;
        int entry2 = -1;
        int dest = -1;
        int a = 0;
        int pc = 0;
        bool isB = false;
        instruction inst;

        void init() {
            busy = false;
            op = opcode::undefined;
            rs1 = -1;
            rs2 = -1;
            imm = 0;
            entry1 = -1;
            entry2 = -1;
            dest = -1;
            a = 0;
            pc = 0;
            isB = false;
            inst = instruction();
        }

        void print() {
            std::cout << "busy:" << busy << " op:" << op << " rs1:" << rs1 << " rs2:" << rs2 << " imm:" << imm
                      << " entry1:"
                      << entry1 << " entry2:" << entry2 << " dest:" << dest << " a:" << a << " pc:" << pc << " isB:"
                      << isB
                      << std::endl;
            inst.print();
        }

        bool operator==(const unit &rhs) const {
            return busy == rhs.busy && op == rhs.op && rs1 == rhs.rs1 && rs2 == rhs.rs2 && imm == rhs.imm &&
                   entry1 == rhs.entry1 &&
                   entry2 == rhs.entry2 && dest == rhs.dest && a == rhs.a && pc == rhs.pc && isB == rhs.isB &&
                   inst == rhs.inst;
        }
    };

    unit rs[capacity];
    unit rsNext[capacity];


    void query(int index);

    void execute(int index);

public:
    Rs();

    Rs(Reg *_reg, Lsb *_lsb, Rob *_rob, Cdb *_cdb, Alu *_alu);

    void init(Reg *_reg, Lsb *_lsb, Rob *_rob, Cdb *_cdb, Alu *_alu);

    void flush();

    bool isFull();

    void issue(int entry, instruction inst, int pc);

    void step();

    void clear() {
        for (int i = 0; i < capacity; ++i) {
            rsNext[i].init();
            rs[i].init();
        }
        alu->clear();
    }
};

#endif //MYPROJECT_RESERVE_STATION_HPP
