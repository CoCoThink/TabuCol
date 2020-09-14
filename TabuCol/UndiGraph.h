//
// Created by Guan on 2020/9/14.
//

#ifndef TABUCOL_UNDIGRAPH_H
#define TABUCOL_UNDIGRAPH_H

#include <vector>
#include <string>
using std::vector;
using std::string;

class UndiGraph{
public:
    int vertexNum;
    vector<int>* edge;
    //return -1 if file open error
    //return -2 if graph creating error
    //return 0 if success
    int createFromCol(string filePath);
};

#endif //TABUCOL_UNDIGRAPH_H
