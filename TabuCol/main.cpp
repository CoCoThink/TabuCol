#include <iostream>
#include"UndiGraph.h"
#include "TabuCol.h"
#include "TabuCol2.h"

using std::cout;
using std::endl;


int main() {
    UndiGraph g;
    //create graph from col file
    int readResult=g.createFromCol("./data/DSJC125.5.col");
    if(readResult!=0){
    	cout<<"file read error"<<endl;
    	return 1;
	}
    //create tabucol algorithm, params : graph ,color num, maxIter
    TabuCol2 t(g, 17 ,100000000);
    t.tabuSearch();
    return 0;
}

