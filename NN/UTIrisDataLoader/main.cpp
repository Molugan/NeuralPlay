//
//  main.cpp
//  UTIrisDataLoader
//
//  Created by Morgane Riviere on 18/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <iostream>
#include <vector>
#include "IrisDataReader.h"

int main(int argc, const char * argv[]) {
    
    std::vector<float> inputs0 = {5.1,3.5,1.4,0.2};
    std::vector<float> inputs1 = {4.9,3.0,1.4,0.2};
    std::vector<float> inputs2 = {4.7,3.2,1.3,0.2};
    std::vector<float> inputs3 = {5.0,3.6,1.4,0.2};
    std::vector<float> inputs4 = {5.4,3.9,1.7,0.4};
    
    std::vector< std::vector<float> > expectedInputs = {inputs0, inputs1, inputs2, inputs3, inputs4};
    
    std::vector<int> expectedLabels = {1, 0, 0, 1, 2};
    
    std::vector< std::vector<float> > inputs;
    std::vector<int> labels;
    
    std::string pathToyDB = "/Users/Morgane/NN/IRIS/ToyData/UTIris.data";
    
    if(!LoadIrisDataSet(pathToyDB, inputs, labels)){
        std::cout << "Test failed, couldn't load the data set" << std::endl;
        return 1;
    }
    
    if(inputs.size() != expectedInputs.size()){
        std::cout << "Test failed, invalid input's size" << std::endl;
        return 1;
    }
    
    if(labels.size() != expectedLabels.size()){
        std::cout << "Test failed, invalid labels' size" << std::endl;
        return 1;
    }
    
    for(int i = 0; i < expectedInputs.size(); i++){
        if(expectedInputs[i].size() != inputs[i].size()){
            std::cout << "Test failed, invalid input" << std::endl;
            return 2;
        }
        for(int j = 0; j < expectedInputs[i].size(); j++){
            if(expectedInputs[i][j] != inputs[i][j]){
                std::cout << "Test failed, invalid input" << std::endl;
                return 2;
            }
        }
    }
    
    for(int i = 0; i < expectedLabels.size(); i++){
        if(expectedLabels[i] != labels[i]){
            std::cout << "Test failed, invalid input" << std::endl;
            return 3;
        }
    }
    
    
    std::cout << "Test succeded" << std::endl;
    return 0;
}
