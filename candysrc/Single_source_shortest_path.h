//
// Created by sriram on 11/9/22.
//

#ifndef CANDY_SINGLE_SOURCE_SHORTEST_PATH_H
#define CANDY_SINGLE_SOURCE_SHORTEST_PATH_H

#include "../includes/ESSENS/ESSENS/Core/Basic_IO/Format/Input/Level2/input_to_network.hpp"
#include "Ins_Del1.hpp"
#include "SSSP_structure.h"
#include "create_tree.hpp"
#include "readinSSSP_data.h"

#include <pybind11/pybind11.h>
// AUXILLIARY OPS
#include <list>
#include <queue>
#include <stack>
/*** All Headers Required From ESSENS **/

void SSPInitialize(int nodes, int src, const A_Network &X, double &startx,
                   double &endx, vector<RT_Vertex> &SSSP, int p);

using namespace std;

int singleSourceShortestPath(std::string inputGraphFile,
                             std::string remainderGraph,
                             std::string changedEdges, int totalNodes,
                             int threads) {
  clock_t q, q1, q2, t;
  double startx, endx, starty, endy;

  /***** Preprocessing to Graph (GUI) ***********/
  t = clock();
  q = clock(); // Assume Processed Input
  // Form node node weight
  // Nodes are numbered contiguously
  // Starts from zero
  // Edges are undirected (both a-->b and b-->a present)
  // Check if valid input is given
  //    if ( argc < 7 ) { cout << "INPUT ERROR:: EIGHT inputs required. First:
  //    filename without tree. Second: Graph Certificate. Third: List of Edges
  //    to be Updated. Fourth: Upper Bound on Weight Fifth: number of nodes.
  //    Sixth: Size of batch. Seventh:number of threads\n"; return 0;}
  //    //Check to see if file opening succeeded
  //    ifstream the_file ( argv[1] ); if (!the_file.is_open() ) { cout<<"INPUT
  //    ERROR:: Could not open main //file\n";} ifstream the_file1 ( argv[2] );
  //    if (!the_file1.is_open() ) { cout<<"INPUT ERROR:: Could not open SCC
  //    file\n";} ifstream the_file2 ( argv[3] ); if (!the_file2.is_open() ) {
  //    cout<<"INPUT ERROR:: Could not open update file\n";}

  /* Set the number of Threads and Vertices**/
  const char *ig = inputGraphFile.c_str();
  const char *ce = changedEdges.c_str();
  const char *ipr = remainderGraph.c_str();
  int maxW = 10;
  int nodes = totalNodes; // total number of vertices
  int batch = 2;          // the number of changes processed per point
  int p = threads;        // total number of threads per core
  int graphDirectedUndirectedIndicator =
      0; // Should be 1 for SCC, 0 for not SCC
         //    ifstream the_file3 ( argv[8] ); if (!the_file3.is_open() ) {
         //    cout<<"INPUT ERROR:: Could not open SCC file\n";}

  /** Set the number of Threads and Vertices**/

  /**   Initializing Memory ***/ /** TBD: Change to Array and Malloc**/
                                 // startx=omp_get_wtime();

  // List of Changes
  // There will be a list for inserts and a list for delete
  vector<xEdge> allChange;
  allChange.clear();
  /*** Initializing Memory ***/

  /*** Read Remainder Edges as Graph ***/
  startx = omp_get_wtime();
  A_Network R;
  // readin_network(&R,  argv[1], -1);
  readin_graphU(&R, nodes, ig);
  endx = omp_get_wtime();
  printf("Total Time for Reading Remainder Network %f \n", endx - startx);
  /*** Finished Reading CRT Tree **/

  /*** Read the Certificate ***/
  startx = omp_get_wtime();
  A_Network X;
  readin_network(&X, ipr, -1);
  // readin_graphU(&X, nodes,argv[1]);
  endx = omp_get_wtime();
  py::print("Total Time for Reading Certificate Network:", endx - startx);
  /*** Finished Reading CRT Tree **/

  /*** Read set of Changed Edges ***/
  startx = omp_get_wtime();
  readin_changes(ce, &allChange);
  endx = omp_get_wtime();
  py::print("Total Time for Reading Update List:", endx - startx);
  /*** Finished Reading Changed Edges **/

  /** Add Buffer for Extra Nodes ***/

  // Initializing  Rooted Tree
  RT_Vertex RTV;
  vector<RT_Vertex> SSSP;
  vector<SCC_vertex> SCC;
  if (graphDirectedUndirectedIndicator == 0) {
    int src = 0; // the source from which the paths are computed

    SSSP.clear();
    SSSP.resize(nodes, RTV);
    SSPInitialize(nodes, src, X, startx, endx, SSSP, p);
    printf("Total Time for Computing degree and Updating SSSSP %f \n",
           endx - startx);
    printf("==Update Starts =======\n");
    startx = omp_get_wtime();
    create_treeS(&X, &R, &SSSP, src, p);
    endx = omp_get_wtime();
    printf("Total Time for Creating tree %f \n", endx - startx);

    double maxV = (double)maxW * X.size();

    starty = omp_get_wtime();
    // Update the inserted and delted edges in the tree
    startx = omp_get_wtime();
    int te = 0;
    edge_update(&allChange, &X, &SSSP, &R, &maxV, &te, p);
    endx = omp_get_wtime();
    py::print("Total Time for Initial Update:", endx - startx);

    startx = omp_get_wtime();
    rest_update(&X, &SSSP, &R, &maxV, &te, p);
    endx = omp_get_wtime();
    py::print("Total Time for Complete Update:", endx - startx);

    endy = omp_get_wtime();
    py::print("Total Time for Initia+Complete Update:", endy - starty, te);
  }

  return 0;
}

void SSPInitialize(int nodes, int src, const A_Network &X, double &startx,
                   double &endx, vector<RT_Vertex> &SSSP, int p) {
  startx = omp_get_wtime();
#pragma omp parallel for num_threads(p)
  for (int i = 0; i < nodes; i++) {

    if (i == src) {
      SSSP[i].Root = -1;
    } else {
      SSSP[i].Root = i;
    }
    SSSP[i].Level = 0;
    SSSP[i].Dist = 0.0;
    SSSP[i].degree = int(X[i].ListW.size());
  }
  endx = omp_get_wtime();
} // end of main

#endif // CANDY_SINGLE_SOURCE_SHORTEST_PATH_H
