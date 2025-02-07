//
// Created by qiuyuhang on 24-7-30.
//
#include "algorithm_logic_unit.hpp"
#include "common_data_bus.hpp"

void Alu::bExe() {
    outBufferNext = buffer;
    switch (buffer.op) {
        case beq:
            if (buffer.rs1 == buffer.rs2) {
                outBufferNext.rsl = true;
            } else outBufferNext.rsl = false;
            break;
        case bne:
            if (buffer.rs1 != buffer.rs2) {
                outBufferNext.rsl = true;
            } else outBufferNext.rsl = false;
            break;
        case blt:
            if (buffer.rs1 < buffer.rs2) {
                outBufferNext.rsl = true;
            } else outBufferNext.rsl = false;
            break;
        case bge:
            if (buffer.rs1 >= buffer.rs2) {
                outBufferNext.rsl = true;
            } else outBufferNext.rsl = false;
            break;
        case bltu:
            if (static_cast<unsigned int>(buffer.rs1) < static_cast<unsigned int>(buffer.rs2)) {
                outBufferNext.rsl = true;
            } else outBufferNext.rsl = false;
            break;
        case bgeu:
            if (static_cast<unsigned int>(buffer.rs1) >= static_cast<unsigned int>(buffer.rs2)) {
                outBufferNext.rsl = true;
            } else outBufferNext.rsl = false;
            break;
        default:
            assert(0);
    }
    if (outBufferNext.rsl == true) {
        outBufferNext.pc += sext(buffer.imm, 13);
    } else {
        outBufferNext.pc += 4;
    }
}

void Alu::bWrite() {
    bWorkingNext = false;
    cdb->writeBr(outBuffer.pc, outBuffer.rsl, outBuffer.entry);
    bufferNext.init();
    outBufferNext.init();
}

Alu::Alu() {}

Alu::Alu(Cdb *cdb) : cdb(cdb) {}

void Alu::init(Cdb *cdb) {
    this->cdb = cdb;
}

void Alu::step() {
    if (buffer.busy) {
        if (!outBuffer.done) {
            bExe();
//            bufferNext.done = true;
            outBufferNext.done = true;
        } else {
            bWrite();
        }
    }
}

void Alu::inBuffer(int rs1, int rs2, int imm, opcode op, int entry, int pc) {
    bWorkingNext = true;
    bufferNext.init();
    bufferNext.busy = true;
    bufferNext.rs1 = rs1;
    bufferNext.rs2 = rs2;
    bufferNext.imm = imm;
    bufferNext.pc = pc;
    bufferNext.op = op;
    bufferNext.entry = entry;
}

void Alu::alu(int vi, int vj, int imm, opcode op, int entry) {
    int res = 0;
    bool memAddr = false;
    switch (op) {
//U-type
        case auipc:
            res = (sext(imm, 32)) + vi;
            break;
//S-type(store)
        case sb:
        case sh:
        case sw:
            res = (vi + sext(imm, 12)) & ~1;
            memAddr = true;
            break;
//I-type(jalr)
        case jalr:
            memAddr = true;
            res = (vi + sext(imm, 12)) & ~1;
            break;
//I-type(load)
        case lb:
        case lh:
        case lw:
        case lbu:
        case lhu:
            memAddr = true;
            res = vi + sext(imm, 12);
            break;
//I-type
        case addi:
            res = vi + sext(imm, 12);
            break;
        case slti:
            res = vi < sext(imm, 12);
            break;
        case sltiu:
            res = vi < static_cast<unsigned int>(sext(imm, 12));
            break;
        case xori:
            res = vi ^ sext(imm, 12);
            break;
        case ori:
            res = vi | sext(imm, 12);
            break;
        case andi:
            res = vi & sext(imm, 12);
            break;
        case slli:
            assert((imm & (1 << 5)) == 0);
            res = vi << (imm & 0x1F);
            break;
        case srli:
            assert((imm & (1 << 5)) == 0);
            res = static_cast<unsigned int>(vi) >> (imm & 0x1F);
            break;
        case srai:
            assert((imm & (1 << 5)) == 0);
            res = vi >> (imm & 0x1F);
            break;
//R-type
        case add:
            res = vi + vj;
            break;
        case sub:
            res = vi - vj;
            break;
        case sll:
            res = vi << (vj & 0x1F);
            break;
        case slt:
            res = vi < vj;
            break;
        case sltu:
            res = static_cast<unsigned int>(vi) < static_cast<unsigned int>(vj);
            break;
        case xor_:
            res = vi ^ vj;
            break;
        case srl:
            res = static_cast<unsigned int>(vi) >> (vj & 0x1F);
            break;
        case sra:
            res = vi >> (vj & 0x1F);
            break;
        case or_:
            res = vi | vj;
            break;
        case and_:
            res = vi & vj;
            break;
    }
    cdb->write(res, entry, memAddr);
}