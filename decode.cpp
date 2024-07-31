//
// Created by qiuyuhang on 24-7-30.
//
#include "decode.hpp"

void instruction::print() {
    std::cout<<"inst    ";
    switch (tp) {
        case U_TYPE:
            switch (op) {
                case lui:
                    std::cout << "lui x" << rd << " " <<std::hex<< imm  << std::endl;
                    break;
                case auipc:
                    std::cout << "auipc x" << rd << " " <<std::hex<< imm  << std::endl;
                    break;
                default:
                    std::cout << "undefined" << std::endl;
                    break;
            }
            break;
        case J_TYPE:
            switch (op) {
                case jal:
                    std::cout << "jal x" << rd << " " <<std::hex<< imm  << std::endl;
                    break;
                default:
                    std::cout << "undefined" << std::endl;
                    break;
            }
            break;
        case I_TYPE:
            switch (op) {
                case lb:
                    std::cout << "lb x" << rd << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                case lh:
                    std::cout << "lh x" << rd << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                case lw:
                    std::cout << "lw x" << rd << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                case lbu:
                    std::cout << "lbu x" << rd << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                case lhu:
                    std::cout << "lhu x" << rd << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                case addi:
                    std::cout << "addi x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case slti:
                    std::cout << "slti x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case sltiu:
                    std::cout << "sltiu x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case xori:
                    std::cout << "xori x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case ori:
                    std::cout << "ori x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case andi:
                    std::cout << "andi x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case slli:
                    std::cout << "slli x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case srli:
                    std::cout << "srli x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                case srai:
                    std::cout << "srai x" << rd << " x" << rs1 << " " <<std::hex<< imm  << std::endl;
                    break;
                default:
                    std::cout << "undefined" << std::endl;
                    break;
            }
            break;
        case B_TYPE:
            switch (op) {
                case beq:
                    std::cout << "beq x" << rs1 << " x" << rs2 << " " <<std::hex<< imm  << std::endl;
                    break;
                case bne:
                    std::cout << "bne x" << rs1 << " x" << rs2 << " " <<std::hex<< imm  << std::endl;
                    break;
                case blt:
                    std::cout << "blt x" << rs1 << " x" << rs2 << " " <<std::hex<< imm  << std::endl;
                    break;
                case bge:
                    std::cout << "bge x" << rs1 << " x" << rs2 << " " <<std::hex<< imm  << std::endl;
                    break;
                case bltu:
                    std::cout << "bltu x" << rs1 << " x" << rs2 << " " <<std::hex<< imm  << std::endl;
                    break;
                case bgeu:
                    std::cout << "bgeu x" << rs1 << " x" << rs2 << " " <<std::hex<< imm  << std::endl;
                    break;
                default:
                    std::cout << "undefined" << std::endl;
                    break;
            }
            break;
        case S_TYPE:
            switch (op) {
                case sb:
                    std::cout << "sb x" << rs2 << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                case sh:
                    std::cout << "sh x" << rs2 << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                case sw:
                    std::cout << "sw x" << rs2 << " " <<std::hex<< imm  << "(x" << rs1 << ")" << std::endl;
                    break;
                default:
                    std::cout << "undefined" << std::endl;
                    break;
            }
            break;
        case R_TYPE:
            switch (op) {
                case add:
                    std::cout << "add x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case sub:
                    std::cout << "sub x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case sll:
                    std::cout << "sll x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case slt:
                    std::cout << "slt x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case sltu:
                    std::cout << "sltu x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case xor_:
                    std::cout << "xor x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case srl:
                    std::cout << "srl x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case sra:
                    std::cout << "sra x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case or_:
                    std::cout << "or x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                case and_:
                    std::cout << "and x" << rd << " x" << rs1 << " x" << rs2 << std::endl;
                    break;
                default:
                    std::cout << "undefined" << std::endl;
                    break;
            }
            break;
        case END:
            std::cout << "end" << std::endl;
            break;
        default:
            std::cout << "undefined" << std::endl;
            break;
    }
}
instruction decode(int iR){
    instruction ins;
    ins.iR = iR;
    if (iR == 0x0ff00513) {
        ins.tp = END;
        ins.op = opcode::end;
        ins.rd = -1;
        ins.rs1 = -1;
        ins.rs2 = -1;
        ins.imm = -1;
        ins.pc = -1;
        return ins;
//        std::cout << (reg[10] & 0xff);
//        exit(0);
//        exit(reg[10]);
    }
    ins.rd = (iR >> 7) & 31;
    ins.rs1 = (iR >> 15) & 31;
    ins.rs2 = (iR >> 20) & 31;
    int opcode = iR & 127;
    ins.originalOp = opcode;
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
