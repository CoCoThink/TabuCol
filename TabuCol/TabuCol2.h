//
// Created by Guan on 2020/9/14.
//

#ifndef TABUCOL_TABUCOL2_H
#define TABUCOL_TABUCOL2_H

#include "UndiGraph.h"


#define LNOISE_INIT 10

class TabuCol2{
public:
    TabuCol2(UndiGraph g,int colorNum,int maxIterationNum);
    void init();
    //solution, Assignment Representation
    int* sol;
    //vertex number in Grouping Representation, groupVertexNum[i] means the number of vertexes with color i
    int* groupVertexNum;
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
    //the tabuTenure is changed to 1-d array,
    //0 represents non-tabu, a positive integer n means color n is tabu
    int* tabuTenure;
    //record the last iter of a move,
    //moveLastIter[i][j]=n means the last iter is n when changing vertex i to color j. init with 0.
    int** moveLastIter;
    struct Move{
        int vertex;
        //original color
        int oriCol;
        //next color
        int nextCol;
        int deltaF;
    }nextMove;
    void findMove();
    void setRandMove();
    //probability(0-100)
    //the probability of accepting one conflict zero move
    int ZNoise=20;
    //the probability of move at random
    int LNoise=LNOISE_INIT;
    //the probability of accepting SECOND_BEST_MOVE
    int SNoise=10;
    //the number of iterations that deltaF >=0, which is used to control LNoise
    int nonNegativeDeltaIter=0;
    void makeMove();
};

#endif //TABUCOL_TABUCOL2_H
