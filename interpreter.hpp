//
// Created by qiuyuhang on 24-7-25.
//

#ifndef MYPROJECT_INTERPRETER_HPP
#define MYPROJECT_INTERPRETER_HPP

#include "memory.hpp"
#include "decode.hpp"
#include "register.hpp"
#include <cassert>
#include "decode.hpp"

int sext(int imm, int len) {
    if (len==32){
        return imm;
    }
    if ((imm >> (len - 1)) & 1) {
//        imm |= ((0xFFFFFFFF << len));
//        cout <<imm<<"   "<<((0xFFFFFFFF << len))<<"   "<<((long long) imm | (long long) (0xFFFFFFFF << len)) << "    ";
        long long tmp = (long long) imm | (long long) (0xFFFFFFFF << len);
//        cout << tmp << endl;
        return tmp;
//        return imm | ((0xFFFFFFFF << len));
    }
    return imm;
}

//void execute(instruction ins) {
//    int temp = 0;
//    switch (ins.tp) {
//        case U_TYPE:
//            switch (ins.op) {
//                case lui:
//                    reg[ins.rd] = ins.imm;
//                    break;
//                case auipc:
//                    reg[ins.rd] = ins.imm + pc;
//                    break;
//            }
//            break;
//        case J_TYPE:
//            switch (ins.op) {
//                case jal:
//                    reg[ins.rd] = pc + 4;
//                    pc += ins.imm;
//                    break;
//            }
//            break;
//        case I_TYPE:
//            switch (ins.op) {
//                case jalr:
//                    temp = pc + 4;
//                    pc = (reg[ins.rs1] + ins.imm) & ~1;
//                    reg[ins.rd] = temp;
//                    break;
//                case lb:
//                    reg[ins.rd] = (memory[reg[ins.rs1] + ins.imm]);
//                    break;
//                case lh:
//                    reg[ins.rd] = memory[reg[ins.rs1] + ins.imm] + (memory[reg[ins.rs1] + ins.imm + 1] << 8);
//                    break;
//                case lw:
//                    reg[ins.rd] = memory[reg[ins.rs1] + ins.imm] +
//                                  (memory[reg[ins.rs1] + ins.imm + 1] << 8) +
//                                  (memory[reg[ins.rs1] + ins.imm + 2] << 16) +
//                                  (memory[reg[ins.rs1] + ins.imm + 3] << 24);
//                    break;
//                case lbu:
//                    reg[ins.rd] = memory[reg[ins.rs1] + ins.imm];
//                    break;
//                case lhu:
//                    reg[ins.rd] = memory[reg[ins.rs1] + ins.imm] + (memory[reg[ins.rs1] + ins.imm + 1] << 8);
//                    break;
//                case addi:
//                    reg[ins.rd] = reg[ins.rs1] + ins.imm;
//                    break;
//                case slti:
//                    reg[ins.rd] = reg[ins.rs1] < ins.imm;
//                    break;
//                case sltiu:
//                    reg[ins.rd] = reg[ins.rs1] < ins.imm;
//                    break;
//                case xori:
//                    reg[ins.rd] = reg[ins.rs1] ^ ins.imm;
//                    break;
//                case ori:
//                    reg[ins.rd] = reg[ins.rs1] | ins.imm;
//                    break;
//                case andi:
//                    reg[ins.rd] = reg[ins.rs1] & ins.imm;
//                    break;
//                case slli:
//                    reg[ins.rd] = reg[ins.rs1] << ins.imm;
//                    break;
//                case srli:
//                    reg[ins.rd] = reg[ins.rs1] >> ins.imm;
//                    break;
//            }
//            break;
//        case S_TYPE:
//            switch (ins.op) {
//                case sb:
//                    memory[reg[ins.rs1] + ins.imm] = reg[ins.rs2];
//                    break;
//                case sh:
//                    memory[reg[ins.rs1] + ins.imm] = reg[ins.rs2];
//                    memory[reg[ins.rs1] + ins.imm + 1] = reg[ins.rs2] >> 8;
//                    break;
//                case sw:
//                    memory[reg[ins.rs1] + ins.imm] = reg[ins.rs2];
//                    memory[reg[ins.rs1] + ins.imm + 1] = reg[ins.rs2] >> 8;
//                    memory[reg[ins.rs1] + ins.imm + 2] = reg[ins.rs2] >> 16;
//                    memory[reg[ins.rs1] + ins.imm + 3] = reg[ins.rs2] >> 24;
//                    break;
//            }
//            break;
//        case R_TYPE:
//            switch (ins.op) {
//                case add:
//                    reg[ins.rd] = reg[ins.rs1] + reg[ins.rs2];
//                    break;
//                case sub:
//                    reg[ins.rd] = reg[ins.rs1] - reg[ins.rs2];
//                    break;
//                case sll:
//                    reg[ins.rd] = reg[ins.rs1] << reg[ins.rs2];
//                    break;
//                case slt:
//                    reg[ins.rd] = reg[ins.rs1] < reg[ins.rs2];
//                    break;
//                case sltu:
//                    reg[ins.rd] = reg[ins.rs1] < reg[ins.rs2];
//                    break;
//                case xor_:
//                    reg[ins.rd] = reg[ins.rs1] ^ reg[ins.rs2];
//                    break;
//                case srl:
//                    reg[ins.rd] = reg[ins.rs1] >> reg[ins.rs2];
//                    break;
//                case sra:
//                    reg[ins.rd] = reg[ins.rs1] >> reg[ins.rs2];
//                    break;
//                case or_:
//                    reg[ins.rd] = reg[ins.rs1] | reg[ins.rs2];
//                    break;
//                case and_:
//                    reg[ins.rd] = reg[ins.rs1] & reg[ins.rs2];
//                    break;
//            }
//            break;
//        case B_TYPE:
//            switch (ins.op) {
//                case beq:
//                    if (reg[ins.rs1] == reg[ins.rs2]) {
//                        pc += ins.imm;
//                    }
//                    break;
//                case bne:
//                    if (reg[ins.rs1] != reg[ins.rs2]) {
//                        pc += ins.imm;
//                    }
//                    break;
//                case blt:
//                    if (reg[ins.rs1] < reg[ins.rs2]) {
//                        pc += ins.imm;
//                    }
//                    break;
//                case bge:
//                    if (reg[ins.rs1] >= reg[ins.rs2]) {
//                        pc += ins.imm;
//                    }
//                    break;
//                case bltu:
//                    if (reg[ins.rs1] < reg[ins.rs2]) {
//                        pc += ins.imm;
//                    }
//                    break;
//                case bgeu:
//                    if (reg[ins.rs1] >= reg[ins.rs2]) {
//                        pc += ins.imm;
//                    }
//                    break;
//            }
//            break;
//        case Type:
//        default:
//            break;
//    }
//}

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
//            cout << "dest: " << reg[ins.rs1] + sext(ins.imm, 12) << "   value:" << reg[ins.rs2] << endl;
            switch (ins.op) {
                default:
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

#endif //MYPROJECT_INTERPRETER_HPP
