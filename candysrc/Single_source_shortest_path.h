//
// Created by sriram on 11/9/22.
//

#ifndef CANDY_SINGLE_SOURCE_SHORTEST_PATH_H
#define CANDY_SINGLE_SOURCE_SHORTEST_PATH_H



#include "SP_structure.hpp"
#include "readin_data.hpp"
//#include "printout_others.hpp"
#include "pr.h"
#include "adaptive.h"
float singleSourceShortestPath(std::string inputGraphFile, std::string changedEdges, std::string initialPageRank, int totalNodes, int threads)
{
    clock_t q, q1, q2,t;
    clock_t updateStartTime;
    clock_t updateEndTime;


    const char* ig = inputGraphFile.c_str();
    const char* ce = changedEdges.c_str();
    const char* ipr = initialPageRank.c_str();

    int p = threads;  //total number of threads per core
    int nodes=totalNodes;//total number of nodes

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

    readin_PageRank( ipr,&X);
    q=clock()-q;
    cout << "Total Time for updating SSSP:"<< ((float)q)/CLOCKS_PER_SEC <<"\n";
//    q=clock();
//    cout<<nodes;
//    vector<changes> neighborRevisedList;
//    neighborRevisedList.resize(nodes);
//    cout<<"here";
//    compute_dValue(&X,&p);
//    cout<<"here11";
//
//    readin_changes(ce, &X,&neighborRevisedList);
//
//
//
//    q=clock()-q;
//    cout << "Total Time for Reading Changes"<< ((float)q)/CLOCKS_PER_SEC <<"\n";
//    /**=============================================**/
//
//    int countAffected=0;
//    int count1=countAffectedvertices(&X,false, &countAffected);
//    countAffected=count1;
//    cout <<"here1";
//    int count=0;
//
///* Sriram Dynamic Implementation */
//
//// Update all the meta information
//    int maxL=100;
//    bool etc= 1;
//
//
//    clock_t DanglingStartTime;
//    clock_t updateDanglingEndTime;
//
//    // updateStartTime=clock();
//    //cout <<"max: "<<maxL<<"\n";
//
//
//
//    DanglingStartTime=clock();
//    removeDanglingVertices(&X, &p);
//    countAffectedvertices(&X,true, &countAffected);
//    updateDanglingEndTime=clock()-DanglingStartTime;
//    float updateTime=float(((float)updateDanglingEndTime)/CLOCKS_PER_SEC);
//
//    cout <<fixed<< "Total Time for remove Dangling vertices"<<updateDanglingEndTime <<"\n";
//
//    vector<int>initialList;
//
//    for(int i=0;i<X.size();i++)
//    {
//        if(X.at(i).updateFlag==true){
//            //cout<<"updateList"<<i<<"\n";
//            initialList.push_back(i);
//            X.at(i).updateFlag=false;
//        }
//
//    }
//
//
//    updateStartTime=clock();
//    //printPageRank(&X);
//    //updateinitialIdentified(&X,&p,&maxL,&neighborRevisedList,&initialList);
//    //printPageRank(&X);
//    updatePageRank(&X,&p,&maxL,&neighborRevisedList, &etc);
//    updateEndTime=clock()-updateStartTime;
//    updateTime=float(((float)updateEndTime)/CLOCKS_PER_SEC);
//    cout <<fixed<< "Total Time for updating Network "<< updateTime <<"\n";
//
//
//
//
//// sort to print first 100
////
////
////

//
////pq.push(make_pair(0.35,3));
//
//
////pq.push(make_pair(0.36,1));
//
////pq.push(make_pair(0.32,2));
//
//
//


    return ((float)q)/CLOCKS_PER_SEC;
}



#endif //CANDY_SINGLE_SOURCE_SHORTEST_PATH_H
