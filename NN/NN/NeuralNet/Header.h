//
//  Header.h
//  NN
//
//  Created by Morgane Riviere on 16/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef NEURALNET_H_
#define NEURALNET_H_

#include <vector>

struct Neuron{
    
    std::vector<float> coeffs;
};

class NeuralNet{
    
    std::vector<Neuron> neuronList;
    
    std::vector<int> sizeLayer;
    
public:
    
    void Init(int sizeInput, int sizeHidden, int sizeOutput);
    
};

#endif /* Header_h */
