//
// Created by qiuyuhang on 24-7-30.
//
#include "reorder_buffer.hpp"
#include "utility.hpp"
#include "interpreter.hpp"

extern Interpreter interpreter;

void Rob::check() {
    interpreter.work1step();
    bool flag= false;
    for (int i = 1; i < 32; ++i) {
//        assert(interpreter.reg[i] == reg->regNext[i].value);
        if (interpreter.reg[i] != reg->regNext[i].value) {
            cerr <<"reg: "<< i << "  your:" << reg->regNext[i].value << "   truth:" << interpreter.reg[i] << endl;
            flag= true;
        }
    }
    for (int i = 0; i < MemoryCapacity; ++i) {
        if (interpreter.memory[i] != memory->memory[i]) {
            cerr <<"mem: "<< i << "  your:" << memory->memory[i] << "   truth:" << interpreter.memory[i] << endl;
            flag= true;
        }
//        assert(interpreter.memory[i] == memory->memory[i]);
    }
    if (flag){
        assert(0);
    }
}

void Rob::Unit::print() {
    cout << "entry:" << entry << " busy:" << busy << " state:" << state << " dest:" << dest << " value:" << value
         << " pc:" << pc << endl;
    inst.print();
}

void Rob::commit() {
    Unit tmp = queue.front();
#ifdef debug
    cout << "Rob commit pc:" << std::hex << tmp.pc << endl;
#endif
    if (tmp.inst.op == opcode::end) {
        cout << (reg->reg[10].value & 0xff);
        exit(0);
    }
    assert(tmp.state == WRITE);
    if (tmp.inst.originalOp == 3 || tmp.inst.tp == type::S_TYPE) {//LOAD,不进入switch
        if (!memory->working && lsb->ready()) {
            queueNext.front().state = COMMIT;
            lsb->commit();
        } else {
#ifdef debug
            cout << "commit fail" << endl;
#endif
        }
        return;
    }
    switch (tmp.inst.tp) {
        case U_TYPE:
        case type::J_TYPE:
        case type::R_TYPE:
        case type::I_TYPE:
            reg->regNext[tmp.dest].value = tmp.value;
            reg->regNext[tmp.dest].busy = false;
            reg->regNext[tmp.dest].entry = -1;
            queueNext.front().state = COMMIT;
            if (tmp.inst.op != jal)cdb->erase(queue.frontIndex);
            queueNext.dequeue();
            break;
        case type::B_TYPE:
            queueNext.dequeue();
            break;
        default:
            assert(0);
    }
    check();
}

void Rob::wrongPredicted(int entry, int pc) {
    cdb->pcFrozen = true;
    queueNext.cut(entry);
}

void Rob::receiveBroadcast() {
    for (int i = queue.frontIndex; i <= queue.rearIndex; i++) {
        if (queue[i].state == ISSUE) {
            if (queue[i].inst.tp == type::B_TYPE) {
                auto tmp = cdb->getBr(queue[i].entry);
                if (queue[i].value != tmp.second.second) {
                    wrongPredicted(i, tmp.second.first);
                    predictor->update(queue[i].inst.pc, false);
                } else {
                    predictor->update(queue[i].inst.pc, true);
                }
                continue;
            }
//                if (queue[i].inst.op==opcode::jal||queue[i].inst.op==opcode::jalr){
//                    queueNext[i].pc=pc;
//                    queueNext[i].state=WRITE;
//                    return;
//                }
            auto tmp = cdb->get(queue[i].entry);
            if (tmp.first) {
                if (queue[i].inst.op == opcode::jalr) {
//                    cdb->stop= false;
                    reg->nextPCReg = tmp.second;
                    cdb->jalrPanic = false;
                    queueNext[i].state = WRITE;
                    continue;
                }
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
        if (queue[i]==queueNext[i]){
            continue;
        }
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
        check();
        queueNext.dequeue();
        if (tmp.inst.tp != S_TYPE)cdb->erase(queue.frontIndex);//只有S指令才不广播
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
    cout << "Rob issue pc:" << std::hex << reg->pcReg << endl;
#endif
#ifdef detail
    cout << "Rob issue pc:" << std::hex << reg->pcReg << endl;
    inst.print();
    unit.pc = reg->pcReg;
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
    if (inst.rd != -1) {
        reg->regNext[inst.rd].busy = true;
        reg->regNext[inst.rd].entry = unit.entry;
    }
    if (inst.tp == type::B_TYPE) {
        cdb->pcFrozen = true;
        if (predictor->getPredict(reg->pcReg) == jump) {
            reg->nextPCReg += sext(inst.imm, 13);
        } else {
            reg->nextPCReg += 4;
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
        reg->regNext[inst.rd].busy = true;
        reg->regNext[inst.rd].entry = unit.entry;
    }
    if (inst.op == opcode::jalr) {
        cdb->jalrPanic = true;
    }
    if (inst.op == opcode::jal) {
        cdb->pcFrozen = true;
        reg->nextPCReg += sext(inst.imm, 21);
        unit.state = WRITE;
    }
    queueNext.enqueue(unit);
}

bool Rob::isFull() {
    return queue.isFull();
}