//
// Created by sriramsrinivas@unomaha.edu on 5/22/20.
//

#include "mapping.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;
int main() {

  // mapping("file_0.txt","map.txt","output.txt");
  vector<int_int> initialGraphVector;

  Struct outputValues;
  vector<int_int> outputafterReverseSCCmap;
  initialGraphVector.push_back(std::pair<int, int>(13, 12));
  initialGraphVector.push_back(std::pair<int, int>(14, 13));
  initialGraphVector.push_back(std::pair<int, int>(15, 17));
  initialGraphVector.push_back(std::pair<int, int>(16, 15));
  initialGraphVector.push_back(std::pair<int, int>(17, 16));

  outputValues = mappingZeroIndexing(initialGraphVector);

  for (int i = 0; i < outputValues.outputGraphVector.size(); i++) {
    cout << outputValues.outputGraphVector.at(i).first << " "
         << outputValues.outputGraphVector.at(i).second << "\n";
  }

  for (int i = 0; i < outputValues.outputGraphVector.size(); i++) {
    outputafterReverseSCCmap.push_back(std::pair<int, int>(
        mapKey(outputValues, outputValues.outputGraphVector.at(i).first),
        outputValues.outputGraphVector.at(i).second));
  }

  for (int i = 0; i < outputafterReverseSCCmap.size(); i++) {
    cout << outputafterReverseSCCmap.at(i).first << " "
         << outputafterReverseSCCmap.at(i).second << "\n";
  }

  return 0;
}
