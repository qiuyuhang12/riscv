int clo = 0;

#include "interpreter.hpp"

Interpreter interpreter;


#include "simulator.hpp"
#include <cassert>

int main() {
#ifdef fileread
    freopen(filePath.c_str(),"r",stdin);
#endif
//int timee= time(0);
    Simulator simulator;
    simulator.work();
//    cout<<endl<<time(0)-timee<<endl;
    return 0;
}