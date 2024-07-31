//
// Created by qiuyuhang on 24-7-29.
//

#ifndef CODE_ALGORITHM_LOGIC_UNIT_HPP
#define CODE_ALGORITHM_LOGIC_UNIT_HPP

#include "utility.hpp"
#include "decode.hpp"
//#include "common_data_bus.hpp"
class Cdb;
class Alu {
public:
    Cdb *cdb;

    struct bBuffer {
        bool busy = false;
        bool done = false;
        opcode op = opcode::undefined;
        int rs1 = 0;
        int rs2 = 0;
        int imm = 0;
        bool rsl = false;
        int pc = 0;
        int entry = 0;
        bBuffer& operator=(const bBuffer& other) {
            busy = other.busy;
            done = other.done;
            op = other.op;
            rs1 = other.rs1;
            rs2 = other.rs2;
            imm = other.imm;
            rsl = other.rsl;
            pc = other.pc;
            entry = other.entry;
            return *this;
        }
        void init() {
            busy = false;
            done = false;
            op = opcode::undefined;
            rs1 = 0;
            rs2 = 0;
            imm = 0;
            rsl = false;
            pc = 0;
            entry = 0;
        }
    } buffer,outBuffer;

    void bExe() ;

    void bWrite();

public:
    Alu();
    Alu(Cdb *cdb);
    void init(Cdb *cdb);
    void step() ;

    void inBuffer(int rs1, int rs2, int imm, opcode op, int entry, int pc);

    void alu(int vi, int vj, int imm, opcode op, int entry);
};

#endif //CODE_ALGORITHM_LOGIC_UNIT_HPP
