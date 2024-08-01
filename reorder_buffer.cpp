//
// Created by qiuyuhang on 24-7-30.
//
#include "reorder_buffer.hpp"
#include "utility.hpp"
#include "interpreter.hpp"

extern Interpreter interpreter;

void Rob::check(int pc) {

    static int checkCount = 0;
    checkCount++;
    bool flag = false;
    cout << "work1step: checkCount: " << checkCount << endl;
    if (interpreter.pc != pc) {
        cerr << " your:" << hex << pc << " true pc: 0x" << interpreter.pc << endl;
        flag = true;
//        assert(0);
    }
    interpreter.work1step();
#ifdef single
    for (int i = 0; i < CdbCapacity; ++i) {
        if (cdb->cdbNext[i].state != Cdb::LEISURE) {
            cerr << "cdb: " << i << " state: " << cdb->cdbNext[i].state << endl;
            flag = true;
        }
    }
#endif
    for (int i = 1; i < 32; ++i) {
//        assert(interpreter.reg[i] == reg->regNext[i].value);
        if (interpreter.reg[i] != reg->regNext[i].value) {
            cerr << "reg: 0x" << hex << i << "  your:" << reg->regNext[i].value << "   truth:" << interpreter.reg[i]
                 << endl;
//            cout <<"reg: 0x"<<hex<< i << "  your:" << reg->regNext[i].value << "   truth:" << interpreter.reg[i] << endl;
            flag = true;
        }
    }
    for (int i = 0; i < MemoryCapacity; ++i) {
        if (interpreter.memory[i] != memory->memory[i]) {
            cerr << "mem: 0x" << hex << i << "  your:" << memory->memory[i] << "   truth:" <<hex<< interpreter.memory[i]
                 << endl;
//            cout <<"mem: 0x"<<hex<< i << "  your:" << memory->memory[i] << "   truth:" << interpreter.memory[i] << endl;
            flag = true;
        }
//        assert(interpreter.memory[i] == memory->memory[i]);
    }
    if (flag) {
//        exit(0);
        assert(0);
    }
}

void Rob::Unit::print() {
    cout << "entry:" << entry << " busy:" << busy << " state:" << state << " dest:" << dest << " value:" << value
         << " pc:" << pc;
    inst.print();
}

void Rob::commit() {
    Unit tmp = queue.front();
#ifdef debug
    cout << "Rob commit pc:" << std::hex << tmp.pc << "  entry: " << tmp.entry << endl;
#endif
    if (tmp.inst.op == opcode::end) {
        cout << (reg->reg[10].value & 0xff);
        exit(0);
    }
    assert(tmp.state == WRITE);
    if (tmp.inst.originalOp == 3 || tmp.inst.tp == type::S_TYPE) {//LOAD,STORE,不进入switch
        if (!memory->working && lsb->ready()) {
            queueNext.front().state = COMMIT;
            lsb->commit(queue.front().entry);
        } else {
#ifdef debug
            cout << "commit fail" << endl;
#endif
        }
        return;
    }
    int pc = tmp.pc;
    switch (tmp.inst.tp) {
        case U_TYPE:
        case type::J_TYPE:
        case type::R_TYPE:
        case type::I_TYPE:
            reg->regNext[tmp.dest].value = tmp.value;
            if (reg->regNext[tmp.dest].entry == tmp.entry) {
                reg->regNext[tmp.dest].busy = false;
                reg->regNext[tmp.dest].entry = -1;
            }
            queueNext.front().state = COMMIT;
            if (tmp.inst.op == jal || tmp.inst.op == jalr) {
                //在 receive 时未删除
                cdb->erase(queue.frontIndex, true);
                cdb->erase(queue.frontIndex);
            } else {
                cdb->erase(queue.frontIndex);
            }
            queueNext.dequeue();
            break;
        case type::B_TYPE:
            queueNext.front().state = COMMIT;
            cdb->eraseBr(queue.frontIndex);
            if (!tmp.branch) {
                wrongPredicted(queue.frontIndex, tmp.value);
            }
            queueNext.dequeue();
            break;
        default:
            assert(0);
    }
#ifdef debug
    check(pc);
#endif
}

void Rob::wrongPredicted(int entry, int pc) {
#ifdef debug
    cout << "rob wrongPredicted nextPc:" << std::hex << pc << "  entry: " << entry << endl;
#endif
    cdb->clearUp = true;
    cdb->pcFrozen = true;
    reg->nextPCReg = pc;
//    queueNext.clear();
////    queueNext.cut(entry);
//    lsb->clear();
//    rs->clear();
//    reg->clear();
//    cdb->clear();
}

void Rob::receiveBroadcast() {
    //todo::front>rear!!!时候
//    for (int i = queue.frontIndex; i <= queue.rearIndex; i++) {
    for (int i = 0; i < capacity; i++) {
        if (!queue.isInQueue(i)) {
            continue;
        }
        if (queue[i].state == ISSUE) {
            if (queue[i].inst.op == opcode::jal || queue[i].inst.op == opcode::jalr) {
                auto tmp = cdb->get(queue[i].entry, true, true);
                if (!tmp.first) {
                    continue;
                }
#ifdef debug
                cout << "rob receiveBroadcast pc:" << std::hex << queue[i].pc << "  entry: " << i << endl;
#endif
                if (queue[i].inst.op == opcode::jalr) {
//                    cdb->stop= false;
                    reg->nextPCReg = tmp.second;
                    cdb->jalrPanic = false;
                    queueNext[i].state = WRITE;
                    continue;
                }
                queueNext[i].pc = tmp.second;
                queueNext[i].state = WRITE;
                continue;
            }
            if (queue[i].inst.tp == type::B_TYPE) {
                auto tmp = cdb->getBr(queue[i].entry);
                if (!tmp.first) {
                    continue;
                }
#ifdef debug
                cout << "rob receiveBroadcast pc:" << std::hex << queue[i].pc << "  entry: " << i << endl;
#endif
//                cout << "ppppppppppppppppppppppc: " << std::hex << reg->nextPCReg << endl;
                if (queue[i].value != tmp.second.second) {
                    queueNext[i].value = tmp.second.first;//从bool变成pc值
                    queueNext[i].branch = false;
//                    wrongPredicted(i, tmp.second.first);
                    predictor->update(queue[i].inst.pc, false);
                } else {
                    queueNext[i].branch = true;
                    predictor->update(queue[i].inst.pc, true);
                }
                queueNext[i].state = WRITE;
                continue;
            }
//                if (queue[i].inst.op==opcode::jal||queue[i].inst.op==opcode::jalr){
//                    queueNext[i].pc=pc;
//                    queueNext[i].state=WRITE;
//                    return;
//                }
            auto tmp = cdb->get(queue[i].entry);
            if (tmp.first) {
#ifdef debug
                cout << "rob receiveBroadcast pc:" << std::hex << queue[i].pc << "  entry: " << i << endl;
#endif

                queueNext[i].value = tmp.second;
                queueNext[i].state = WRITE;
            }
        }
    }
}

Rob::Rob() {}

Rob::Rob(Reg *_reg, Lsb *_lsb, Cdb *_cdb, Memory *_memory, Rs *_rs, Predictor *_predictor) : reg(_reg), lsb(_lsb),
                                                                                             cdb(_cdb), memory(_memory),
                                                                                             rs(_rs),
                                                                                             predictor(_predictor) {}

void Rob::init(Reg *_reg, Lsb *_lsb, Cdb *_cdb, Memory *_memory, Rs *_rs, Predictor *_predictor) {
    reg = _reg;
    lsb = _lsb;
    cdb = _cdb;
    memory = _memory;
    rs = _rs;
    predictor = _predictor;
}

int Rob::getNextRearIndex() {
    return queue.getNextRearIndex();
}

void Rob::flush() {
#ifdef detail
    cout << "rob flush" << endl;
    for (int i = 0; i < capacity; ++i) {
        if (queue[i] == queueNext[i]) {
            continue;
        }
        cout << "rob: " << i << endl;
        queue[i].print();
        queueNext[i].print();
    }
#endif
    queue = queueNext;
}

void Rob::step() {
    receiveBroadcast();
    if (queue.isEmpty()) {
        return;
    }
    auto tmp = queue.front();
    if (tmp.state == TODELETECDB) {
#ifdef debug
        cout << "rob TODELETECDB pc:" << std::hex << tmp.pc << "  entry: " << queue.frontIndex << endl;
#endif
        queueNext.dequeue();
        if (tmp.inst.tp != S_TYPE)cdb->erase(queue.frontIndex);//只有S指令才不广播
#ifdef debug
        check(queue.front().pc);
#endif
    }
    if (tmp.state == WRITE || tmp.inst.op == opcode::end) {
        commit();
//    } else if (tmp.state == COMMIT) {
    } else if (tmp.inst.tp == type::S_TYPE || tmp.inst.originalOp == 3) {
//        assert(tmp.inst.tp==type::S_TYPE||tmp.inst.originalOp==3);
//        if (!memory->working) {
        if (!memory->working && tmp.state == COMMIT) {
            queueNext.front().state = TODELETECDB;
        }
    }
}

void Rob::issue(int iR) {
    instruction inst = decode(iR);
    Unit unit;
    if (inst.op == opcode::end) {
        unit.inst = inst;
        unit.state = WRITE;
        unit.busy = true;
        unit.entry = queue.getNextRearIndex();
        unit.pc = reg->pcReg;
        queueNext.enqueue(unit);
        return;
    }
#ifdef debug
    cout << "Rob issue pc:" << std::hex << reg->pcReg << "  entry: " << queue.getNextRearIndex() << endl;
#endif
#ifdef detail
    //    cout << "Rob issue pc:" << std::hex << reg->pcReg << endl;
    inst.print();
#endif
    unit.inst = inst;
    unit.busy = true;
    unit.state = ISSUE;
    unit.dest = inst.rd;
    unit.entry = queue.getNextRearIndex();
    unit.pc = reg->pcReg;
    if (inst.tp == type::S_TYPE || inst.originalOp == 3) {
        unit.state = WRITE;
    }
    if (inst.rd != -1 && inst.tp != type::S_TYPE && inst.tp != type::B_TYPE) {
        reg->regNext[inst.rd].busy = true;
        reg->regNext[inst.rd].entry = unit.entry;
    }
    if (inst.tp == type::B_TYPE) {
        cdb->pcFrozen = true;
        if (predictor->getPredict(reg->pcReg) == jump) {
            reg->nextPCReg += sext(inst.imm, 13);
            unit.value = reg->nextPCReg;
        } else {
            reg->nextPCReg += 4;
            unit.value = reg->nextPCReg;
        }
    }
    if (inst.op == opcode::lui) {
        unit.value = sext(inst.imm, 32);
        reg->regNext[inst.rd].busy = true;
        reg->regNext[inst.rd].entry = unit.entry;
        unit.state = WRITE;
        cdb->write(unit.value, unit.entry);
        unit.state = WRITE;
    }
    if (inst.op == opcode::jalr || inst.op == opcode::jal) {
        unit.value = reg->pcReg + 4;
        cdb->write(reg->pcReg + 4, unit.entry);
        reg->regNext[inst.rd].busy = true;
        reg->regNext[inst.rd].entry = unit.entry;
    }
    if (inst.op == opcode::jalr) {
        cdb->jalrPanic = true;
    }
    if (inst.op == opcode::jal) {
        cdb->pcFrozen = true;
        reg->nextPCReg += sext(inst.imm, 21);
        cdb->write(reg->pcReg + sext(inst.imm, 21), unit.entry, true);
        unit.state = WRITE;
    }
    queueNext.enqueue(unit);
}

bool Rob::isFull() {
    return queue.isFull();
}