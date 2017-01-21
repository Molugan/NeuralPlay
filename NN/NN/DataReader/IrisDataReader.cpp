//
//  IrisDataReader.cpp
//  NN
//
//  Created by Morgane Riviere on 18/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include "IrisDataReader.h"
#include <fstream>
#include <vector> 
#include <iostream>

namespace{
    
    void ParseLine(const std::string& line, std::vector<float>& vals, int& label){
        if(line.size() < 16)
            return;
        
        vals.resize(4);
        
        vals[0] = std::stof(line.substr(0, 3));
        vals[1] = std::stof(line.substr(4, 3));
        vals[2] = std::stof(line.substr(8, 3));
        vals[3] = std::stof(line.substr(12, 3));
        
        label = GetLabelFromName(line.substr(16));
    }
}

int GetLabelFromName(const std::string name){
    if(name == "Iris-setosa")
       return 0;
       
    if(name == "Iris-versicolor")
        return 1;
    
    if(name == "Iris-virginica")
        return 2;
    
    return -1;
}


bool LoadIrisDataSet(const std::string pathFile, std::vector< std::vector<float> >& inputDataSet, std::vector<int> &inputLabels){
    
    std::ifstream file;
    file.open(pathFile.c_str());
    
    if(!file){
        std::cout << "LoadIrisDataSet::ERROR::couldn't open " << pathFile << std::endl;
        return false;
    }
    
    inputLabels.clear();
    inputDataSet.clear();
    
    std::vector<float> currentData(4);
    int currentlabel;
    
    while(!file.eof()){
        std::string currentLine;
        file >> currentLine;
        
        ParseLine(currentLine, currentData, currentlabel);
        
        inputLabels.push_back(currentlabel);
        inputDataSet.push_back(currentData);
        
    }
    
    return true;
    
}
