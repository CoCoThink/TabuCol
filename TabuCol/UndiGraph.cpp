//
// Created by Guan on 2020/9/14.
//

#include "UndiGraph.h"
#include <fstream>

using std::ifstream;

//split string function
vector<string> split(const string &str, const string &pattern)
{
    vector<string> res;
    if(str == "")
        return res;
    string strs = str + pattern;
    size_t pos = strs.find(pattern);

    while(pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        res.push_back(temp);
        strs = strs.substr(pos+1, strs.size());
        pos = strs.find(pattern);
    }
    return res;
}


int UndiGraph::createFromCol(string filePath) {
    ifstream input(filePath);
    string line;
    if(input.is_open()){
        //find the line of p
        while(!input.eof())
        {
            getline(input, line);
            if(line[0]!='p'){
                continue;
            }else{
                this->vertexNum=stoi(split(line," ")[2]);
                edge=new vector<int>[this->vertexNum];
                break;
            }
        }
        //read edge
        while(!input.eof())
        {
            getline(input, line);
            vector<string> strs=split(line," ");
            if (line.find("e", 0) != string::npos){
                int v1=stoi(strs[1])-1;
                int v2=stoi(strs[2])-1;
                edge[v1].push_back(v2);
                edge[v2].push_back(v1);
            }
        }
        return 0;
    }else{
        return -1;
    }
}