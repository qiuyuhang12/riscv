//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_DECODE_HPP
#define MYPROJECT_DECODE_HPP

#include <iostream>
#include "register.hpp"
#include "memory.hpp"

enum type {
    Type,
    U_TYPE,//
    J_TYPE,//
    I_TYPE,
    B_TYPE,
    S_TYPE,
    R_TYPE
};
enum opcode {
    undefined,
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
    int rd = 0;
    int rs1 = 0;
    int rs2 = 0;
    int imm = 0;
};

instruction decode(int iR) {
    if (iR == 0x0ff00513) {
        cout << (reg[10] & 0xff);
        exit(0);
        exit(reg[10]);
    }
    instruction ins;
    ins.rd = (iR >> 7) & 31;
    ins.rs1 = (iR >> 15) & 31;
    ins.rs2 = (iR >> 20) & 31;
    int opcode = iR & 127;
    switch (opcode) {
        case 55:
            ins.tp = U_TYPE;
            ins.op = lui;
            ins.imm = (iR >> 12)<<12;
            ins.rs1=-1;
            ins.rs2=-1;
            break;
        case 23:
            ins.tp = U_TYPE;
            ins.op = auipc;
            ins.imm = (iR >> 12)<<12;
            ins.rs1=-1;
            ins.rs2=-1;
            break;
        case 111:
            ins.tp = J_TYPE;
            ins.op = jal;
            ins.imm =
                    (((iR >> 12) & (0b11111111)) << 12) |
                    (((iR >> 20) & 1) << 11) |
                    (((iR >> 21) & 1023) << 1) |
                    (((iR >> 31) & 1) << 20);
            ins.rs1=-1;
            ins.rs2=-1;
            break;
        case 103:
        case 3:
        case 19:
            ins.tp = I_TYPE;
            ins.imm = (iR >> 20) & 4095;
            ins.rs2=-1;
            break;
        case 99:
            ins.tp = B_TYPE;
            ins.imm =
                    (((iR >> 8) & 15) << 1) |
                    (((iR >> 25) & 63) << 5) |
                    (((iR >> 7) & 1) << 11) |
                    (((iR >> 31) & 1) << 12);
            break;
        case 35:
            ins.tp = S_TYPE;
            ins.imm = ((iR >> 7) & 31) |
                      (((iR >> 25) & 127) << 5);
            break;
        case 51:
            ins.tp = R_TYPE;
            break;
        default:
            ins.tp = Type;
            ins.op = undefined;
            break;
    }
    int funct3 = (iR >> 12) & 7;
    if (ins.tp == B_TYPE) {
        switch (funct3) {
            case 0:
                ins.op = beq;
                break;
            case 1:
                ins.op = bne;
                break;
            case 4:
                ins.op = blt;
                break;
            case 5:
                ins.op = bge;
                break;
            case 6:
                ins.op = bltu;
                break;
            case 7:
                ins.op = bgeu;
                break;
            default:
                ins.op = undefined;
                break;
        }
    } else if (ins.tp == S_TYPE) {
        switch (funct3) {
            case 0:
                ins.op = sb;
                break;
            case 1:
                ins.op = sh;
                break;
            case 2:
                ins.op = sw;
                break;
            default:
                ins.op = undefined;
                break;
        }
    } else if (ins.tp == R_TYPE) {
        int funct7 = iR >> 25;
        switch (funct3) {
            case 0:
                if (funct7 == 0) {
                    ins.op = add;
                } else {
                    ins.op = sub;
                }
                break;
            case 1:
                ins.op = sll;
                break;
            case 2:
                ins.op = slt;
                break;
            case 3:
                ins.op = sltu;
                break;
            case 4:
                ins.op = xor_;
                break;
            case 5:
                if (funct7 == 0) {
                    ins.op = srl;
                } else {
                    ins.op = sra;
                }
                break;
            case 6:
                ins.op = or_;
                break;
            case 7:
                ins.op = and_;
                break;
            default:
                ins.op = undefined;
                break;
        }
    } else if (ins.tp == I_TYPE) {
        switch (opcode) {
            case 103:
                ins.op = jalr;
                break;
            case 3:
                switch (funct3) {
                    case 0:
                        ins.op = lb;
                        break;
                    case 1:
                        ins.op = lh;
                        break;
                    case 2:
                        ins.op = lw;
                        break;
                    case 4:
                        ins.op = lbu;
                        break;
                    case 5:
                        ins.op = lhu;
                        break;
                    default:
                        ins.op = undefined;
                        break;
                }
                break;
            case 19:
                switch (funct3) {
                    case 0:
                        ins.op = addi;
                        break;
                    case 2:
                        ins.op = slti;
                        break;
                    case 3:
                        ins.op = sltiu;
                        break;
                    case 4:
                        ins.op = xori;
                        break;
                    case 6:
                        ins.op = ori;
                        break;
                    case 7:
                        ins.op = andi;
                        break;
                    case 1:
                        ins.op = slli;
                        break;
                    case 5:
                        if ((iR >> 30) == 0) {
                            ins.op = srli;
                        } else {
                            ins.op = srai;
                        }
                        break;
                    default:
                        ins.op = undefined;
                        break;
                }
                break;
            default:
                ins.op = undefined;
                break;
        }
    }
    return ins;
}

#endif //MYPROJECT_DECODE_HPP
