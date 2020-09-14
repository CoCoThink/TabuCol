//
// Created by Guan on 2020/9/14.
//

#include "TabuCol.h"
#include <iostream>
#include <climits>
#include <cstring>

using std::cout;
using std::endl;

//init
TabuCol::TabuCol(UndiGraph graph,int colorNum,int maxIterationNum) {
    this->colorNumK=colorNum;
    this->g=graph;
    this->maxIter=maxIterationNum;
    f=0;
    //allocate memory
    sol=new int[g.vertexNum];
//    bestSol=new int[g.vertexNum];
    tabuTenure=new int*[g.vertexNum];
    adjColTab=new int*[g.vertexNum];
    for(int i=0;i<g.vertexNum;++i){
        adjColTab[i]=new int[colorNumK];
        tabuTenure[i]=new int[colorNumK];
        memset(tabuTenure[i], 0, sizeof(int)*colorNumK);
        memset(adjColTab[i], 0, sizeof(int)*colorNumK);
    }

    //select a solution randomly
    for(int i=0;i<g.vertexNum;++i){
        sol[i]=rand()%colorNum;
    }

    //init Adjacent_Color_Table & f
    int color_i;
    int color_j;
    for(int i=0;i<g.vertexNum;++i){
        color_i=sol[i];
        for(int j=0;j<g.edge[i].size();++j){
            color_j=sol[g.edge[i][j]];
            if(color_i==color_j){
                ++f;
            }
            ++adjColTab[i][color_j];
        }
    }
    f/=2;
    bestF=f;
}

void TabuCol::tabuSearch() {
    iter=0;
    while(iter<maxIter){
        ++iter;
        findMove();
        makeMove();
        if(f==0){
            cout<<"Success"<<endl;
            //output result
//            for(int i=0;i<g.vertexNum;++i){
//                cout<<i+1<<' '<<sol[i]<<endl;
//            }
            //verify
            for(int i=0;i<g.vertexNum;++i){
                for(int j=0;j<g.edge[i].size();++j){
                    if(sol[i]==sol[g.edge[i][j]]){
                        cout<<"Error"<<endl;
                    }
                }
            }
            //output Number of Iterations
            cout<<"Number of Iterations: "<<iter<<endl;
            return;
        }
    }
    cout<<"Not Found"<<endl;
}

void TabuCol::findMove() {
    struct move{
        int vertex;
        int color;
    };
    //There may be some moves with the same deltaF.
    vector<struct move> tabuMoveList,nontabuMoveList;
    int tabuMinDeltaF=INT_MAX;
    int nontabuMinDeltaF=INT_MAX;
    int tmpDeltaF;
    for(int i=0;i<g.vertexNum;++i){
        if(adjColTab[i][sol[i]] > 0){
            for(int k=0;k<colorNumK;++k){
                if(k!=sol[i]){
                    tmpDeltaF=adjColTab[i][k]-adjColTab[i][sol[i]];
                    if(tabuTenure[i][k] < iter){ //< or <= ?
                        //update the non-tabu best move
                        if(tmpDeltaF <= nontabuMinDeltaF){
                            if(tmpDeltaF < nontabuMinDeltaF){
                                nontabuMoveList.clear();
                                nontabuMinDeltaF=tmpDeltaF;
                            }
                            struct move tmp={i,k};
                            nontabuMoveList.push_back(tmp);
                        }
                    }else{
                        //update the tabu best move
                        if(tmpDeltaF <= tabuMinDeltaF){
                            if(tmpDeltaF < tabuMinDeltaF){
                                tabuMoveList.clear();
                                tabuMinDeltaF=tmpDeltaF;
                            }
                            struct move tmp={i,k};
                            tabuMoveList.push_back(tmp);
                        }
                    }
                }
            }
        }
    }
//    Aspiration criterion: it holds if both the following two conditions satisfy:
//    1. the best tabu move is better than the previous best known solution;
//    2. the best tabu move is better than the best non-tabu move in the current neighborhood.
    if((f+tabuMinDeltaF)<bestF && tabuMinDeltaF < nontabuMinDeltaF){
        nextMove.deltaF=tabuMinDeltaF;
        //select a move randomly
        int randIndex= rand() % tabuMoveList.size();
        nextMove.vertex=tabuMoveList[randIndex].vertex;
        nextMove.oriCol=sol[tabuMoveList[randIndex].vertex];
        nextMove.nextCol=tabuMoveList[randIndex].color;
    }else{
        nextMove.deltaF=nontabuMinDeltaF;
        int randIndex= rand() % nontabuMoveList.size();
        nextMove.vertex = nontabuMoveList[randIndex].vertex;
        nextMove.oriCol=sol[nextMove.vertex];
        nextMove.nextCol=nontabuMoveList[randIndex].color;
    }
}

void TabuCol::makeMove() {
    //update solution ,f and tabuTenure
    sol[nextMove.vertex]=nextMove.nextCol;
    f = f + nextMove.deltaF;
    if(f < bestF){
        bestF = f;
    }
    tabuTenure[nextMove.vertex][nextMove.oriCol]=iter+f+rand()%10;
    //Update the Adjacent_Color_Table;
    for(int i=0;i<g.edge[nextMove.vertex].size();++i){
        --adjColTab[g.edge[nextMove.vertex][i]] [nextMove.oriCol];
        ++adjColTab[g.edge[nextMove.vertex][i]] [nextMove.nextCol];
    }
}
