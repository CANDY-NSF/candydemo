//
// Created by sriram on 11/12/22.
//

#ifndef CANDY_SSSP_STRUCTURE_H
#define CANDY_SSSP_STRUCTURE_H

/*** All Headers Required From ESSENS **/
// INPUT HEADERS

#include "../includes/ESSENS/ESSENS/Core/Basic_IO/Format/Input/Level1/ADJ/create_networkP.hpp"
#include "../includes/ESSENS/ESSENS/Core/Basic_IO/Format/Input/Level2/input_to_network.hpp"

#include <map>
#include <set>

// OUTPUT HEADERS

// #include "printout_others.hpp"

// SET_OPERATIONS
#include "../includes/ESSENS/ESSENS/Core/Basic_SetOps/Level2/unary_set_operations.hpp"

#include "../includes/ESSENS/ESSENS/Core/Basic_SetOps/Level2/binary_set_operations.hpp"

// NETWORK TRAVERSAL
#include "../includes/ESSENS/ESSENS/Core/Basic_Traversal/Level2/ADJ/traversal.hpp"

// ADD and DELETE EDGE
#include "../includes/ESSENS/ESSENS/Basic_Change/Level0/ADJ/add_edge.hpp"
#include "../includes/ESSENS/ESSENS/Basic_Change/Level0/ADJ/del_edge.hpp"
#include <omp.h>

// #include <concurrent_vector.h>
using namespace std;


//========================|
//Structure to indicate whether Edge is to be inserted/deleted
struct xEdge {
    Edge theEdge;
    int inst;
    bool insertedToDatastructure;

    xEdge()
    {
        insertedToDatastructure=false;
    }
    void clear()
    {}
};

//===================|



// Data Structure for each vertex in the rooted tree
struct RT_Vertex
{
    int Root;    //root fo the tree
    int Parent; //mark the parent in the tree
    double EDGwt; //mark weight of the edge
    double Dist;  //Distance from root
    bool Update;  //Whether the distance of this edge was updated
    int num_update; //Number of times the node is updated
    int it; //iteration at which updated
//int degree;
    //Constructor
    RT_Vertex()
    {
        Root=-1;
        Parent=-1;
        EDGwt=0.0;
        Dist =0.0;
        Update=false;
        num_update=0;
        it=0;
    }

    //Destructor
    void clear()
    {}

};
//The Rooted tree is a vector of structure RT_Vertex;


void printRT(vector<RT_Vertex> *RT)
{
    for(int i=0;i<RT->size();i++)
    {
        printf("Vertex:%d: Parent:%d; EDGwt:%f; Dist%f \n", i, RT->at(i).Parent, RT->at(i).EDGwt,  RT->at(i).Dist);
    }
}


void print_tree(vector<RT_Vertex> *RT, char *filename)
{
    //Open file
    ofstream myfile;
    myfile.open (filename, ios::trunc); //clear if same named file exists

    for(int i=0;i<RT->size();i++)
    {

        myfile<< i <<" "<<RT->at(i).Parent<<" "<<RT->at(i).EDGwt<<" "<<RT->at(i).Dist<<"\n";

    }

    //Close File
    myfile.close();

    return;
}


//Structure of Edge with level
struct lEdge {
    Edge theEdge;
    int Level;
};

//Data Structure for Remainder Edges
//Bucket of edge list and its Level in buckets ranging from 0-10; 10-20 20-30  ....90-100
struct RE_Bucket
{
    int st;
    int end;
    vector<Edge> REdges; //set of edges in this bucket
    vector<int_double> val_ind; //index from which a particular value starts

    //Constructor
    RE_Bucket()
    {
        st=-1;
        end=-1;
        REdges.clear();
        val_ind.clear();
    }

    //Destructor
    void clear()
    {}

};

//Define compare for edge
struct EdgeCompare
{
    bool operator() (const Edge& edge1, const Edge& edge2) const
    {
        return edge1.edge_wt< edge2.edge_wt;
    }
};

//Map of Edge to Position Index
typedef map<Edge,int, EdgeCompare> map_Edge_int;







#endif
