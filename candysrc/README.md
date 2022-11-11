# AdaptivePageRank

ESSENS is required to run this implmentation, Please download ESSENS from (https://github.com/CANDY-NSF/ESSENS) and update the path in Makefile.

## Compile command
make all
make cE

## Run Input file

./pr test_graph.txt iniPR_test_graph ce_TestGraph1 20 72 100 true

Input: 
  - Original graph input file
  - Initial PR for graph file
  - Changed Edges
  - Number of nodes in graph
  - Number of Threads
  - Number of Iterations to ocnverge
  - boolean flag for ETC ( could have values only true/false)
  
### Generate Changed Edges
  ./cE.out  test1 1000 50
 Input:
 - Original graph input file
 - Number of changed edges to generate
 - % of edge insertion
 
