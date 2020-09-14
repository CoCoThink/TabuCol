//
// Created by Guan on 2020/9/14.
//

#ifndef TABUCOL_TABUCOL_H
#define TABUCOL_TABUCOL_H

#include "UndiGraph.h"
class TabuCol{
public:
    TabuCol(UndiGraph g,int colorNum,int maxIterationNum);
    void init();
    //solution
    int* sol;
    //Evaluation Function=the num of conflicting edges
    int f;
    //for Aspiration
//    int* bestSol;
    int bestF;
    void tabuSearch();
    void setColrNum(int k){
        this->colorNumK=k;
    };
private:
    int colorNumK;
    UndiGraph g;
    //Number of Iterations
    int iter;
    //maximum number of iterations
    int maxIter;
    //Adjacent_Color_Table
    int** adjColTab;
    int** tabuTenure;
    struct Move{
        int vertex;
        //original color
        int oriCol;
        //next color
        int nextCol;
        int deltaF;
    }nextMove;
    void findMove();
    void makeMove();
};

#endif //TABUCOL_TABUCOL_H
