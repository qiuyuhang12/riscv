//
// Created by qiuyuhang on 24-7-30.
//
#include "reserve_station.hpp"
#include "common_data_bus.hpp"
#include "register.hpp"
#include "load_store_buffer.hpp"

void Rs::query(int index) {
    if (rs[index].entry1 != -1) {
        auto tmp = cdb->get(rs[index].entry1);
        if (tmp.first) {
            rsNext[index].rs1 = tmp.second;
            rsNext[index].entry1 = -1;
        }
    }
    if (rs[index].entry2 != -1) {
        auto tmp = cdb->get(rs[index].entry2);
        if (tmp.first) {
            rsNext[index].rs2 = tmp.second;
            rsNext[index].entry2 = -1;
        }
    }
}

void Rs::execute(int index) {
#ifdef debug
    cout << "Rs execute pc:" << std::hex << rs[index].pc << "  entry: " << rs[index].dest << endl;
#endif
    if (rs[index].isB) {
        alu->inBuffer(rs[index].rs1, rs[index].rs2, rs[index].imm, rs[index].op, rs[index].dest, rs[index].pc);
    } else {
        alu->alu(rs[index].rs1, rs[index].rs2, rs[index].imm, rs[index].op, rs[index].dest);
    }
}

Rs::Rs() {
    for (int i = 0; i < capacity; i++) {
        rs[i].init();
        rsNext[i].init();
    }
}

Rs::Rs(Reg *_reg, Lsb *_lsb, Rob *_rob, Cdb *_cdb, Alu *_alu) : reg(_reg), lsb(_lsb), rob(_rob), cdb(_cdb), alu(_alu) {
    for (int i = 0; i < capacity; i++) {
        rs[i].init();
        rsNext[i].init();
    }
}

void Rs::init(Reg *_reg, Lsb *_lsb, Rob *_rob, Cdb *_cdb, Alu *_alu) {
    reg = _reg;
    lsb = _lsb;
    rob = _rob;
    cdb = _cdb;
    alu = _alu;
    for (int i = 0; i < capacity; i++) {
        rs[i].init();
        rsNext[i].init();
    }
}

void Rs::flush() {
#ifdef detail
    cout << "rs flush" << endl;
    for (int i = 0; i < capacity; ++i) {
        if (rs[i] == rsNext[i]) {
            continue;
        }
        rs[i].print();
        rsNext[i].print();
    }
#endif
    for (int i = 0; i < capacity; i++) {
        rs[i] = rsNext[i];
    }
}

bool Rs::isFull() {
    for (int i = 0; i < capacity; i++) {
        if (!rs[i].busy) {
            return false;
        }
    }
    return true;
}

void Rs::issue(int entry, instruction inst, int pc) {
#ifdef debug
    cout << "Rs issue pc:" << std::hex << pc << "  entry: " << entry << endl;
#endif
#ifdef detail
//    cout << "RS issue pc: " << reg->pcReg <<"  entry: "<<entry<< endl;
    inst.print();
#endif
    assert(inst.op != jal);
    for (int i = 0; i < capacity; i++) {
        if (!rs[i].busy) {
            rsNext[i].init();
            rsNext[i].inst = inst;
            rsNext[i].busy = true;
            rsNext[i].op = inst.op;
            rsNext[i].dest = entry;
            rsNext[i].pc = pc;
//                if (inst.op==opcode::jal){
//                    rsNext[i].rs2 = reg->pcReg;
//                    rsNext[i].imm=inst.imm;
//                    return;
//                }
            if (reg->reg[inst.rs1].busy) {
                auto tmp = cdb->get(reg->reg[inst.rs1].entry);
                if (tmp.first) {
                    rsNext[i].rs1 = tmp.second;
                    rsNext[i].entry1 = -1;
                } else {
                    rsNext[i].entry1 = reg->reg[inst.rs1].entry;
                }
            } else {
                rsNext[i].rs1 = reg->reg[inst.rs1].value;
            }
            switch (inst.tp) {
                case U_TYPE:
                    assert(inst.op == auipc);
                    rsNext[i].rs1 = reg->pcReg;
                    rsNext[i].imm = inst.imm;
                    break;
                case I_TYPE:
                    rsNext[i].imm = inst.imm;
                    break;
                case R_TYPE:
                    if (reg->reg[inst.rs2].busy) {
                        auto tmp = cdb->get(reg->reg[inst.rs2].entry);
                        if (tmp.first) {
                            rsNext[i].rs2 = tmp.second;
                            rsNext[i].entry2 = -1;
                        } else {
                            rsNext[i].entry2 = reg->reg[inst.rs2].entry;
                        }
                    } else {
                        rsNext[i].rs2 = reg->reg[inst.rs2].value;
                    }
                    break;
                case S_TYPE:
                    if (reg->reg[inst.rs2].busy) {
                        auto tmp = cdb->get(reg->reg[inst.rs2].entry);
                        if (tmp.first) {
                            rsNext[i].rs2 = tmp.second;
                            rsNext[i].entry2 = -1;
                        } else {
                            rsNext[i].entry2 = reg->reg[inst.rs2].entry;
                        }
                    } else {
                        rsNext[i].rs2 = reg->reg[inst.rs2].value;
                    }
                    rsNext[i].imm = inst.imm;
                    break;
                case B_TYPE:
                    if (reg->reg[inst.rs2].busy) {
                        auto tmp = cdb->get(reg->reg[inst.rs2].entry);
                        if (tmp.first) {
                            rsNext[i].rs2 = tmp.second;
                            rsNext[i].entry2 = -1;
                        } else {
                            rsNext[i].entry2 = reg->reg[inst.rs2].entry;
                        }
                    } else {
                        rsNext[i].rs2 = reg->reg[inst.rs2].value;
                    }
                    rsNext[i].imm = inst.imm;
                    rsNext[i].isB = true;
                    break;
                default:
                    assert(0);//unknown type
            }
            return;
        }
    }
    assert(0);//rs is full
}

void Rs::step() {
    for (int i = 0; i < capacity; i++) {
        if (rs[i].busy) {
            query(i);
        }
    }
    int tobeExecuted = -1;
    for (int i = 0; i < capacity; i++) {
        if (rs[i].busy) {
            if (rs[i].entry1 == -1 && rs[i].entry2 == -1) {
                if (tobeExecuted==-1||rs[i].inst.clock<rs[tobeExecuted].inst.clock){
                    tobeExecuted=i;
                }
//                execute(i);
////                    rob->commit(rs[i].dest, rs[i].a);
////                    cdb->commit(rs[i].a, rs[i].dest);
//                rsNext[i].init();
//                break;
            }
        }
    }
    if (tobeExecuted!=-1){
        execute(tobeExecuted);
        rsNext[tobeExecuted].init();
    }
}