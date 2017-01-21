//
//  main.cpp
//  UTNeuralNet
//
//  Created by Morgane Riviere on 17/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "NeuralNet.h"

bool UTValueInit(){
    NeuralNet network;
    network.Init(2, 2, 2, 1);
    
    float tolerance = 0.001f;
    
    std::vector<float> input1 = { 1.f, 0.f};
    float expectedOutput = 0.9472f;
    
    if(!network.Evualuate(input1)){
        std::cout << "Couldn't evaluate the input" << std::endl;
        return false;
    }
    
    std::vector<float> output;
    
    network.GetOutput(output);
    
    if(output.size() != 1){
        std::cout << "Invalid output's size" << std::endl;
        return false;
    }
    
    if(fabsf(output[0] - expectedOutput) > tolerance){
        std::cout << "Invalid output's value" << std::endl;
        std::cout << "Expected " << expectedOutput << std::endl;
        std::cout << "Returned " << output[0] << std::endl;
        return false;
    }
    return true;
}

int main(int argc, const char * argv[]) {
    
    int succededTest = 0;
    int nTest = 1;
    
    if(!UTValueInit()){
        std::cout << "UTValueInit failed" << std::endl;
    }
    else
        succededTest++;
    
    std::cout << succededTest << " succeded" << std::endl;
    std::cout << nTest - succededTest << " failed" << std::endl;
    return 0;
}
