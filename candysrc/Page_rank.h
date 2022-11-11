//
// Created by sriram on 11/4/22.
//

#ifndef CANDY_PAGE_RANK_H
#define CANDY_PAGE_RANK_H
#include "SP_structure.hpp"
#include "readin_data.hpp"
#include <pybind11/pybind11.h>
//#include "printout_others.hpp"
#include "pr.h"
#include "adaptive.h"
#include <string>

namespace py = pybind11;
float pageRank(std::string inputGraphFile, std::string changedEdges, std::string initialPageRank, int totalNodes, int threads)
{
    clock_t q, q1, q2,t;
    clock_t updateStartTime;
    clock_t updateEndTime;
    py::print("Update process started");


    const char* ig = inputGraphFile.c_str();
    const char* ce = changedEdges.c_str();
    const char* ipr = initialPageRank.c_str();

    int p = threads;  //total number of threads per core
    int nodes=totalNodes;//total number of nodes
    py::print("Graph Name:", inputGraphFile);
    py::print("Nodes:", totalNodes);


    /**=============================================**/

    /**=============================================**/
    /*** Reading in Network at Time Step 0 ***/
    /***============================================**/
    //Obtain the list of edges.
    q=clock();
    PR_Network X;
    PR_Vertex PR_v;
    X.resize(nodes,PR_v);
    int edges=readin_network(&X,ig);
    py::print("Edges:", edges);
//    cout <<"Average:"<<edges/nodes<<"\n";
    readin_PageRank( ipr,&X);
    q=clock()-q;
    py::print("Total Time for reading initial PR:", ((float)q)/CLOCKS_PER_SEC);

    vector<changes> neighborRevisedList;
    neighborRevisedList.resize(nodes);

    compute_dValue(&X,&p);
    int countChanges=readin_changes(ce, &X,&neighborRevisedList);
    q=clock()-q;
    py::print("Total Time for Reading Changes",((float)q)/CLOCKS_PER_SEC);
    py::print("Total Number of changes:",countChanges);
    /**=============================================**/

    int countAffected=0;
    int count1=countAffectedvertices(&X,false, &countAffected);
    countAffected=count1;

    int count=0;



    PR_Network Y;
    PR_Vertex PR_v1;
    Y.resize(nodes,PR_v1);


    int maxL=100;
    clock_t DanglingStartTime;
    clock_t updateDanglingEndTime;

    DanglingStartTime=clock();
    removeDanglingVertices(&X, &p);
    countAffectedvertices(&X,true, &countAffected);
    updateDanglingEndTime=clock()-DanglingStartTime;
    float updateTime=float(((float)updateDanglingEndTime)/CLOCKS_PER_SEC);
    py::print("Total Time for removing Dangling vertices",updateTime);
    vector<int>initialList;

    for(int i=0;i<X.size();i++)
    {
        if(X.at(i).updateFlag==true){
            //cout<<"updateList"<<i<<"\n";
            initialList.push_back(i);
            X.at(i).updateFlag=false;
        }

    }
    updateStartTime=clock();
    updatePageRank(&X,&p,&maxL,&neighborRevisedList);
    updateEndTime=clock()-updateStartTime;
    updateTime=float(((float)updateEndTime)/CLOCKS_PER_SEC);

    typedef pair<double,int> iPair;

    priority_queue< iPair, vector <iPair> > pq;


    for (int i=0;i<X.size();i++)
    {
        pq.push(make_pair(X.at(i).page_rank,i));
    }

    py::print("Top 5 vertices ranked based on updated PR (high-low)");
    py::print("Node_id");
    for (int i=0;i<=4;i++)
    {
        py::print(pq.top().second);
        pq.pop();
    }
    py::print("Total Time for updating Network:", updateTime);

    return ((float)q)/CLOCKS_PER_SEC;
}



#endif //CANDY_PAGE_RANK_H
