//
// Created by qiuyuhang on 24-7-28.
//

#ifndef CODE_SIMULATOR_HPP
#define CODE_SIMULATOR_HPP

#include "utility.hpp"
#include "decode.hpp"

#include "register.hpp"
#include "load_store_buffer.hpp"
#include "memory.hpp"
#include "predictor.hpp"
#include "reorder_buffer.hpp"
#include "reserve_station.hpp"
#include "common_data_bus.hpp"
#include "algorithm_logic_unit.hpp"
#include "interpreter.hpp"

//extern int clo;
class Simulator {
public:
    int pc = 0;
    int pcNext = 0;
    int iR = 0;

    void addPC();

    void addPC(int imm);

    void addPC(int reg_rs1, int imm);


    Reg reg;
    Rob rob;
    Lsb lsb;
    Memory memory;
    Predictor predictor;
    Rs rs;
    Cdb cdb;
    Alu alu;

    void flush();

public:
    Simulator();

    void work();
};

#endif //CODE_SIMULATOR_HPP
