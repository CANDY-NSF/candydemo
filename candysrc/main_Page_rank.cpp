//
// Created by sriramsrinivas@unomaha.edu on 1/9/20.
//

#include "SP_structure.hpp"
#include "readin_data.hpp"
// #include "printout_others.hpp"
#include "adaptive.h"
#include "pr.h"
#include <iostream>
#include <queue>
#include <string>
// #include "../include/adaptive.h"
// include "pythonadaptive.h"

int pageRank(int argc) {
  clock_t q, q1, q2, t;
  clock_t updateStartTime;
  clock_t updateEndTime;

  if (argc < 7) {
    cout << "INPUT ERROR:: 7 inputs required. First: filename \n Second: file "
            "with PR\n  Third: Set of changed edges \n Fourth: Number of nodes "
            "\n Fifth: Number of threads \n Sixth: Number of iterations\n "
            "Seventh: Output File name ";
  }
  // Check to see if file opening succeeded
  // ifstream the_file1 ( argv[1] ); if (!the_file1.is_open() ) { cout<<"INPUT
  // ERROR:: Could not open file\n";} ifstream the_file2 ( argv[2] ); if
  // (!the_file2.is_open() ) { cout<<"INPUT ERROR:: Could not open file\n";}
  // ifstream the_file3 ( argv[3] ); if (!the_file3.is_open() ) { cout<<"INPUT
  // ERROR:: Could not open file\n";}
  int p = 32;     // total number of threads per core
  int nodes = 10; // total number of nodes
  // ifstream the_file4 ( argv[6] ); if (!the_file3.is_open() ) { cout<<"INPUT
  // ERROR:: Could not open file\n";}

  /**=============================================**/

  /**=============================================**/
  /*** Reading in Network at Time Step 0 ***/
  /***============================================**/
  // Obtain the list of edges.
  q = clock();
  PR_Network X;
  PR_Vertex PR_v;
  X.resize(nodes, PR_v);

  int edges = readin_network(&X, "readedges");

  cout << "Average" << edges / nodes << "\n";

  cout << "done"
       << "\n";
  readin_PageRank("initialpr", &X);
  q = clock() - q;
  cout << "Total Time for Reading Input" << ((float)q) / CLOCKS_PER_SEC << "\n";

  q = clock();

  vector<changes> neighborRevisedList;
  neighborRevisedList.resize(nodes);

  compute_dValue(&X, &p);
  /* for (int i=0;i<10;i++)
   {
    cout <<i<<"::"<<X[i].dValue<<"\n";
   }*/
  readin_changes("changes", &X, &neighborRevisedList);

  /*

     for(int i=0;i<neighborRevisedList.size();i++)
 {
         cout<<i<<"::";
          for (int
 j=0;j<neighborRevisedList.at(i).subtractionNeighborList.size();j++)

          {

                 cout<<neighborRevisedList.at(i).subtractionNeighborList.at(j)<<"\t"<<;

          }



  for (int j=0;j<neighborRevisedList.at(i).additionNeighborList.size();j++)

          {

                 cout<<neighborRevisedList.at(i).additionNeighborList.at(j)<<"\t";

          }

 cout <<"\n";

     }

 */

  q = clock() - q;
  cout << "Total Time for Reading Changes" << ((float)q) / CLOCKS_PER_SEC
       << "\n";
  /**=============================================**/

  int countAffected = 0;
  int count1 = countAffectedvertices(&X, false, &countAffected);
  countAffected = count1;
  cout << "here1";
  int count = 0;
  /*
  for(int i=0;i<X.size();i++)
  { if(X[i].level==0)
          {count++;}
  }
  //    cout <<"in";


  */
  /* Galois Implementation */

  PR_Network Y;
  PR_Vertex PR_v1;
  Y.resize(nodes, PR_v1);
  cout << "here";
  readin_network(&Y, "network");
  prepareinputforGalois(&Y);
  adaptiveModel(&p, "network");
  // read Galois Output

  readGaloisOutput();

  // cout<<count<<":::\n";

  /* Galois COmplete */
  /* Sriram Dynamic Implementation */

  // Update all the meta information
  int maxL = 100;
  bool etc = 1;
  // Obtain value of probability of reaching page--guve n by dValue
  // updateStartTime=clock();

  /*
  for (int i=0;i<X.size();i++)

  {
   for(int j=0;j<X.at(i).In_Neigh.size();j++)
   {

  cout<< i <<":::" <<X.at(i).In_Neigh[j].first<<":::"<<
  X.at(i).In_Neigh[j].second <<"\n";

          }
  }

  */

  // compute_dValue(&X,&p);

  // updateLevel(&X, &p, &maxL);
  // updateReach(&X, &p, &maxL);

  clock_t DanglingStartTime;
  clock_t updateDanglingEndTime;

  // updateStartTime=clock();
  // cout <<"max: "<<maxL<<"\n";

  DanglingStartTime = clock();
  removeDanglingVertices(&X, &p);
  countAffectedvertices(&X, true, &countAffected);
  updateDanglingEndTime = clock() - DanglingStartTime;
  float updateTime = float(((float)updateDanglingEndTime) / CLOCKS_PER_SEC);

  cout << fixed << "Total Time for remove Dangling vertices"
       << updateDanglingEndTime << "\n";

  vector<int> initialList;

  for (int i = 0; i < X.size(); i++) {
    if (X.at(i).updateFlag == true) {
      // cout<<"updateList"<<i<<"\n";
      initialList.push_back(i);
      X.at(i).updateFlag = false;
    }
  }

  updateStartTime = clock();
  // printPageRank(&X);
  // updateinitialIdentified(&X,&p,&maxL,&neighborRevisedList,&initialList);
  // printPageRank(&X);
  updatePageRank(&X, &p, &maxL, &neighborRevisedList, &etc);
  updateEndTime = clock() - updateStartTime;
  updateTime = float(((float)updateEndTime) / CLOCKS_PER_SEC);
  cout << fixed << "Total Time for updating Network " << updateTime << "\n";

  // sort to print first 100
  //
  //
  //
  typedef pair<double, int> iPair;

  priority_queue<iPair, vector<iPair>> pq;

  // pq.push(make_pair(0.35,3));

  // pq.push(make_pair(0.36,1));

  // pq.push(make_pair(0.32,2));

  for (int i = 0; i < X.size(); i++) {

    pq.push(make_pair(X.at(i).page_rank, i));
  }

  ofstream myfile;
  myfile.open("output.txt");

  while (!pq.empty()) {
    myfile << '\t' << pq.top().second << "\t" << pq.top().first << "\n";
    pq.pop();
  }
  myfile << '\n';
  cout << fixed << "Total Time for updating Network " << updateTime << "\n";
  myfile.close();

  /*
  for (int i=0;i<X.size();i++)

  {
   for(int j=0;j<X.at(i).In_Neigh.size();j++)
   {

  cout<< i <<X.at(i).In_Neigh[j].first<<"\n";

          }
  }

  */
  return 1;
}
int main(int argc) { pageRank(argc); }
// cout <<fixed<< "Total Time for updating Network "<< updateTime <<"\n";
