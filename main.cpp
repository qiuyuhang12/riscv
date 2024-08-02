int clo=0;
#include "interpreter.hpp"
Interpreter interpreter;


#include "simulator.hpp"
#include <cassert>
int main() {
#ifdef fileread
    freopen(filePath.c_str(),"r",stdin);
#endif
//    while (1){
//        interpreter.work1step();
//    }
    Simulator simulator;
    simulator.work();
    return 0;

    return 0;
}