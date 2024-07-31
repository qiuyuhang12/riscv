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
    int pc[capacity]={0};
    int size=0;
public:
    predict getPredict(int _pc);
    void update(int _pc,bool correctness) ;
};
#endif //CODE_PREDICTOR_HPP
