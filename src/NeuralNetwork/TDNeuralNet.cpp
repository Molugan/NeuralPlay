//
//  TDNeuralNet.cpp
//  NN
//
//  Created by Morgane Riviere on 22/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <NeuralNetwork/TDNeuralNet.h>

namespace {
    inline float Sigmoid(float x){
        return 1 / ( 1 + exp(-x));
    }
    
    inline float dSigmoid(float x){
        return 2 / (coshf(x) + 1);
    }
}

TDNeuralNet::TDNeuralNet(const int sizeHidden) : NeuralNet(){
    
    Init(kSizeInput, sizeHidden, kNHiddens, kSizeOutput);
    
    InitTrainingNeurons();
    InitCoeffWithRandomValue();

    ResetGradient();
    
    p_lambda     = 0.3f;
}

void TDNeuralNet::UpdateNeuronsWithGradient(float newOutput, float lastOutput){
    
    const int tot_neurons = (int) p_neuronList.size();
        
    const float diffOutput = o_learningRate * (newOutput - lastOutput);
        
    for(int i_neuron = 0; i_neuron < tot_neurons; i_neuron++){
        const int nCoeffs = (int) p_neuronList[i_neuron].coeffs.size();
        for(int coeff = 0; coeff < nCoeffs; coeff++){
            p_neuronList[i_neuron].coeffs[coeff] += diffOutput * p_trainingNeurons[i_neuron].dcoeffs[coeff];
        }
    }
}

void TDNeuralNet::Reset(){
    ResetGradient();
}

bool TDNeuralNet::TrainOnMove(const std::vector<float>& boardStatus, const int endGameScore, const float lastOutput){
    
    if(!Evualuate(boardStatus)){
        std::cout << "TDNeuralNet::TrainOnMove::ERROR:: invalid input" << std::endl;
        return false;
    }
    
    float newOutput = GetValueFromOutcome(endGameScore);
    //Update the current weights using the previous gradients calculated
    UpdateNeuronsWithGradient(newOutput, lastOutput);

    // Apply the lambda multiplier
    ApplyLambdaMultiplier();
    
    //If this isn't the last turn (expectedOutput != kNULL), update the gradients values
    
    if(endGameScore >= 0)
        return true;
    
    
    //Finally, add the new gradient computed at this turn
    BackPropagation(&boardStatus[0], 0);

    return true;
}

void TDNeuralNet::GetDerivateEnergy(const int /*expectedLabel*/){
    
    p_trainingNeurons[p_offsetOutput].dE_val= 1;
}

void TDNeuralNet::ApplyLambdaMultiplier(){
    
    const int totNeurons = (int) p_trainingNeurons.size();
    
    for(int i = 0; i < totNeurons; i++){
        const int n_coeffs = (int) p_trainingNeurons[i].dcoeffs.size();
        for(int coeff = 0; coeff < n_coeffs; coeff++){
            p_trainingNeurons[i].dcoeffs[coeff] *= p_lambda;
        }
    }
}
