//
// Created by sriram on 11/12/22.
//

#ifndef CANDY_READINSSSP_DATA_H
#define CANDY_READINSSSP_DATA_H

/*** All Headers Required From ESSENS **/
// INPUT HEADERS
// #include
// "../includes/ESSENS/ESSENS/Core/Basic_IO/Format/Input/Level2/input_to_network.hpp""
// #include<set>
// #include<map>
// #include
// "../includes/ESSENS/ESSENS/Core/Basic_IO/Format/Input/Level1/ADJ/create_networkP.hpp"
//
//
////OUTPUT HEADERS
////#include
///"../includes/ESSENS/ESSENS/Core/Basic_IO/Format/Output/Level2/printout_network.hpp"
//
////#include "printout_others.hpp"
//
////SET_OPERATIONS
// #include
// "../includes/ESSENS/ESSENS/Core/Basic_SetOps/Level2/unary_set_operations.hpp"
//
// #include
// "../includes/ESSENS/ESSENS/Core/Basic_SetOps/Level2/binary_set_operations.hpp"
//
////NETWORK TRAVERSAL
// #include
// "../includes/ESSENS/ESSENS/Core/Basic_Traversal/Level2/ADJ/traversal.hpp"
//
//
////ADD and DELETE EDGE
// #include "../includes/ESSENS/ESSENS/Basic_Change/Level0/ADJ/add_edge.hpp"
// #include "../includes/ESSENS/ESSENS/Basic_Change/Level0/ADJ/del_edge.hpp"
#include "SSSP_structure.h"
#include <omp.h>

// #include <concurrent_vector.h>
using namespace std;

#include "SP_structure.hpp"
// #include "minimum_spanTX.hpp"
using namespace std;

void readin_SCC(const char *file, vector<SCC_vertex> *SCC) {

  FILE *graph_file;
  char line[128];
  SCC_vertex myedge;

  graph_file = fopen(file, "r");
  while (fgets(line, 128, graph_file) != NULL) {
    sscanf(line, "%d %d ", &myedge.node, &myedge.sccID);
    SCC->push_back(myedge);
  }
  fclose(graph_file);
}

void readin_changes(const char *myfile, vector<xEdge> *allChange) {
  // File reading parameters
  FILE *graph_file;
  char line[128];
  xEdge myedge;
  int ins;

  graph_file = fopen(myfile, "r");
  while (fgets(line, 128, graph_file) != NULL) {
    // Read line
    sscanf(line, "%d %d %lf %d", &myedge.theEdge.node1, &myedge.theEdge.node2,
           &myedge.theEdge.edge_wt, &ins);
    if (ins == 1) {
      myedge.inst = true;
    } else {
      myedge.inst = false;
    }

    // myedge.theEdge.edge_wt=((int)(ceil(myedge.theEdge.edge_wt))%max_val+1);
    allChange->push_back(myedge);

  } // end of while
  fclose(graph_file);

  return;
} // end of function

void readin_remainder(A_Network *R, int highV, vector<Edge> *reBs) {
  for (int i = 0; i < R->size(); i++) {
    for (int j = 0; j < R->at(i).ListW.size(); j++) {
      int_double myval;
      myval = R->at(i).ListW[j];

      if (i > myval.first) {
        continue;
      }
      if (myval.second > double(highV)) {
        continue;
      }

      Edge myedge;
      myedge.node1 = i;
      myedge.node2 = myval.first;
      myedge.edge_wt = myval.second;

      reBs->push_back(myedge);

    } // end of for
  }   // end of for

  return;
} // end of function*/

void readin_remainder(const char *myfile, int highV, vector<Edge> *reBs) {
  // File reading parameters
  FILE *graph_file;
  char line[128];
  Edge myedge;
  int ins;

  graph_file = fopen(myfile, "r");
  while (fgets(line, 128, graph_file) != NULL) {
    // Read line
    sscanf(line, "%d %d %lf", &myedge.node1, &myedge.node2, &myedge.edge_wt);

    // if(SSSP->at(myedge.node1).Dist > myedge.

    if (myedge.node1 > myedge.node2) {
      continue;
    }
    if (myedge.edge_wt > double(highV)) {
      continue;
    }

    // printf("%d %d ===\n", myedge.node1, myedge.node2);
    reBs->push_back(myedge);

  } // end of while
  fclose(graph_file);

  return;
} // end of function*/

// Assumes the all nodes present
// Node starts from 0
// Total number of vertices=nodes and are consecutively arranged
// reads only the edges in the edge list, does not reverse them to make
// undirected
void readin_graphU(A_Network *X, int nodes, const char *myfile) {

  // Initialize for X
  ADJ_Bundle AList; // create current Adj_Bundle
  X->resize(nodes, AList);
  int_double colvals;

  // File reading parameters
  FILE *graph_file;
  char line[128];
  int node1, node2;
  double edge_wt;

  graph_file = fopen(myfile, "r");
  int current_node = 0;
  vector<int_double> ListW;
  while (fgets(line, 128, graph_file) != NULL) {
    // Read line
    sscanf(line, "%d %d %lf", &node1, &colvals.first, &colvals.second);

    // Initailize and Add Row
    if (current_node != node1) {
      // if(current_node >-1) //Update network after each row
      {
        // Update Rows
        // X->push_back(AList); //Add row and neighbors to network;
        X->at(current_node).ListW = ListW;
        ListW.clear(); // Clear List of Edges;

      } // end of if

      // Set to next node
      current_node = node1;
      // AList.Row=current_node;
    } // end of if

    // Add Columns corresponding to the rows
    // colvals.first=node2;
    // colvals.second=edge_wt;
    ListW.push_back(colvals);

  } // end of while
  fclose(graph_file);
}

#endif // CANDY_READINSSSP_DATA_H
