//
// Created by sriramsrinivas@unomaha.edu on 1/9/20.
//

#ifndef PR_PR_H
#define PR_PR_H
#include <limits>
#include <cmath>
typedef std::numeric_limits< double > dbl;
#include "SP_structure.hpp"
#include <omp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <queue>
#include<omp.h>

/*----
 *
 * d= (Pr-V)/(1/n-V)
 * where V= {PR[v]/p(V)
 */


void compute_dValue(PR_Network *X, int *p)
{
    //cout <<"I am here";

// #pragma omp parallel for schedule(dynamic) num_threads(*p)
    for(int i=0;i<X->size();i++) {


	if(X->at(i).In_Neigh.size()==0) {X->at(i).dValue=X->at(i).page_rank*X->size();}
	else
	{
	    //no checking for -1 since this is dvalue before update
     double sum=0.00;
	for(int j=0;j<X->at(i).In_Neigh.size();j++)
          {		  
	    int neigh=X->at(i).In_Neigh[j].first;
	   // printf("%d--%d\n",i,neigh);
	  // cout <<i<<"\t"<<neigh<<"\t"<<X->at(neigh).Out_size <<"\n";
	   sum=sum+((float)X->at(neigh).page_rank/(float)X->at(neigh).Out_size);
	  
	  }

          //cout<<i<<"::"<<sum<<"\n";

	double numerator=X->at(i).page_rank-sum;
	double denominator=((float)1.00/(float)X->size())-sum;


//cout<<i<<"::"<<numerator<<"\n";
//cout<<i<<"::"<<denominator<<"\n";

	X->at(i).dValue=abs(numerator)/abs(denominator);
	} //end of else

	//check for negative
	if(X->at(i).dValue<0) {X->at(i).dValue=-X->at(i).dValue;}
    
	}//end of for
}
/***End of Function ****/



void  printPageRank(PR_Network *X)
{
    int a = 0;
	if(X->size()>40)
	{
		a = 40;
	}
	else{
		a = X->size();
	}
    for (int i=0;i< a;i++) {
        cout << i << ":" << X->at(i).page_rank <<"\n";
    }
}
/***End of Function ****/


/*
     * PR(v)= (d(given in preprocessing file )/n(number of nodes in the network) + (1-d)*[PR(v1)/O(V1)+PR(v2)/)(V2)])

     *   PR(v1)/O(v1)+PR(v2)/ O(v2) == Value which is stored in the meta information structure
     *    V1 and v2 have edges which reach V
     *
     *    not now
     *
     */


void updateinitialIdentified( PR_Network *X, int *p, int *maxL,vector<changes> *neighborRevisedList, vector<int>*initialList )

{


	int counter=0;
  bool change=true;


  #pragma omp parallel for schedule(dynamic) num_threads(*p)

 for (int i=0;i<initialList->size();i++)
 {

 
 double new_pr=X->at(initialList->at(i)).page_rank;
                     for(int j=0;j<neighborRevisedList->at(initialList->at(i)).subtractionNeighborList.size();j++)
                     {

                         new_pr=new_pr-((float)X->at(neighborRevisedList->at(initialList->at(i)).subtractionNeighborList.at(j)).page_rank/(float)X->at(neighborRevisedList->at(initialList->at(i)).subtractionNeighborList.at(j)).Out_size);
                     }
                      for(int j=0;j<neighborRevisedList->at(initialList->at(i)).additionNeighborList.size();j++)
                     {

                         new_pr=new_pr+((float)X->at(neighborRevisedList->at(initialList->at(i)).additionNeighborList.at(j)).page_rank/(float)X->at(neighborRevisedList->at(initialList->at(i)).additionNeighborList.at(j)).Out_size);
                     } 
 

                  for(int j=0;j<X->at(initialList->at(i)).Out_Neigh.size();j++)
                {
                 if(X->at(initialList->at(i)).Out_Neigh[j].second!=-1)
                {
                    int neigh=X->at(initialList->at(i)).Out_Neigh[j].first;

                    X->at(neigh).updateFlag=true;

                }
                }
                     

                  //#pragma atomic update
                     X->at(initialList->at(i)).page_rank=new_pr;
                     }


 }





void updatePageRank(PR_Network *X, int *p, int *maxL,vector<changes> *neighborRevisedList)
{
    //cout<<*maxL;
  int counter=0;
  bool change=true;
  //int condition=*maxL;  
  while(change  &&  counter < *maxL) {
 
	  change=false;
       
	       //Compute new page rank
	//double sum=0.00;
	//cout<<"thraeds"<<*p<<"\n";
            omp_set_num_threads(*p);		 
		 
		#pragma omp parallel for schedule(dynamic)
		 for (int i=0;i<X->size();i++)

		 { //cout<<"t"<<omp_get_num_threads() <<"\n";

			 //cout <<i<<":"<<X->at(i).dValue<<"\n";

                  double sum=0.00;
			 
                  if(X->at(i).updateFlag==true)
		  {

		 for(int j=0;j<X->at(i).In_Neigh.size();j++)
                  {
	           if(X->at(i).In_Neigh[j].second!=-1)		  
	            {int neigh=X->at(i).In_Neigh[j].first;
			   // cout <<neigh<<"test"<<i<<"\n";
			  //  if(X->at(neigh).Dangling==false)
	                    sum=sum+((float)X->at(neigh).page_rank/(float)X->at(neigh).Out_size);
			    //cout <<i<<":::"<<neigh<<":::"<<X->at(neigh).Out_size<<"\n";
		        
		           
		    }
		  }//end of for
               


	            
			  /*
			  double new_pr=X->at(i).page_rank;
                     for(int j=0;j<neighborRevisedList->at(i).subtractionNeighborList.size();j++)
		     {
                           
                         new_pr=new_pr-((float)X->at(neighborRevisedList->at(i).subtractionNeighborList.at(j)).page_rank/(float)X->at(neighborRevisedList->at(i).subtractionNeighborList.at(j)).Out_size);
		     }




		      for(int j=0;j<neighborRevisedList->at(i).additionNeighborList.size();j++)
                     {

                         new_pr=new_pr+((float)X->at(neighborRevisedList->at(i).additionNeighborList.at(j)).page_rank/(float)X->at(neighborRevisedList->at(i).additionNeighborList.at(j)).Out_size);
                     }

*/
                  

 


		 double new_pr=((float)X->at(i).dValue/float((X->size())));
				 
				new_pr+=abs((1-X->at(i).dValue)*sum);
				//
		 //double new_pr=sum;

				 //cout <<i<<":"<<new_pr<<"\n";

	//	 for(int j=0;j<X->at(i).In_Neigh.size();j++)
	//	 {
	//	    if(X->at(i).In_Neigh[j].second!=-1)
	//	    {
	//	      int neigh=X->at(i).In_Neigh[j].first;
                     
		      
		     // if (X->at(neigh).Dangling==True)
		      
		      // #pragma omp atomic update 
		     // X->at(neigh).msg++;
		      
	    if(abs(new_pr-X->at(i).page_rank) >0.000000001)
            {
			    
		    
		  for(int j=0;j<X->at(i).Out_Neigh.size();j++)
                {
                 if(X->at(i).Out_Neigh[j].second!=-1)
                {
                    int neigh=X->at(i).Out_Neigh[j].first;
		   // if(X->at(i).Out_Neigh.size()>2)
                  // {
		    X->at(neigh).updateFlag=true;
		  // }	     


		} 
		}
		     change=true;

		  #pragma atomic update
		     X->at(i).page_rank=new_pr;
		     }
		    
		    
		   
		 
           // }//end of for		     

            }//end of if

           } // end of for
  
        //cout <<counter<<"counter" <<"\n";

        counter++;

    }//end of while
  //  cout <<"maxL"<<*maxL<<"\n";
    //cout<<"counterStatus:"<<counter<<"\n";
}
/***End of Function ***/


void removeDanglingVertices(PR_Network *X, int *p)
{

//long * vertexMarkedForUpdateAfterEachIteration = (long *)malloc (X->size()*sizeof(long));

#pragma omp parallel for schedule(dynamic) num_threads(*p)
for (int i=0;i<X->size();i++)
{

//cout <<i<<" :"<<X->at(i).Out_Neigh.size()<<"size"<<"\n";

if(X->at(i).Out_size<1)
{
	//cout <<i<<"Dangling"<<"\n";
#pragma atomic write
	X->at(i).Dangling=true;

}


}


}

int countAffectedvertices( PR_Network *X,bool dang, int *countAffected )
{
    
    int count=0;
    
    
    for(int i=0;i<X->size();i++)
    {

        if(X->at(i).updateFlag==true && X->at(i).Dangling==dang )
        {
           
		
		count++;
	   

        }
    
    }

    if(dang==true)
	    count=*countAffected-count;

    cout <<"Total Affected count:"<<count<<"\n";

    cout <<"Percentage Affected:"<<((float)count/(float)X->size())*100<<"\n";

return count;

}


//Finding the level of each node
void updateLevel(PR_Network *X, int *p, int *maxL)
{
#pragma omp parallel for schedule(dynamic) num_threads(*p)
   for(int i=0;i<X->size();i++)
   {  
	   
	   if(X->at(i).level==0)
	   {
	    queue<int> myqueue;
            myqueue.push(i);

	    vector<bool> visited;
	    visited.resize(X->size(),false);
    while(!myqueue.empty())
    {

         int node=myqueue.front();
	 myqueue.pop();
	 visited[node]=true;

	 for(int j=0;j<X->at(node).In_Neigh.size();j++)
        {

		int m=X->at(node).In_Neigh[j].first;
                int value=X->at(node).In_Neigh[j].second;
            
		if(value!=-1) {
                    int parentLevel=X->at(node).level+1;
                    int currentNodeLevel=X->at(m).level;
                   
		    //update node level if needed
		    if(currentNodeLevel<parentLevel) {
                        currentNodeLevel=parentLevel;
                         
			#pragma omp atomic write
                        X->at(m).level=currentNodeLevel;
			if(visited[m]==false)
			{

			myqueue.push(m);}
		    }

		    if(currentNodeLevel >*maxL)
		    {*maxL=currentNodeLevel;}

            }//end of if

        }//end of for

    }//end of while


   }//end of if
   }//end of for

}
/**** End of Function ***/


void updateReach(PR_Network *X, int *p, int *maxL)
{
#pragma omp parallel for schedule(dynamic) num_threads(*p)
   for(int i=0;i<X->size();i++)
   {  
	   
	   if(X->at(i).updateFlag==true)
	   {
	    queue<int> myqueue;
            myqueue.push(i);

    while(!myqueue.empty())
    {

         int node=myqueue.front();
	 myqueue.pop();
	 X->at(node).visited=true;

	 for(int j=0;j<X->at(node).In_Neigh.size();j++)
        {

		int m=X->at(node).In_Neigh[j].first;
                int value=X->at(node).In_Neigh[j].second;
            
		if(value!=-1) {

                       #pragma omp atomic update
                        X->at(m).reach++; 

			if(X->at(m).visited==false && X->at(m).updateFlag==false)
			myqueue.push(m);

            }//end of if

        }//end of for

    }//end of while


   }//end of if
   }//end of for

}
/**** End of Function ***/
#endif //PR_PR_H
