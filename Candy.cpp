#include <pybind11/pybind11.h>
#include "candysrc//Page_rank.h"
#include "candysrc/Single_source_shortest_path.h"
#include <iostream>
#include <string>

namespace py = pybind11;
void calculatePR(std::string inputGraphFile, std::string changedEdges, std::string initialPageRank, int totalNodes, int threads)
{
//    py::print("Graph name:");
//
//    py::print("Number of Nodes:");
//
//    py::print("Number of Edges:18000");
//
//    py::print("Number of Batches:3");

    py::print("Initiating update process");
    float computeTime= pageRank(inputGraphFile,changedEdges,initialPageRank,totalNodes,threads);
   string Output="Time to Update Page Rank for Batch 1:";
   Output+=std::to_string(computeTime);



}

int add(int i, int j) {
    return i + j;
}

float calculateSSSP(std::string inputGraphFile, std::string changedEdges, std::string remainderGraph, int totalNodes, int threads)
{
    float computeTime= singleSourceShortestPath(inputGraphFile,remainderGraph,changedEdges,totalNodes,threads);
    return computeTime;
}

namespace py = pybind11;

void help()
{

    py::print("Currently CANDY Python wrapper supports updating Page Rank (PR), Single Source Shortest Path (SSSP).");
    py::print("All functions for CANDY requires initial graph file, file with changed edges, initial values of graph property, num of nodes, and number of threads.");
    py::print("For example, if I want to calculate Page Rank, here is how I would call and pass the input parameters candy.pageRank('DirectedGraph', "
            "'changedEdgesPR', 'InitialPR',6,32).\n");



}

void about()
{
py::print("CANDY software is funded by NSF, for more information about our group check our website https://sites.google.com/view/candy-nsf/CANDY");
}


PYBIND11_MODULE(candy, mod) {

    mod.def("pageRank", &calculatePR, "Calculate Page Rank");
    mod.def("singleSourceShortestPath", &calculateSSSP, "Calculate Single Source Shortest Path");
    mod.def("help", &help,"help functions tells you the parameters and function this CANDY wrapper supports");
    mod.def("about", &about ,"About us");
    mod.def("add", &add, "A function which adds two numbers");

}
