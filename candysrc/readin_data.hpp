#ifndef READIN_DATA_HPP
#define READIN_DATA_HPP
#include <algorithm>
#include "SP_structure.hpp"
#include <iomanip>
#include <iostream>

using namespace std;


//Read the edgelist of the network
void readin_networkE(char *myfile, vector<int_int> *allEdges, int *nodes)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    int_int edge;
    
    allEdges->clear();
    *nodes=0;
    
    graph_file=fopen(myfile, "r");
    int l=0;
    while(fgets(line,128,graph_file) != NULL)
    {
        if(l==0) {l++; continue;}
        sscanf(line,"%d  %d",&edge.first, &edge.second);
        allEdges->push_back(edge);
        
        if(*nodes<edge.first) {*nodes=edge.first;}
        if(*nodes<edge.second) {*nodes=edge.second;}
        
        l++;
        
    }//end of while
    
    fclose(graph_file);
    
    
}
/*** End of Function ***/

//Read Network From File
int  readin_network(PR_Network *X,  const char *file)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    
    graph_file=fopen(file, "r");
    int l=0;
    int nodes=0;
    int edges=0;
    int_int dummy;
    dummy.first=1;
    dummy.second=0;
    while(fgets(line,128,graph_file) != NULL)
    {
        int n1,n2;
        //Read line
        sscanf(line,"%d %d",&n1,&n2);
        //cout <<n1<<"\n";
        //Number of nodes given in the first line
//        if(l==0)
//        {l++; continue;}

        dummy.first=n1;
        X->at(n2).In_Neigh.push_back(dummy);
        dummy.first=n2;
        X->at(n1).Out_Neigh.push_back(dummy);
       X->at(n1).Out_size++;

        l++;
    }//end of while

    edges=l;
    
    fclose(graph_file);
    
    return edges;
}
/********* End of Function **********/



// information about Page Rank initial Values take them for updating at later stage

void readin_PageRank( const char *file, PR_Network *X)
{

    FILE *graph_file;
    char line[128];
    graph_file=fopen(file, "r");
    int ID;
    float pageRank;

    while(fgets(line,128,graph_file) != NULL)
    {
        sscanf(line,"%d %f",&ID, &pageRank);

        //Add vertex to the component list
            X->at(ID).page_rank=pageRank;

    }
    fclose(graph_file);
}

/*** End of Function ***/


//Information about set of changed edges
int readin_changes( const char *myfile, PR_Network *X, vector<changes> *neighborRevisedList)
{
    //File reading parameters
    FILE *graph_file;
    char line[128];
    int_int edge;
    int type;
    int count=0;
 int countNumberOfAffected=0;
    
    graph_file=fopen(myfile, "r");
    while(fgets(line,128,graph_file) != NULL)
    {  countNumberOfAffected++;
        sscanf(line,"%d  %d %d",&edge.first, &edge.second, &type);
        
        int n1=edge.first;
        int n2=edge.second;
        int_int dummy;
        dummy.first=1;
        dummy.second=0;
       
       // cout <<"here34";
        if(type==0)
        {

		neighborRevisedList->at(n1).subtractionNeighborList.push_back(n2);

            for (int i=0;i<X->at(n1).Out_Neigh.size();i++)
            {
                if(X->at(n1).Out_Neigh[i].first==n2)
                {
	            X->at(n1).Out_Neigh[i].second=-1;
		    X->at(n1).Out_size--;
                    break;
                }
            }
           
	   //cout <<"here"<<n2<<":::"<<"\n"; 
	    for (int i=0;i<X->at(n2).In_Neigh.size();i++)
            {
                if(X->at(n2).In_Neigh[i].first==n1)
                {
			//cout <<"here1"<<"\n";
	            X->at(n2).In_Neigh[i].second=-1;
		    //cout <<"here2"<<"\n";
                    break;
                }
            }
           
	    
          //if(X->at(n2).In_Neigh.size()== X->at(n1).In_Neigh.size())
        // {
	    X->at(n2).updateFlag=true;



            X->at(n2).level=0;
	// }

        }//end of delete
        

        if(type==1)
        {
         //Add edges in X
            dummy.first=n1;
            X->at(n2).In_Neigh.push_back(dummy);
            dummy.first=n2;
            X->at(n1).Out_Neigh.push_back(dummy);
            X->at(n1).Out_size++;

	     neighborRevisedList->at(n1).additionNeighborList.push_back(n2);


//  if(X->at(n2).In_Neigh.size()== X->at(n1).In_Neigh.size())
//{
	  
	    X->at(n2).updateFlag=true;
     

            X->at(n2).level=0;
//}
        }//end of add
        
        
    }//end of while


    //int countNumberOfAffected=0;
    //
    //
/*
 for(int i=0;i<X->size();i++)

 {
		     

	 //cout <<i<<
	 
	 if(X->at(i).updateFlag==true)
{


 for(int j=0;j<X->at(i).Out_Neigh.size();j++)
 {
                   


	int neigh=X->at(i).Out_Neigh[j].first;


 //cout<<i<<"::"<<neigh<<"\n";

                     X->at(neigh).updateFlag=true;


               }
  
             }
}
*/

//cout <<"Total Affected count:"<<countNumberOfAffected<<"\n";
//
//cout <<"Percentage Affected"<<((float)countNumberOfAffected/(float)X->size())*100<<"\n";

    fclose(graph_file);
    return  countNumberOfAffected;
    
}
/*** End of Function ***/


#endif
