#include "utility.hpp"
#include "load_store_buffer.hpp"
#include "register.hpp"

void Lsb::query(int index) {
    if (lsb[index].regEntry != -1) {
        auto tmp = cdb->get(lsb[index].regEntry);
        if (tmp.first) {
            lsbNext[index].reg = tmp.second;
            lsbNext[index].regEntry = -1;
        }
    }
    if (lsb[index].memEntry != -1) {
        auto tmp = cdb->get(lsb[index].memEntry, true);
        if (tmp.first) {
            lsbNext[index].mem = tmp.second;
            lsbNext[index].memEntry = -1;
        }
    }
}

void Lsb::receiveBroadcast() {
//    for (int i = lsb.frontIndex; i <= lsb.rearIndex; i++) {
    for (int i = 0; i < capacity; i++) {
        if (!lsb.isInQueue(i)) {
            continue;
        }
        if (lsb[i].regEntry != -1) {
            auto tmp = cdb->get(lsb[i].regEntry);
            if (tmp.first) {
#ifdef debug
                cout << "lsb receiveBroadcast pc : " << lsb[i].inst.pc << "  entry: " << lsb[i].entry
                     << "   lsb reg(要存的值) : " << tmp.second << "    reg 依赖： " << lsb[i].regEntry << endl;
#endif
                lsbNext[i].reg = tmp.second;
                lsbNext[i].regEntry = -1;
            }
        }
        if (lsb[i].memEntry != -1) {
            auto tmp = cdb->get(lsb[i].memEntry, true);
            if (tmp.first) {
#ifdef debug
                cout << "lsb receiveBroadcast pc : " << lsb[i].inst.pc << "  entry: " << lsb[i].entry
                     << "   lsb mem(地址) : " << tmp.second << endl;
#endif
                lsbNext[i].mem = tmp.second;
                lsbNext[i].memEntry = -1;
            }
        }
    }
}

void Lsb::execute() {
#ifdef debug
    cout << "lsb execute pc : " << lsb.front().inst.pc << "  entry: " << lsb.front().entry << endl;
    cout << "lsb mem : " << lsb.front().mem << endl;
//    lsb.front().print();
#endif
    assert(!lsb.isEmpty());
    int loadValue = -1;
    switch (lsb.front().op) {
        case sb:
            memory->store(lsb.front().mem, lsb.front().reg, 1);
            break;
        case sh:
            memory->store(lsb.front().mem, lsb.front().reg, 2);
            break;
        case sw:
            memory->store(lsb.front().mem, lsb.front().reg, 4);
            break;
        case lb:
            loadValue = memory->load(lsb.front().mem, 1);
//            lsbNext.front().value = memory->load(lsb.front().mem, 1);
//                cdb->commit(memory->load(lsb.front().mem, 4), lsb.front().dest);
            break;
        case lh:
            loadValue = memory->load(lsb.front().mem, 2);
//            lsbNext.front().value = memory->load(lsb.front().mem, 2);
            break;
        case lw:
            loadValue = memory->load(lsb.front().mem, 4);
//            lsbNext.front().value = memory->load(lsb.front().mem, 4);
            break;
        case lbu:
            loadValue = memory->loadU(lsb.front().mem, 1);
//            lsbNext.front().value = memory->loadU(lsb.front().mem, 1);
            break;
        case lhu:
            loadValue = memory->loadU(lsb.front().mem, 2);
//            lsbNext.front().value = memory->loadU(lsb.front().mem, 2);
            break;
        default:
            assert(0);//undefined opcode
    }
    if (lsb.front().inst.tp == I_TYPE) {
//        cout<<"load: "<<loadValue<<endl;
        lsbNext.front().value = loadValue;
        cdb->write(loadValue, lsb.front().entry);
        if (reg->regNext[lsb.front().dest].entry == lsb.front().entry) {
            reg->regNext[lsb.front().dest].busy = false;
            reg->regNext[lsb.front().dest].entry = -1;
        }
        reg->regNext[lsb.front().dest].value = loadValue;
    }
}


Lsb::Lsb() {}

Lsb::Lsb(Reg *reg, Rob *rob, Memory *memory, Cdb *cdb) : reg(reg), rob(rob), memory(memory), cdb(cdb) {}

void Lsb::init(Reg *reg, Rob *rob, Memory *memory, Cdb *cdb) {
    this->reg = reg;
    this->rob = rob;
    this->memory = memory;
    this->cdb = cdb;
}

void Lsb::flush() {
#ifdef detail
    cout << "lsb flush" << endl;
    for (int i = 0; i < capacity; ++i) {
        if (lsb[i] == lsbNext[i]) {
            continue;
        }
        lsb[i].print();
        lsbNext[i].print();
    }
#endif
    lsb = lsbNext;
}

bool Lsb::isFull() {
    return lsb.isFull();
}

void Lsb::issue(instruction inst, int entry) {
#ifdef debug
    cout << "Lsb issue pc: " << reg->pcReg << "  entry: " << entry << endl;
#endif

#ifdef detail
    inst.print();
#endif
    unit tmp;
    tmp.init();
    tmp.inst = inst;
    tmp.busy = true;
    tmp.op = inst.op;
    tmp.entry = entry;
    tmp.memEntry = entry;
    switch (inst.tp) {
        case I_TYPE:
            tmp.dest = inst.rd;
            break;
        case S_TYPE:
            if (reg->regNext[inst.rs2].busy) {
                auto tmp0 = cdb->get(reg->regNext[inst.rs2].entry);
                if (tmp0.first) {
                    tmp.reg = tmp0.second;
                } else {
                    tmp.regEntry = reg->regNext[inst.rs2].entry;
                }
            } else {
                tmp.reg = reg->regNext[inst.rs2].value;
            }
            break;
        default:
            assert(0);
    }
    if (!lsbNext.enqueue(tmp)) {
        assert(0);//lsb is full
    }
}


void Lsb::commit(int entry) {
    if (entry != lsb.front().entry) {
        cerr << hex << "entry: " << entry << "  lsb_entry: " << lsb.front().entry << "  lsb_pc: " << lsb.front().inst.pc
             << endl;
        assert(0);
    }
    if (!memory->working) {
#ifdef debug
        cout << "lsb commit pc : " << lsb.front().inst.pc << " entry: " << lsb.front().entry << endl;
//        lsb.front().print();
#endif
        assert(lsb.front().memEntry == -1 && lsb.front().regEntry == -1);
        memory->startWork();
        lsbNext.front().time = 1;
    }
}

void Lsb::step() {
    if (lsb.isEmpty()) {
        return;
    }
    receiveBroadcast();
    if (lsb.front().time > 0) {//等待memory完成
        lsbNext.front().time++;
        if (!memory->working) {
            execute();
            lsbNext.dequeue();
        }
    }
}

bool Lsb::ready() {
    return lsb.front().memEntry == -1 && lsb.front().regEntry == -1;
}