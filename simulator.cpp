//
// Created by qiuyuhang on 24-7-30.
//
#include "simulator.hpp"
#include <cassert>

void Simulator::addPC() {
    pcNext += 4;
    if (pcNext >= 1 << 20) {
        exit(0);
    }
}

void Simulator::addPC(int imm) {
    pcNext += sext(imm, 21);
    assert(imm % 4 == 0);
    assert(pc % 4 == 0);
    if (pc >= 1 << 20) {
        exit(0);
    }
}

void Simulator::addPC(int reg_rs1, int imm) {
    pcNext = (reg_rs1 + sext(imm, 12)) & ~1;
    assert(pc % 4 == 0);
    if (pc >= 1 << 20) {
        exit(0);
    }
}

void Simulator::flush() {
//#ifdef debug
#ifdef detail
    std::cout << std::hex << "pc: " << pc << "    pcNext: " << pcNext << std::endl;
#endif
#ifdef regshow
    //    if (clo==0x119){
            reg.print(0xf,0x10);
    //    }
#endif

    pc = pcNext;
    iR = 0;
    memory.flush();

    if (cdb.clearUp== true){
        rob.clear();
        return;
    }
    reg.flush();
    rob.flush();
    lsb.flush();
    rs.flush();
    cdb.flush();
    alu.flush();
//todo
    cdb.pcFrozen = false;
}

#ifndef debug

Simulator::Simulator() : reg(pc, pcNext) {
#endif
#ifdef debug
    Simulator::Simulator() : reg(pc, pcNext), memory(filePath) {
#endif
    rob.init(&reg, &lsb, &cdb, &memory, &rs, &predictor);
    lsb.init(&reg, &rob, &memory, &cdb);
    rs.init(&reg, &lsb, &rob, &cdb, &alu);
    alu.init(&cdb);
}

void Simulator::work() {
    int cccclock = 0;
    while (true) {
//#ifdef debug
//        if (cccclock%1000==0)
#ifdef debug
        cout << "-----------------------------------------" << cccclock
             << "-----------------------------------------" <<boolalpha<<cdb.jalrPanic<< endl;
#endif
//        if (rob.queue[5].state!=Rob::UNKNOWN){
//            cout<<"rob[5]:"<<rob.queue[5].state<<endl;
//            rob.queue[5].print();
//        }
//#endif
//        if (clo > 0x7880) {
//            break;
//        }
//        cout<<clock<<endl;

        if (cccclock & 1) {
//            cout<<"cdb[0]:"<<cdb.cdb[0].state<<"  "<<cdb.cdbNext[0].state<<endl;
            flush();
#ifdef debug
            if (rob.queue.isEmpty()){
                for (int i = 0; i < 32; ++i) {
                    assert(!reg.reg[i].busy);
                }
            }
#endif
        } else {
//            if (clock%1000==0)

#ifdef debug
            cout << boolalpha << rob.isFull() << "    " << rs.isFull() << "    " << lsb.isFull() << "    "
                 << memory.working << endl;
#endif

            bool full = rob.isFull() || rs.isFull() || lsb.isFull() || memory.working;
            if (!cdb.jalrPanic && !full) {
                int iR = memory.getiR(pc);
                auto inst = decode(iR);
                inst.pc = pc;
                rob.issue(iR);
//                reg.print(0x15,0x16);
                if (inst.tp != type::U_TYPE && inst.tp != type::J_TYPE && inst.op != opcode::end) {
                    rs.issue(rob.getNextRearIndex(), inst, pc);
//                    reg.print(0x15,0x16);
                }
                // load & store
                if ((inst.tp == type::S_TYPE || inst.originalOp == 3) && inst.op != opcode::end) {
                    lsb.issue(inst, rob.getNextRearIndex());
                }
                if (!cdb.pcFrozen) {
                    addPC();
                } else {
                    cdb.pcFrozen = false;
                }
//                reg.print(0xf, 0x10);
            }
            rob.step();
//            reg.print(0xf, 0x10);

            rs.step();
//            reg.print(0xf, 0x10);

            lsb.step();
            memory.step();
            alu.step();
        }
        ++cccclock;
        ++clo;
    }
}