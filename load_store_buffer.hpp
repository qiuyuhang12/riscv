//
// Created by qiuyuhang on 24-7-29.
//

#ifndef CODE_LOAD_STORE_BUFFER_HPP
#define CODE_LOAD_STORE_BUFFER_HPP

#include "utility.hpp"
#include "decode.hpp"

#include "register.hpp"
#include "memory.hpp"
#include "common_data_bus.hpp"

class Reg;

class Rob;

//class Memory;
template<typename T>
class CircularQueue;

class Lsb {
public:
    Reg *reg;
    Rob *rob;
    Memory *memory;
    Cdb *cdb;

    struct unit {
        bool busy = false;
        opcode op;
        int reg = -1;//寄存器值（S要存的内容）
        int mem = -1;//LS地址值
        int regEntry = -1;//在s操作，保存值的寄存器忙时的entry，可从广播得到值
        int memEntry = -1;//本指令的rob entry，可从广播得到内存地址值
        int dest = -1;//L操作写入的寄存器编号
        int value = -1;//L操作，已经从内存读出来的值
        int time = 0;
        int entry = -1;//本指令的rob entry，不修改！
        instruction inst;

        void print() {
            cout << "busy:" << busy << " op:" << op << " reg:" << reg << " mem:" << mem << " regEntry:" << regEntry
                 << " memEntry:" << memEntry << " dest:" << dest << " value:" << value << " time:" << time << endl;
            inst.print();
        }

        void init() {
            busy = false;
            op = opcode::undefined;
            reg = -1;
            mem = -1;
            regEntry = -1;
            memEntry = -1;
            dest = -1;
            value = -1;
            time = 0;
        }

        bool operator==(const unit &rhs) const {
            return busy == rhs.busy && op == rhs.op && reg == rhs.reg && mem == rhs.mem && regEntry == rhs.regEntry &&
                   memEntry == rhs.memEntry && dest == rhs.dest && value == rhs.value && time == rhs.time &&
                   entry == rhs.entry && inst == rhs.inst;
        }
    };

//    unit lsb[capacity];
    CircularQueue<unit> lsb;
    CircularQueue<unit> lsbNext;

    void query(int index);

    void execute();

    void receiveBroadcast();

public:
    Lsb();

    Lsb(Reg *reg, Rob *rob, Memory *memory, Cdb *cdb);

    void init(Reg *reg, Rob *rob, Memory *memory, Cdb *cdb);

    void flush();

    bool isFull();

    void issue(instruction inst, int entry);

    //供rob调用,启动一个S指令
    void commit(int entry);

    //供simulator调用
    void step();

    bool ready();

    void clear() {
        lsbNext.clear();
        lsb.clear();
    }
};

#endif //CODE_LOAD_STORE_BUFFER_HPP
