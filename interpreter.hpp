//
// Created by qiuyuhang on 24-7-25.
//
//#include "utility.hpp"
#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
//#define debug
extern const std::string filePath;
extern int sext(int imm, int len);
//const std::string filePath = "/run/media/qiuyuhang/data/ppca/riscv/sample/sample.data";
//int sext(int imm, int len) {
//    if (len == 32) {
//        return imm;
//    }
//    if ((imm >> (len - 1)) & 1) {
//        long long tmp = (long long) imm | (long long) (0xFFFFFFFF << len);
//        return tmp;
//    }
//    return imm;
//}
using namespace std;

//int memory[1 << 13] = {0};
class Interpreter {
public:
    Interpreter() {
        pre();
    }
    const std::string filePath = "/run/media/qiuyuhang/data/ppca/riscv/testcases/pi.data";

    int reg[32] = {0};
    int memory[1 << 20] = {0};
    int pc = 0;
    int iR;

    int get8() {
        int tmp = 0;
        for (int i = 0; i < 4; i++) {
            tmp += memory[8 + i] << (i * 8);
        }
        return tmp;
    }

    void step() {
        iR = 0;
        for (int i = 0; i < 4; i++) {
            iR += memory[pc + i] << (i * 8);
        }
    }

    void addPC() {
        pc += 4;
        if (pc >= 1 << 20) {
            exit(0);
        }
    };

    void loadCode() {
        ifstream file(filePath, ios::in);
        if (!file) {
            cerr << "Unable to open file: " << filePath << endl;
            return;
        }
        // 读取文件内容到字符串
        stringstream buffer;
        buffer << file.rdbuf();
        string line;
        int pointer = 0;
        while (getline(buffer, line)) {
            if (line[0] == '@') {
                pointer = stoi(line.substr(1), nullptr, 16); // Assuming the address is in hex
                continue;
            }
            istringstream iss(line);
            unsigned int byteValue;
            while (iss >> hex >> byteValue) {
                memory[pointer] = byteValue;
                pointer++;
            }
        }
    }

    void cinCode() {
        return;
        string line;
        int pointer = 0;
        int tm = time(0);
        while (getline(cin, line)) {
//        if (tm< time(0)-3){
//            break;
//        }
            if (line[0] == '@') {
//            cout<<0<<endl;
                pointer = stoi(line.substr(1), nullptr, 16); // Assuming the address is in hex
                continue;
            }
            istringstream iss(line);
            unsigned int byteValue;
            while (iss >> hex >> byteValue) {
                memory[pointer] = byteValue;
                pointer++;
            }
        }
    }

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
        int rd = -1;
        int rs1 = 0;
        int rs2 = 0;
        int imm = 0;
    };

    instruction decode() {
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
                ins.imm = (iR >> 12) << 12;
                ins.rs1 = -1;
                ins.rs2 = -1;
                break;
            case 23:
                ins.tp = U_TYPE;
                ins.op = auipc;
                ins.imm = (iR >> 12) << 12;
                ins.rs1 = -1;
                ins.rs2 = -1;
                break;
            case 111:
                ins.tp = J_TYPE;
                ins.op = jal;
                ins.imm =
                        (((iR >> 12) & (0b11111111)) << 12) |
                        (((iR >> 20) & 1) << 11) |
                        (((iR >> 21) & 1023) << 1) |
                        (((iR >> 31) & 1) << 20);
                ins.rs1 = -1;
                ins.rs2 = -1;
                break;
            case 103:
            case 3:
            case 19:
                ins.tp = I_TYPE;
                ins.imm = (iR >> 20) & 4095;
                ins.rs2 = -1;
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

    void interpreter(instruction ins) {
        int temp = 0;
        switch (ins.tp) {
            case U_TYPE:
                switch (ins.op) {
                    case lui:
                        reg[ins.rd] = sext(ins.imm, 32);
                        break;
                    case auipc:
                        reg[ins.rd] = (sext(ins.imm, 32)) + pc;
                        break;
                }
                break;
            case J_TYPE:
                switch (ins.op) {
                    case jal:
                        reg[ins.rd] = pc + 4;
                        pc += sext(ins.imm, 21);
                        break;
                }
                break;
            case I_TYPE:
                switch (ins.op) {
                    case jalr:
                        temp = pc + 4;
                        pc = (reg[ins.rs1] + sext(ins.imm, 12)) & ~1;
                        reg[ins.rd] = temp;
                        break;
                    case lb:
                        reg[ins.rd] = sext(memory[reg[ins.rs1] + sext(ins.imm, 12)], 8);
                        break;
                    case lh:
                        reg[ins.rd] = sext(memory[reg[ins.rs1] + sext(ins.imm, 12)] +
                                           (memory[reg[ins.rs1] + sext(ins.imm, 12) + 1] << 8), 16);
                        break;
                    case lw:
//                        cout<<hex<<"clo: "<<clo<<"    "<<hex<<reg[ins.rs1] + sext(ins.imm, 12)<<endl;
                        reg[ins.rd] = memory[reg[ins.rs1] + sext(ins.imm, 12)] +
                                      (memory[reg[ins.rs1] + sext(ins.imm, 12) + 1] << 8) +
                                      (memory[reg[ins.rs1] + sext(ins.imm, 12) + 2] << 16) +
                                      (memory[reg[ins.rs1] + sext(ins.imm, 12) + 3] << 24);
                        break;
                    case lbu:
                        reg[ins.rd] = memory[reg[ins.rs1] + sext(ins.imm, 12)];
                        break;
                    case lhu:
                        reg[ins.rd] = memory[reg[ins.rs1] + sext(ins.imm, 12)] +
                                      (memory[reg[ins.rs1] + sext(ins.imm, 12) + 1] << 8);
                        break;
                    case addi:
                        reg[ins.rd] = reg[ins.rs1] + sext(ins.imm, 12);
                        break;
                    case slti:
                        reg[ins.rd] = reg[ins.rs1] < sext(ins.imm, 12);
                        break;
                    case sltiu:
                        reg[ins.rd] = reg[ins.rs1] < static_cast<unsigned int>(sext(ins.imm, 12));
                        break;
                    case xori:
                        reg[ins.rd] = reg[ins.rs1] ^ sext(ins.imm, 12);
                        break;
                    case ori:
                        reg[ins.rd] = reg[ins.rs1] | sext(ins.imm, 12);
                        break;
                    case andi:
                        reg[ins.rd] = reg[ins.rs1] & sext(ins.imm, 12);
                        break;
                    case slli:
                        assert((ins.imm & (1 << 5)) == 0);
                        reg[ins.rd] = reg[ins.rs1] << (ins.imm & 0x1F);
                        break;
                    case srli:
                        assert((ins.imm & (1 << 5)) == 0);
                        reg[ins.rd] = static_cast<unsigned int>(reg[ins.rs1]) >> (ins.imm & 0x1F);
                        break;
                    case srai:
                        assert((ins.imm & (1 << 5)) == 0);
                        reg[ins.rd] = reg[ins.rs1] >> (ins.imm & 0x1F);
                        break;
                }
                break;
            case S_TYPE:
//            cout << "dest: " << reg[ins.rs1] + sext(ins.imm, 12) << "   pc:" << reg[ins.rs2] << endl;
                switch (ins.op) {
                    case sb:
                        memory[reg[ins.rs1] + sext(ins.imm, 12)] = reg[ins.rs2] & 0xFF;
                        break;
                    case sh:
                        memory[reg[ins.rs1] + sext(ins.imm, 12)] = reg[ins.rs2] & 0xFF;
                        memory[reg[ins.rs1] + sext(ins.imm, 12) + 1] = (reg[ins.rs2] >> 8) & 0xFF;
                        break;
                    case sw:
                        memory[reg[ins.rs1] + sext(ins.imm, 12)] = reg[ins.rs2] & 0xFF;
                        memory[reg[ins.rs1] + sext(ins.imm, 12) + 1] = (reg[ins.rs2] >> 8) & 0xFF;
                        memory[reg[ins.rs1] + sext(ins.imm, 12) + 2] = (reg[ins.rs2] >> 16) & 0xFF;
                        memory[reg[ins.rs1] + sext(ins.imm, 12) + 3] = (reg[ins.rs2] >> 24) & 0xFF;
                        break;
                }
                break;
            case R_TYPE:
                switch (ins.op) {
                    case add:
                        reg[ins.rd] = reg[ins.rs1] + reg[ins.rs2];
                        break;
                    case sub:
                        reg[ins.rd] = reg[ins.rs1] - reg[ins.rs2];
                        break;
                    case sll:
                        reg[ins.rd] = reg[ins.rs1] << (reg[ins.rs2] & 0x1F);
                        break;
                    case slt:
                        reg[ins.rd] = reg[ins.rs1] < reg[ins.rs2];
                        break;
                    case sltu:
                        reg[ins.rd] = static_cast<unsigned int>(reg[ins.rs1]) < static_cast<unsigned int>(reg[ins.rs2]);
                        break;
                    case xor_:
                        reg[ins.rd] = reg[ins.rs1] ^ reg[ins.rs2];
                        break;
                    case srl:
                        reg[ins.rd] = static_cast<unsigned int>(reg[ins.rs1]) >> (reg[ins.rs2] & 0x1F);
                        break;
                    case sra:
                        reg[ins.rd] = reg[ins.rs1] >> (reg[ins.rs2] & 0x1F);
                        break;
                    case or_:
                        reg[ins.rd] = reg[ins.rs1] | reg[ins.rs2];
                        break;
                    case and_:
                        reg[ins.rd] = reg[ins.rs1] & reg[ins.rs2];
                        break;
                }
                break;
            case B_TYPE:
                switch (ins.op) {
                    case beq:
                        if (reg[ins.rs1] == reg[ins.rs2]) {
                            pc += sext(ins.imm, 13);
                        } else addPC();
                        break;
                    case bne:
                        if (reg[ins.rs1] != reg[ins.rs2]) {
                            pc += sext(ins.imm, 13);
                        } else addPC();
                        break;
                    case blt:
                        if (reg[ins.rs1] < reg[ins.rs2]) {
                            pc += sext(ins.imm, 13);
                        } else addPC();
                        break;
                    case bge:
                        if (reg[ins.rs1] >= reg[ins.rs2]) {
                            pc += sext(ins.imm, 13);
                        } else addPC();
                        break;
                    case bltu:
                        if (static_cast<unsigned int>(reg[ins.rs1]) < static_cast<unsigned int>(reg[ins.rs2])) {
                            pc += sext(ins.imm, 13);
                        } else addPC();
                        break;
                    case bgeu:
                        if (static_cast<unsigned int>(reg[ins.rs1]) >= static_cast<unsigned int>(reg[ins.rs2])) {
                            pc += sext(ins.imm, 13);
                        } else addPC();
                        break;
                }
                break;
            case Type:
            default:
                break;
        }
        reg[0] = 0;
    }

    void pre() {
#ifdef debug
        loadCode();
#endif
#ifndef debug
        cinCode();
#endif
    }

    void work1step() {

        int clock = 0;
//        while (pc < 1 << 20) {
//    while (pc<1<<20&&clock<1000){
#ifdef debug
//        ++clock;
        assert(pc % 4 == 0);
        assert(get8() == 0x0ff00513);
//        if (clock%1000==0)
//            cout << clock << "  " << hex << pc << dec << endl;
#endif
        step();
        instruction ins = decode();
        interpreter(ins);
//        if (ins.tp == B_TYPE || ins.op == jal || ins.op == jalr)continue;
        if (ins.tp == B_TYPE || ins.op == jal || ins.op == jalr)return;
        addPC();
//        }
    }
};

#endif