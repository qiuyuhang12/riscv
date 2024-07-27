#include "decode.hpp"
#include "memory.hpp"
#include "interpreter.hpp"
#include "register.hpp"

int main() {
//    loadCode();
    cinCode();
    int clock = 0;
    while (pc < 1 << 20) {

//    while (pc<1<<20&&clock<1000){
        ++clock;
        assert(pc % 4 == 0);
        assert(get8() == 0x0ff00513);
        step();
//        if (clock%1000==0)
//        cout << clock << "  " << hex << pc << dec << endl;
        instruction ins = decode(iR);
        interpreter(ins);
        if (ins.tp == B_TYPE || ins.op == jal || ins.op == jalr)continue;
        addPC();
    }
    return 0;
}