//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_DECODE_HPP
#define MYPROJECT_DECODE_HPP

#include <iostream>
#include "utility.hpp"

enum type {
    Type,
    END,
    U_TYPE,//
    J_TYPE,//
    I_TYPE,
    B_TYPE,
    S_TYPE,
    R_TYPE
};
enum opcode {
    undefined,
    end,
    lui,
    auipc,
    jal,
    jalr,
    beq,
    bne,
    blt,
    bge,
    bltu,
    bgeu,
    lb,
    lh,
    lw,
    lbu,
    lhu,
    sb,
    sh,
    sw,
    addi,
    slti,
    sltiu,
    xori,
    ori,
    andi,
    slli,
    srli,
    srai,
    add,
    sub,
    sll,
    slt,
    sltu,
    xor_,
    srl,
    sra,
    or_,
    and_
};

struct instruction {
    type tp = Type;
    opcode op = undefined;
    int originalOp;
    int rd = -1;
    int rs1 = 0;
    int rs2 = 0;
    int imm = 0;
    int pc = 0;
    int iR = 0;

    void print();
    bool operator==(const instruction &rhs) const{
        return tp == rhs.tp &&
               op == rhs.op &&
               originalOp == rhs.originalOp &&
               rd == rhs.rd &&
               rs1 == rhs.rs1 &&
               rs2 == rhs.rs2 &&
               imm == rhs.imm &&
               pc == rhs.pc &&
               iR == rhs.iR;
    }
};

instruction decode(int iR);

#endif //MYPROJECT_DECODE_HPP
