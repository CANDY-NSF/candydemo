#ifndef INSDEL_HPP
#define INSDEL_HPP

#include "SSSP_structure.h"
//#include "tree_info.hpp"
//#include "rewire.hpp"
using namespace std;
//Process all Changes
//At the end of executing this function
//All Edges that can be inserted to the SSSP tree will be inserted, and their Parents and Edge weights updated
//All Edges to be inserted into the Remainder List will be added
//All Edges to be deleted from Key edges will be deleted, and their Parents and Edge weights updated
//All Edges to be deleted from Remainder Edges will be marked as deleted
//The rest of the nodes may not be updated.
void edge_update(vector<xEdge> *allChange, A_Network *X,vector<RT_Vertex> *SSSP, double *maxW, int p)
{
    vector<int> Edgedone;
    vector<double> UpdatedDist;

    int iter=0;

    //Mark how the edge is processed
    Edgedone.clear();
    Edgedone.resize(allChange->size(),-1);

    //Store the updated distance value
    UpdatedDist.clear();
    UpdatedDist.resize(X->size(),0.0);

    //Initialize with current distance for each node
#pragma omp parallel for num_threads(p)
    for(int i=0; i<SSSP->size(); i++)
    { UpdatedDist[i]=SSSP->at(i).Dist;}


    bool change=true;

    printf("Insertions %d %d %d  \n", iter, allChange->size(), Edgedone.size());


    int numS=allChange->size();
    //numS=10000000;
#pragma omp parallel for num_threads(p)
    for(int xe=0; xe<numS; xe++)
    {
        //get the edge
        xEdge myxE=allChange->at(xe);
        Edge mye=myxE.theEdge;

        /***Conditions for ignoring edges to insert ***/
        //  printf("%d:%d==%d:%d \n", mye.node1, SSSP->at(mye.node1).Root, mye.node2, SSSP->at(mye.node2).Root);

        //check if edge not same tree--then do not insert
        if(SSSP->at(mye.node1).Root != SSSP->at(mye.node2).Root)
        { Edgedone[xe]=0; //mark to not add
            continue;}

        /*** End of conditions to ignore ***/


        //if insert
        if(myxE.inst==1)
        {
            //We check the distances based on updated-dist, to cull some insertion edges
            //In case of conflicts, actual distance remains correct

            //Default is remainder edge
            Edgedone[xe]=2;


            //Check twice once for  n1->n2 and once for n2->n1
            for(int yy=0;yy<2;yy++)
            {
                int node1, node2;
                if(yy==0)
                {node1=mye.node1;
                    node2=mye.node2;}
                else
                {node1=mye.node2;
                    node2=mye.node1;}

                //  printf("%d:%f:::%d::%f:::%f \n", node1, UpdatedDist[node1],node2, UpdatedDist[node2], mye.edge_wt);
                //Check whether node1 is relaxed
                if(UpdatedDist[node2]> UpdatedDist[node1]+mye.edge_wt)
                {
                    //printf("%d:%f:::%d::%f:::%f \n", node1, UpdatedDist[node1],node2, UpdatedDist[node2], mye.edge_wt);

                    //Update Parent and EdgeWt
                    SSSP->at(node2).Parent=node1;
                    SSSP->at(node2).EDGwt=mye.edge_wt;
                    UpdatedDist[node2]=UpdatedDist[SSSP->at(node2).Parent]+SSSP->at(node2).EDGwt;

                    //Mark Edge to be added--node1 updated
                    Edgedone[xe]=1;
                    break;
                }

            }//end of for =>yy

        }//end of if insert

        else  //if deleted
        {

            //Check if edge exists in the tree
            //this will happen if node1 is parentof node or vice-versa

            //Mark edge as deleted
            if(SSSP->at(mye.node1).Parent==mye.node2)
            {
                SSSP->at(mye.node1).EDGwt=*maxW;}
            else {
                //Mark edge as deleted
                if(SSSP->at(mye.node2).Parent==mye.node1)
                {
                    SSSP->at(mye.node2).EDGwt=*maxW;}
            }


            // Delete from graph
            int ops=0;
            for(int k=0;k<X->at(mye.node1).ListW.size();k++)
            {
                int myn=X->at(mye.node1).ListW[k].first;
                if(myn==mye.node2)
                {
                    X->at(mye.node1).ListW[k].second=-1;
                    ops=X->at(mye.node1).Ops[k];
                    X->at(mye.node2).ListW[ops].second=-1;
                    break;
                }

            }//end of for


        }//end of else if deleted

    }//end of for--going through the inputs



    //Go over the inserted edges to see if they need to be changed
    //At the end of these segment all inserted edges will be processed
    change=true;
    while(change)
    {
        change=false;
#pragma omp parallel for num_threads(p)
        for(int xe=0; xe<numS; xe++)
        {
            //Check if correct edge was inserted
            if(Edgedone[xe]==1)
            {

                //get the edge
                xEdge myxE=allChange->at(xe);
                Edge mye=myxE.theEdge;

                //reset it to 2, remiander edge
                Edgedone[xe]=2;

                int node1, node2;
                if(UpdatedDist[mye.node1]> UpdatedDist[mye.node2])
                {node1=mye.node1;
                    node2=mye.node2;}
                else
                {node1=mye.node2;
                    node2=mye.node1;}

                //printf("TTT:%d:%f %d:%f %f \n", node1, UpdatedDist[node1], node2, UpdatedDist[node2], mye.edge_wt);
                //Check if some other edge was added--mark edge to be added
                if(UpdatedDist[node1]> UpdatedDist[node2]+mye.edge_wt)
                {Edgedone[xe]=1;}

                //Check if incorrect edge wt was written--mark edge to be added
                if( (SSSP->at(node1).Parent==node2) && (SSSP->at(node1).EDGwt!=mye.edge_wt))
                {Edgedone[xe]=1;}

                //Check if incorrect updated distance is given--mark edge to be added
                if(UpdatedDist[node1]!=UpdatedDist[SSSP->at(node1).Parent]+SSSP->at(node1).EDGwt)
                {Edgedone[xe]=1;}


                if(Edgedone[xe]==1)
                {
                    //Update Parent and EdgeWt
                    SSSP->at(node1).Parent=node2;
                    SSSP->at(node1).EDGwt=mye.edge_wt;
                    UpdatedDist[node1]=UpdatedDist[SSSP->at(node1).Parent]+SSSP->at(node1).EDGwt;
                    change =true;
                }


            }//end of if

        }//end of for
    }//end of while change

    //Update the distances
    //Initialize with current distance for each node
#pragma omp parallel for num_threads(p)
    for(int i=0; i<X->size(); i++)
    {
        //do not update source node

        if(SSSP->at(i).Parent==-1){continue;}

        //Update if child of deleted edge
        if(SSSP->at(i).EDGwt==*maxW)
        {SSSP->at(i).Dist=*maxW; SSSP->at(i).Update=true;
            SSSP->at(i).num_update=SSSP->at(i).num_update+1;
            continue;}

        //Updated if child of inserted edge
        if(SSSP->at(i).Dist > UpdatedDist[i])
        { SSSP->at(i).Dist=UpdatedDist[i];
            SSSP->at(i).Update=true;
            SSSP->at(i).num_update=SSSP->at(i).num_update+1;
        }
    }


    return;
}
/**** End of Function*****/





//At the end of this function  the distance of other vertices that were affected by the insertion and deletion will be updated.
//The disconnected parts will remain disconnected if there is no remainder edge to join them

void rest_update(A_Network *X,vector<RT_Vertex> *SSSP, double *maxW, int asl, int p)
{

    int nodes=X->size();
    int its=0; //number of iterations
    bool change=true; //marking whether the connections changed in the iteration




    vector<double> UpdatedDist;
    //Store the updated distance value
    UpdatedDist.clear();
    UpdatedDist.resize(X->size(),0.0);



    //Initialize with current distance for each node
#pragma omp parallel for num_threads(p)
    for(int i=0; i<X->size(); i++)
    { UpdatedDist[i]=SSSP->at(i).Dist;
        //printf("%d:%f--%f\n",i, UpdatedDist[i], SSSP->at(i).Dist);
    }


    int iter=0;
    while(change && its < 70)
    {
        printf("Iteration:%d \n", its);

        change=false;
#pragma omp parallel for num_threads(p)
        for(int t=0;t<nodes; t++)
        {

            //If i is updated--update its neighbors as required
            if(SSSP->at(t).Update && (SSSP->at(t).it==its))
            {

                // printf("%d --%d\n",t,its);
                queue<int> myQ,myQnew;
                int levels=0;

                myQ.push(t);

                while(!myQ.empty() && levels<=asl)
                {


                    int i=myQ.front();
                    myQ.pop();

                    //   printf("%d--%d--%d::\n",i,levels,myQ.size());
                    SSSP->at(i).Update=false;
                    int px=SSSP->at(i).Parent;

                    //For its nieghbors in X
                    for(int j=0;j<X->at(i).ListW.size();j++)
                    {
                        int myn=X->at(i).ListW[j].first;
                        double mywt=X->at(i).ListW[j].second;
                        double mydist;


                        //If edge deleted continue
                        if(mywt<0){continue;}


                        // printf("LL %d:%f:%d...%d:%f:%f \n", i, UpdatedDist[i], px,myn, mywt, UpdatedDist[myn]);


                        //Case 1: Neighbor is Parent
                        //Check that the Edge weight matches the parent
                        //NOTE:using atomic structures can reduce this step
                        if(myn==px)
                        {
                            // printf("in case 1\n");
                            if(SSSP->at(i).EDGwt==*maxW)
                            {continue;}

                            mydist=UpdatedDist[myn]+SSSP->at(i).EDGwt;

                            if( (SSSP->at(i).EDGwt !=mywt) || (UpdatedDist[i]>mydist ))
                            {
                                SSSP->at(i).EDGwt=mywt;
                                if(mywt>=*maxW )
                                {UpdatedDist[i]=*maxW;}
                                else
                                {UpdatedDist[i]=mydist;}

                                if(! SSSP->at(i).Update)
                                { SSSP->at(i).Update=true;
                                    SSSP->at(i).it=its+1;
                                    myQ.push(i);

                                    change=true;}

                            } //end of if

                            SSSP->at(i).num_update=SSSP->at(i).num_update+1;
                            //printf("%d:%f...%d:%f \n", i, UpdatedDist[i], myn, UpdatedDist[myn]);
                            continue;
                        }//end of if case 1


                        //Case 2: neighbor is Child
                        //Update the values
                        if(SSSP->at(myn).Parent==i)
                        {
                            //printf("in case 2\n");

                            SSSP->at(myn).Parent=i;
                            SSSP->at(myn).EDGwt=mywt;
                            UpdatedDist[myn]=UpdatedDist[SSSP->at(myn).Parent]+SSSP->at(myn).EDGwt;


                            if(UpdatedDist[myn]>=*maxW)
                            {UpdatedDist[myn]=*maxW;}
                            if(! SSSP->at(myn).Update)
                            {
                                SSSP->at(myn).Update=true;
                                SSSP->at(myn).it=its+1;
                                myQ.push(myn);

                                change=true;
                            }

                            SSSP->at(i).num_update=SSSP->at(i).num_update+1;

                            //printf("%d:%f...%d:%f \n", i, UpdatedDist[i], myn, UpdatedDist[myn]);
                            continue;
                        }//end of if case 2


                        //Case 3: neighbor is Neutral
                        //Check if distance of neighbor or of i can be lowered
                        if((SSSP->at(myn).Parent!=i) && (SSSP->at(i).Parent!=myn))
                        {
                            // printf("in case 3\n");
                            if(UpdatedDist[i]>UpdatedDist[myn]+mywt)
                            {
                                SSSP->at(i).Parent=myn;
                                SSSP->at(i).EDGwt=mywt;
                                UpdatedDist[i]=UpdatedDist[SSSP->at(i).Parent]+SSSP->at(i).EDGwt;

                                if(! SSSP->at(i).Update)
                                {
                                    SSSP->at(i).Update=true;
                                    SSSP->at(i).it=its+1;
                                    myQ.push(i);
                                    change=true;
                                }
                            }

                            if(UpdatedDist[myn]>UpdatedDist[i]+mywt)
                            {
                                SSSP->at(myn).Parent=i;
                                SSSP->at(myn).EDGwt=mywt;
                                UpdatedDist[myn]=UpdatedDist[SSSP->at(myn).Parent]+SSSP->at(myn).EDGwt;
                                SSSP->at(i).num_update=SSSP->at(i).num_update+1;

                                if(! SSSP->at(myn).Update)
                                {
                                    SSSP->at(myn).Update=true;
                                    SSSP->at(myn).it=its+1;
                                    myQ.push(myn);


                                    change=true;
                                }
                            }
                            //printf("%d:%f...%d:%f \n", i, UpdatedDist[i], myn, UpdatedDist[myn]);
                        }//end of case 3



                    }//end of for neighbors

                    //update level
                    levels++;


                    //myQ=myQnew;

                }//end of while myQ;

            }//end of if Updated

            // for(int j=0;j<myQnew;j++)
            // {SSSP->at(i).Update=true;}


        }//end of for all nodes

        its++;

    }//end of while
    printf("Total Iterations to Converge %d \n", its);

    //Update the distances
    //Initialize with current distance for each node
#pragma omp parallel for num_threads(p)
    for(int i=0; i<X->size(); i++)
    {
        //do not update parent
        if(SSSP->at(i).Parent==-1){continue;}

        int px=SSSP->at(i).Parent;
        if(UpdatedDist[i]>=*maxW)
        {SSSP->at(i).Dist=*maxW;}
        else
        {SSSP->at(i).Dist=UpdatedDist[SSSP->at(i).Parent]+SSSP->at(i).EDGwt;}
    }

    return;
}


#endif


