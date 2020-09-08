#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <cstring>

using std::ifstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::stoi;

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

class UndiGraph{
public:
    int vertexNum;
    vector<int>* edge;
    //return -1 if file open error
    //return -2 if graph creating error
    //return 0 if success
    int createFromCol(string filePath);
};

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

int main() {
    UndiGraph g;
    //create graph from col file
    int readResult=g.createFromCol("./data/DSJC125.9.col");
    if(readResult!=0){
    	cout<<"file read error"<<endl;
    	return 1;
	}
    //create tabucol algorithm, params : graph ,color num, maxIter
    TabuCol t(g, 44 ,100000000);
    t.tabuSearch();
    return 0;
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
