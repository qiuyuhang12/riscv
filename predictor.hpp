//
// Created by qiuyuhang on 24-7-28.
//

#ifndef CODE_PREDICTOR_HPP
#define CODE_PREDICTOR_HPP
#include "utility.hpp"
enum predict{
    jump,
    noJump,
};
class Predictor{
public:
    static const int capacity=PredictorCapacity;
    int result[capacity]={0};
    int shoot=0;
    int got=0;
//    int pc[capacity]={0};
    int size=0;
public:
    Predictor(){
        for (int i = 0; i < capacity; ++i) {
            result[i]=0;
        }
    }
    ~Predictor(){
#ifdef showShoot
        std::cout<<"shoot: "<<shoot<<"  got: "<<got<<"  rate: "<<(double)got/shoot<<std::endl;
#endif
    }
    predict getPredict(int _pc);
    void update(int _pc,bool correctness) ;
};
#endif //CODE_PREDICTOR_HPP
