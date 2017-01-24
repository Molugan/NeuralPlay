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
    
    p_lastOutput = 0.f;
    p_lambda     = 0.1f;
}

/*std::vector<float> TDNeuralNet::MakeOutputVectorFromOutcome(GameOutcome outcome){
    std::vector<float> outVector(3, 0.f);
    switch (outcome) {
        case kVictory:
            outVector[0] = 1;
            break;
        case kDraw:
            outVector[1] = 1;
        case kDefeat:
            outVector[2] = 1;
            break;
        case kNULL:
            outVector[0] = p_neuronList[p_offsetOutput].output;
            outVector[1] = p_neuronList[p_offsetOutput + 1].output;
            outVector[2] = p_neuronList[p_offsetOutput + 2].output;
            break;
    }
    
    return outVector;
}*/

void TDNeuralNet::UpdateNeuronsWithGradient(float newOutput){
    
    const int tot_neurons = (int) p_neuronList.size();
        
    const float diffOutput = o_learningRate * (newOutput - p_lastOutput);
        
    for(int i_neuron = 0; i_neuron < tot_neurons; i_neuron++){
        const int nCoeffs = (int) p_neuronList[i_neuron].coeffs.size();
        for(int coeff = 0; coeff < nCoeffs; coeff++){
            p_neuronList[i_neuron].coeffs[coeff] += diffOutput * p_trainingNeurons[i_neuron].dcoeffs[coeff];
        }
    }
}

void TDNeuralNet::ApplyActivationFunctionAtNeuron(int i_neuron){
    p_neuronList[i_neuron].output = Sigmoid(p_neuronList[i_neuron].currentVal);
}

void TDNeuralNet::ApplyActivationDiffAtNeuron(int i_neuron){
    p_trainingNeurons[i_neuron].df_val = dSigmoid(p_neuronList[i_neuron].currentVal);
}

void TDNeuralNet::Reset(){
    ResetGradient();
    p_lastOutput = 0.f;
}

bool TDNeuralNet::TrainOnMove(const std::vector<float>& boardStatus, const GameOutcome expectedOutput){
    
    if(!Evualuate(boardStatus)){
        std::cout << "TDNeuralNet::TrainOnMove::ERROR:: invalid input" << std::endl;
        return false;
    }
    
    float newOutput = GetValueFromOutcome(expectedOutput);
    //Update the current weights using the previous gradients calculated
    UpdateNeuronsWithGradient(newOutput);
    
    //If this isn't the last turn (expectedOutput != kNULL), update the gradients values
    
    if(expectedOutput != kNULL)
        return true;
    
    //Copy the output
    for(int i_output = 0; i_output < kSizeOutput; i_output++){
        p_lastOutput = newOutput;
    }
    
    // Apply the lambda multiplier
    ApplyLambdaMultiplier();
    
    //Finally, add the new gradient computed at this turn
    BackPropagation(&boardStatus[0], 0);

    return true;
}

void TDNeuralNet::GetDerivateEnergy(const int /*expectedLabel*/){
    
    p_trainingNeurons[p_offsetOutput].dE_val= 1;
}

/*void TDNeuralNet::InitTrainingNeuronsTD(){
    
    const int tot_neurons = (int) p_neuronList.size();
    p_trainingNeuronsTD.resize(tot_neurons);
    
    for(int i = 0; i < tot_neurons; i++){
        p_trainingNeuronsTD[i].dcoeffs.resize(kSizeOutput);
        p_trainingNeuronsTD[i].dE_vals.resize(kSizeOutput);
        const int sizeCoeff = (int) p_neuronList[i].coeffs.size();
        for(int out = 0; out < kSizeOutput; out++){
            p_trainingNeuronsTD[i].dcoeffs[out].resize(sizeCoeff, 0);
        }
    }
}
*/

void TDNeuralNet::ApplyLambdaMultiplier(){
    
    const int totNeurons = (int) p_trainingNeurons.size();
    
    for(int i = 0; i < totNeurons; i++){
        const int n_coeffs = (int) p_trainingNeurons[i].dcoeffs.size();
        for(int coeff = 0; coeff < n_coeffs; coeff++){
            p_trainingNeurons[i].dcoeffs[coeff] *= p_lambda;
        }
    }
}

/*void TDNeuralNet::BackpropagationTD(const std::vector<float> &boardStatus, const int currentDerivate){
    //Initialization
    
    //Get dE/dy_out
    GetDerivateEnergy(currentDerivate);
    
    //Backpropagation to the intermediate layers
    int currentLayerOffset = (int) p_neuronList.size();
    int currentLayerSize   = kSizeOutput;
    int nextLayerSize      = p_sizeLayer[p_nLayers - 2];
    int nextLayerOffset    = currentLayerOffset - nextLayerSize;
    int prevLayerOffset    = (int) p_neuronList.size();
    int prevLayerSize      = kSizeOutput;
    
    for(int layer = p_nLayers - 1; layer > 0; layer--){
        
        prevLayerOffset    = currentLayerOffset;
        prevLayerSize      = currentLayerSize;
        currentLayerSize   = p_sizeLayer[layer];
        
        currentLayerOffset -= p_sizeLayer[layer];
        
        nextLayerSize   = p_sizeLayer[layer - 1];
        nextLayerOffset = currentLayerOffset - nextLayerSize;
        
        for(int i_neuron = 0; i_neuron < currentLayerSize; i_neuron++){
            
            const int indexNeuron = i_neuron + currentLayerOffset;
            if(layer != p_nLayers - 1)
                ApplyActivationDiffAtNeuron(indexNeuron);
            else{
                p_trainingNeuronsTD[indexNeuron].df_val = 1;
            }
            
            //if dE/dsout hasn't been initialized yet (ie, if we are not considering the output layer)
            if(layer < p_nLayers - 1){
                p_trainingNeuronsTD[indexNeuron].dE_vals[currentDerivate] = 0;
                
                //Step one : get the value of dE/ds(l,h)   (l = layer, h index of theneuron in the layer)
                for(int i_prev_layer = 0; i_prev_layer < prevLayerSize; i_prev_layer++){
                    p_trainingNeuronsTD[indexNeuron].dE_vals[currentDerivate] += p_trainingNeuronsTD[prevLayerOffset + i_prev_layer].dE_vals[currentDerivate] * p_neuronList[prevLayerOffset + i_prev_layer].coeffs[i_neuron] * p_trainingNeuronsTD[prevLayerOffset + i_prev_layer].df_val;
                }
            }
            
            //Step one : get the value of dE/dw(l,h, i)   (l = layer, h index of the neuron in the layer, i : index of the coefficient in the current neuron)
            for(int i_coeff = 0; i_coeff < nextLayerSize; i_coeff++){
                const float dw = p_trainingNeuronsTD[indexNeuron].dE_vals[currentDerivate] * p_neuronList[i_coeff + nextLayerOffset].output * p_trainingNeurons[indexNeuron].df_val;
                
                // TODO : we could improve it by replacing the learning rate by the jacobian. For now, we will stick to the learning rate
                p_trainingNeuronsTD[indexNeuron].dcoeffs[currentDerivate][i_coeff] += dw;
            }
            
            //don't forget the constant value
            p_trainingNeuronsTD[indexNeuron].dcoeffs[currentDerivate][nextLayerSize] += p_trainingNeuronsTD[indexNeuron].dE_vals[currentDerivate] * p_trainingNeuronsTD[indexNeuron].df_val;
            
        }
    }
    
    
    //finally, deal with the input layer
    prevLayerOffset  = currentLayerOffset;
    prevLayerSize    = currentLayerSize;
    currentLayerSize = p_sizeLayer[0];
    
    for(int i_neuron = 0; i_neuron < currentLayerSize; i_neuron++){
        
        ApplyActivationDiffAtNeuron(i_neuron);
        p_trainingNeuronsTD[i_neuron].dE_vals[currentDerivate] = 0;
        for(int i_prev_layer = 0; i_prev_layer < prevLayerSize; i_prev_layer++){
            p_trainingNeuronsTD[i_neuron].dE_vals[currentDerivate] += p_trainingNeuronsTD[prevLayerOffset + i_prev_layer].dE_vals[currentDerivate] * p_neuronList[prevLayerOffset + i_prev_layer].coeffs[i_neuron] * p_trainingNeuronsTD[prevLayerOffset + i_prev_layer].df_val;
        }
        
        for(int i_coeff = 0; i_coeff < p_sizeInput; i_coeff++){
            const float dw = p_trainingNeuronsTD[i_neuron].dE_vals[currentDerivate] * boardStatus[i_coeff] * p_trainingNeuronsTD[i_neuron].df_val;
            
            p_trainingNeuronsTD[i_neuron].dcoeffs[currentDerivate][i_coeff] += dw;
        }
        
        p_trainingNeuronsTD[i_neuron].dcoeffs[currentDerivate][p_sizeInput] += p_trainingNeuronsTD[i_neuron].dE_vals[currentDerivate] * p_trainingNeuronsTD[i_neuron].df_val;
        
    }
}*/
