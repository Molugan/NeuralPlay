//
//  main.cpp
//  NN
//
//  Created by Morgane Riviere on 16/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <iostream>

#include "NeuralNet/NeuralNet.h"
#include "DataReader/IrisDataReader.h"

#define LOADING_ERROR 1
#define TRAINING_ERROR 2
#define EVALUATION_ERROR 3

namespace  {
    void SoftMax(const std::vector<float>& resultNN, std::vector<float>& output){
        const int size = resultNN.size();
        output.resize(size);
        
        float normalizationFactor = 0.f;
        
        for(int i = 0; i < size; i++){
            const float expVal = expf(resultNN[i]);
            output[i] = expVal;
            normalizationFactor += expVal;
        }
        
        normalizationFactor = 1.f / normalizationFactor;
        
        for(int i = 0; i < size; i++){
            output[i] *= normalizationFactor;
        }
    }
    
    int GetMaxIndex(const std::vector<float>& input){
        if(input.empty())
            return -1;
        
        int index = 0;
        float maxVal = input[0];
        
        for(int i = 1; i < input.size(); i++){
            if(input[i] > maxVal){
                index = i;
                maxVal = input[i];
            }
        }
        
        return index;
    }
}

int main(int argc, const char * argv[]) {
    
    std::vector< std::vector<float> > inputs;
    std::vector<int> labels;
    
    const std::string pathDBTraining = "/Users/Morgane/NN/IRIS/Data/iris.data";
    
    if(!LoadIrisDataSet(pathDBTraining, inputs, labels)){
        std::cout << "Couldn't load the training data set" << std::endl;
        return LOADING_ERROR;
    }
    
    NeuralNet network;
    network.Init(4, 6, 2, 3);
    
    if(!network.Train(inputs, labels)){
        std::cout << "Failed to train the network" << std::endl;
        return TRAINING_ERROR;
    }
    
    const std::string pathDBTesting = "/Users/Morgane/NN/IRIS/Data/bezdekIris.data";
    if(!LoadIrisDataSet(pathDBTesting, inputs, labels)){
        std::cout << "Couldn't load the test data set" << std::endl;
        return LOADING_ERROR;
    }
    
    std::vector<float> outNetwork;
    int nValids = 0;
    for(int i = 0; i < labels.size(); i++){
        if(!network.Evualuate(inputs[i])){
            std::cout << "Failed to evalutate the network on the test data base" << std::endl;
        }
        
        network.GetOutput(outNetwork);
        SoftMax(outNetwork, outNetwork);
        
        int index = GetMaxIndex(outNetwork);
        
        std::cout << i << " : Expected " <<  labels[i] << " got p = (" << outNetwork[0] << " , " <<outNetwork[1] << " , " << outNetwork[2] << ") ";
        
        if(index == labels[i]){
            nValids++;
            std::cout << "OK" << std::endl;
        }
        else{
            std::cout << "FAILED" << std::endl;
        }
    }
    
    std::cout << "Success rate : " << static_cast<float>(nValids) / static_cast<float>(labels.size()) << std::endl;
    
    network.Save("/Users/Morgane/NN/TestOutput/Patate");
    return 0;
}
