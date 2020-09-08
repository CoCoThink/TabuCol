# TabuCol

## Introduction

A C++ OOP implementation of TabuCol Graph Coloring Algorithm which is the first application of Tabu Search to graph coloring.

## Literature

[1] Hertz, A., & de Werra, D. (1987). Using tabu search techniques for graph coloring. *Computing*, *39*(4), 345-351. 

## Implementation Abstraction

### Class

#### UndiGraph

- Store Graph with Adjacent List.
- Provide a public method to create graph from col file.

#### TabuCol

- Search Solution with a certain K——Number of Colors.
- Find Neighborhood solution and choose one with findMove method
- Apply the best move with makeMove method
- The iteration process stops when iteration time reaches maxIter

## Class Usage

```C++
int main() {
    UndiGraph g;
    //create graph from col file
    int readResult=g.createFromCol("./data/DSJC500.5.col");
    if(readResult!=0){
    	cout<<"file read error"<<endl;
    	return 1;
	}
    //create tabucol algorithm, params : graph ,color num, maxIter
    TabuCol t(g, 49 ,100000000);
    t.tabuSearch();
    return 0;
}
```

1. Define UndiGraph object
2. Call createFromCol(), the only parameter is path to col file
3. Define TabuCol object with 3 parameters: Graph object, ColorNumK, Max iteration time
4. Call tabuSearch()



## Test Data



| Data Set  | Keys     | Program Result | Number of Iterations |
| --------- | -------- | -------------- | -------------------- |
| DSJC125.1 | 5        | 5              | 4103                 |
| DSJC125.5 | 17       | 17             | 87068                |
| DSJC125.9 | 44       | 44             | 11393                |
| DSJC250.1 | 8        | 8              | 22497                |
| DSJC250.5 | 28       | 28             | 9111442              |
| DSJC250.9 | 72       | 72             | 1815284              |
| DSJC500.1 | 12       | 12             | 105484402            |
| DSJC500.5 | 49,48,47 | 49             | 60805687             |



## Summary

On datasets with less than 500 vertexes, the algorithm can output the result in acceptable time, while on datasets with equal or greater than 500 vertexes, it takes a long time to iterate.