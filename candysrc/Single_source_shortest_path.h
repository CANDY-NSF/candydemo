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



int singleSourceShortestPath(std::string inputGraphFile,
                             std::string remainderGraph,
                             std::string changedEdges, int totalNodes,
                             int threads) {
    clock_t q, q1, q2, t;
    double startx, endx, starty, endy;


    /***** Preprocessing to Graph (GUI) ***********/
    t = clock();
    q = clock();    //Assume Processed Input

/* Set the number of Threads and Vertices**/
    const char* ig = inputGraphFile.c_str();
    const char* ce = changedEdges.c_str();
    const char* ipr = remainderGraph.c_str();

    int asl=0; //asl denotes the lenght of asynchrony; 0=synchronization at each level
    // number of levels of asynchrony
    /** Set the number of Threads and Vertices**/


    int maxW = 10;
    int nodes = totalNodes; // total number of vertices
    // the number of changes processed per point
    int p = threads;        // total number of threads per core
    RT_Vertex RTV;
    vector<RT_Vertex> SSSP;
    SSSP.clear();
    SSSP.resize(nodes, RTV);

    vector<RT_Vertex> SSSP_Org;


    //List of Changes
    //There will be a list for inserts and a list for delete
    vector<xEdge> allChange;
    allChange.clear();
    /*** Initializing Memory ***/

    int src=0; //the source from which the paths are computed


    /*** Read the SSSP as a Graph ***/
    startx=omp_get_wtime();
    A_Network X;
    A_Network T;
    readin_networkM(&X, &T, nodes, ipr, ig);
    endx=omp_get_wtime();
    printf("Total Time for Reading Whole Network %f \n", endx-startx);
    /*** Finished Reading  **/


    //Create Rooted Tree
    startx=omp_get_wtime();
    create_treeS(&T, &SSSP,src, p);
    endx=omp_get_wtime();
    printf("Total Time for Reading SSSP Tree %f \n", endx-startx);
    SSSP_Org=SSSP;
    /*** Finished Reading  **/

    /*** Read set of Changed Edges ***/
    startx=omp_get_wtime();
    readin_changes(ce, &allChange);
    endx=omp_get_wtime();
    printf("Total Time for Reading Update List %f \n", endx-startx);
    /*** Finished Reading Changed Edges **/


    double maxV=(double)maxW*X.size();


    printf("==Update Starts =======\n");

    starty=omp_get_wtime();

    //Update the inserted and delted edges in the tree
    startx=omp_get_wtime();
    edge_update(&allChange, &X, &SSSP, &maxV, p);
    endx=omp_get_wtime();
    printf("Total Time for Initial Update  %f\n", endx-startx);


    //Update the other vertices based on new inserted edges
    startx=omp_get_wtime();
    rest_update(&X, &SSSP, &maxV, asl, p);
    endx=omp_get_wtime();
    printf("Total Time for Complete Update  %f\n", endx-startx);
    //printRT(&SSSP);

    endy=omp_get_wtime();

    printf("====Total Time %f \n", endy-starty);


    //Counting
    int opt_update=0;
    int total_update=0;
    int tree_update=0;
    for(int i=0;i<nodes;i++)
    {
        total_update=total_update+SSSP[i].num_update;

        if(SSSP[i].num_update>0)
        {
            total_update=total_update+SSSP[i].num_update;}

        if(SSSP_Org[i].Parent!=SSSP[i].Parent)
        { tree_update=tree_update+1;}

        if(SSSP_Org[i].Dist!=SSSP[i].Dist)
        { opt_update=opt_update+1;}
    }

    printf("Optimal Tree Updates %d: Optimal Vertex Updates %d : Total Updates %d \n",tree_update, opt_update, total_update);
    return 0;
} // end of main


#endif // CANDY_SINGLE_SOURCE_SHORTEST_PATH_H
