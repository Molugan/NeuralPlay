//
//  TDNeuralNet.h
//  NN
//
//  Created by Morgane Riviere on 22/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef TDNeuralNet_h
#define TDNeuralNet_h

#include <stdio.h>
#include "NeuralNet.h"

class TDNeuralNet : public NeuralNet{
    
public:
    
private:
    
    float p_lambda;
    
    bool TrainOnMove(const std::vector<int>& boardStatus);
};
#endif /* TDNeuralNet_h */
