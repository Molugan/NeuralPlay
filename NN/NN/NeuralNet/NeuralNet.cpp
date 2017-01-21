//
//  NeuralNet.cpp
//  NN
//
//  Created by Morgane Riviere on 16/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include "NeuralNet.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <ctime>
#include <cstdlib>

namespace {
    inline float Sigmoid(float x){
        return 1 / ( 1 + exp(-x));
    }
    
    inline float dSigmoid(float x){
        return 2 / (coshf(x) + 1);
    }
}

NeuralNet::NeuralNet() :
o_learningRate(0.1f),
p_sizeInput(0),
p_sizeOutput(0){
    
}

void NeuralNet::Init(const unsigned int sizeInput, const unsigned int sizeHidden, const unsigned int nHiddens,const unsigned int sizeOutput){
    
    const int totNeurons = nHiddens * sizeHidden +sizeOutput;
    
    p_neuronList.resize(totNeurons);
    
    for(int i = 0; i < sizeHidden; i++){
        p_neuronList[i].coeffs.resize(sizeInput + 1, 1);
    }
    
    for(int i = sizeHidden; i < totNeurons; i++){
        p_neuronList[i].coeffs.resize(sizeHidden + 1, 1);
    }
    
    p_nLayers = nHiddens + 1;
    p_sizeLayer.resize(p_nLayers);
    for(int i = 0; i< nHiddens; i++){
        p_sizeLayer[i] = sizeHidden;
    }
    
    p_sizeLayer[nHiddens] = sizeOutput;
    p_sizeInput           = sizeInput;
    p_sizeOutput          = sizeOutput;
    p_offsetOutput        = nHiddens * sizeHidden;
}

void NeuralNet::ApplyActivationFunctionAtNeuron(int i_neuron){
    p_neuronList[i_neuron].output = Sigmoid(p_neuronList[i_neuron].currentVal);
}

void NeuralNet::ApplyActivationDiffAtNeuron(int i_neuron){
    p_trainingNeurons[i_neuron].df_val = dSigmoid(p_neuronList[i_neuron].currentVal);
}

bool NeuralNet::Evualuate(const std::vector<float> &input){
    
    if(input.size() != p_sizeInput){
        std::cout << "NeuralNet::Evualuate::Error:: invalid input size" <<std::endl;
        return false;
    }
    
    // Step one : get the value of the first layer
    int currentLayerSize = p_sizeLayer[0];
    int currentCoeffSize = p_sizeInput;
    
    for(int i = 0; i < currentLayerSize; i++){
        p_neuronList[i].Init();
        for(int nVal = 0; nVal < currentCoeffSize; nVal++){
            p_neuronList[i].currentVal += p_neuronList[i].coeffs[nVal] * input[nVal];
        }
        
        ApplyActivationFunctionAtNeuron(i);
    }
    
    int currentLayerOffset = currentLayerSize;
    int lastOffset = 0;
    
    for(int layer = 1; layer < p_nLayers; layer++){
        currentCoeffSize = p_sizeLayer[layer - 1];
        currentLayerSize = p_sizeLayer[layer];
        
        for(int i = 0; i < currentLayerSize; i++){
            const int indexNeuron = i + currentLayerOffset;
            p_neuronList[indexNeuron].Init();
            for(int nVal = 0; nVal < currentCoeffSize; nVal++){
                p_neuronList[indexNeuron].currentVal += p_neuronList[indexNeuron].coeffs[nVal] * p_neuronList[lastOffset + nVal].output;
            }
            
            ApplyActivationFunctionAtNeuron(indexNeuron);
        }
        currentLayerOffset += currentLayerSize;
        lastOffset+= currentCoeffSize;
    }
    
    return true;
}

bool NeuralNet::Save(const std::string outPath) const {
     
    std::ofstream file;
    file.open(outPath);
    
    if(!file){
        std::cout << "NeuralNet::Save::ERROR::couldn't open " << outPath << std::endl;
        return false;
    }
    
    file << p_sizeInput << std::endl;
    file << p_nLayers << std::endl;
    
    int offsetNeuron = 0;
    int lastSize = p_sizeInput;
    
    for(int layer = 0; layer < p_nLayers; layer++){
        const int currentSize = p_sizeLayer[layer] ;
        file << currentSize<< std::endl;
        for(int i = 0; i < currentSize; i++){
            for(int val = 0; val < lastSize + 1; val++){
                file << p_neuronList[offsetNeuron + i].coeffs[val] << " ";
            }
            
            file << std::endl;
        }
        offsetNeuron+=currentSize;
        lastSize = currentSize;
    }
    
    return true;
}

void NeuralNet::GetOutput(std::vector<float>& output) const{
    output.resize(p_sizeOutput);
    
    for(int val = 0; val < p_sizeOutput; val++){
        output[val] = p_neuronList[p_offsetOutput + val].output;
    }
}

int NeuralNet::GetOutputwithMaxVal() const{
    
    int index = 0;
    float value = p_neuronList[p_offsetOutput].output;
    
    for(int i = 1; i < p_sizeOutput; i++){
        if(p_neuronList[p_offsetOutput+ i].output > value){
            index = i;
            value = p_neuronList[p_offsetOutput+ i].output;
        }
    }
    
    return index;
}


///////////////////////////////////////
// Training stage
///////////////////////////////////////

float NeuralNet::GetSoftMaxNormalier() const {
    
    float output = 0.f;
    for(int i_neuron = 0; i_neuron < p_sizeOutput; i_neuron++){
        output += expf(p_neuronList[p_offsetOutput + i_neuron].output);
    }
    
    return output;
}

void NeuralNet::BackPropagation(const float* input, int expectedOutput){
    
    //Initialization
    
    //Get dE/dy_out
    GetDerivateEnergy(expectedOutput);
    
    //Backpropagation to the intermediate layers
    int currentLayerOffset = (int) p_neuronList.size();
    int currentLayerSize   = p_sizeOutput;
    int nextLayerSize      = p_sizeLayer[p_nLayers - 2];
    int nextLayerOffset    = currentLayerOffset - nextLayerSize;
    int prevLayerOffset    = (int) p_neuronList.size();
    int prevLayerSize      = p_sizeOutput;
    
    for(int layer = p_nLayers - 1; layer > 0; layer--){
        
        prevLayerOffset    = currentLayerOffset;
        prevLayerSize      = currentLayerSize;
        currentLayerSize   = p_sizeLayer[layer];
        
        currentLayerOffset -= p_sizeLayer[layer];
        
        nextLayerSize   = p_sizeLayer[layer - 1];
        nextLayerOffset = currentLayerOffset - nextLayerSize;
        
        for(int i_neuron = 0; i_neuron < currentLayerSize; i_neuron++){
            
            const int indexNeuron = i_neuron + currentLayerOffset;
            ApplyActivationDiffAtNeuron(indexNeuron);
            
            //if dE/dsout hasn't been initialized yet (ie, if we are not considering the output layer)
            if(layer < p_nLayers - 1){
                p_trainingNeurons[indexNeuron].dE_val = 0;
            
                //Step one : get the value of dE/ds(l,h)   (l = layer, h index of theneuron in the layer)
                for(int i_prev_layer = 0; i_prev_layer < prevLayerSize; i_prev_layer++){
                    p_trainingNeurons[indexNeuron].dE_val += p_trainingNeurons[prevLayerOffset + i_prev_layer].dE_val * p_neuronList[prevLayerOffset + i_prev_layer].coeffs[i_neuron] * p_trainingNeurons[prevLayerOffset + i_prev_layer].df_val;
                }
            }
            
            //Step one : get the value of dE/dw(l,h, i)   (l = layer, h index of the neuron in the layer, i : index of the coefficient in the current neuron)
            for(int i_coeff = 0; i_coeff < nextLayerSize; i_coeff++){
                const float dw = p_trainingNeurons[indexNeuron].dE_val * p_neuronList[i_coeff + nextLayerOffset].output * p_trainingNeurons[indexNeuron].df_val;
                
                // TODO : we could improve it by replacing the learning rate by the jacobian. For now, we will stick to the learning rate
                p_trainingNeurons[indexNeuron].dcoeffs[i_coeff] += dw;
            }
            
            //don't forget the constant value
            p_trainingNeurons[indexNeuron].dcoeffs[nextLayerSize] += p_trainingNeurons[indexNeuron].dE_val * p_trainingNeurons[indexNeuron].df_val;
            
        }
    }
    
    
    //finally, deal with the input layer
    prevLayerOffset  = currentLayerOffset;
    prevLayerSize    = currentLayerSize;
    currentLayerSize = p_sizeLayer[0];
    
    for(int i_neuron = 0; i_neuron < currentLayerSize; i_neuron++){
        
        ApplyActivationDiffAtNeuron(i_neuron);
        p_trainingNeurons[i_neuron].dE_val = 0;
        for(int i_prev_layer = 0; i_prev_layer < prevLayerSize; i_prev_layer++){
            p_trainingNeurons[i_neuron].dE_val += p_trainingNeurons[prevLayerOffset + i_prev_layer].dE_val * p_neuronList[prevLayerOffset + i_prev_layer].coeffs[i_neuron] * p_trainingNeurons[prevLayerOffset + i_prev_layer].df_val;
        }
        
        for(int i_coeff = 0; i_coeff < p_sizeInput; i_coeff++){
            const float dw = p_trainingNeurons[i_neuron].dE_val * input[i_coeff] * p_trainingNeurons[i_neuron].df_val;
            
            p_trainingNeurons[i_neuron].dcoeffs[i_coeff] += dw;
        }
        
        p_trainingNeurons[i_neuron].dcoeffs[p_sizeInput] += p_trainingNeurons[i_neuron].dE_val * p_trainingNeurons[i_neuron].df_val;
        
    }
    
}

void NeuralNet::UpdateNeuronsWithGradient(const int n_samples){
    
    const int totNeurons = (int) p_neuronList.size();
    for(int i_neuron = 0; i_neuron < totNeurons; i_neuron++){
        const int n_coeffs = (int) p_neuronList[i_neuron].coeffs.size();
        for(int i_coeff = 0; i_coeff < n_coeffs; i_coeff++){
            p_neuronList[i_neuron].coeffs[i_coeff] -= p_trainingNeurons[i_neuron].dcoeffs[i_coeff] * o_learningRate / n_samples;
        }
    }
}

void NeuralNet::ResetGradient(){
    const int totNeurons = (int) p_neuronList.size();
    for(int i_neuron = 0; i_neuron < totNeurons; i_neuron++){
        const int n_coeffs = (int) p_neuronList[i_neuron].coeffs.size();
        for(int i_coeff = 0; i_coeff < n_coeffs; i_coeff++){
            p_trainingNeurons[i_neuron].dcoeffs[i_coeff] = 0;
        }
    }
}

void NeuralNet::InitTrainingNeurons(){
    const int totNeurons = (int) p_neuronList.size();
    p_trainingNeurons.resize(totNeurons);
    
    for(int i_neuron = 0; i_neuron < totNeurons; i_neuron++){
        const int n_coeffs = (int) p_neuronList[i_neuron].coeffs.size();
        p_trainingNeurons[i_neuron].dcoeffs.resize(n_coeffs, 0.f);
    }
}

void NeuralNet::InitCoeffWithRandomValue(){
    
    std::srand(std::time(0));
    float factorRand = 1.f / static_cast<float>(RAND_MAX);
    const int totNeurons = (int) p_neuronList.size();
    for(int i_neuron = 0; i_neuron < totNeurons; i_neuron++){
        const int n_coeffs = (int) p_neuronList[i_neuron].coeffs.size();
        for(int i_coeff = 0; i_coeff < n_coeffs; i_coeff++){
            p_neuronList[i_neuron].coeffs[i_coeff] = 2.f * (static_cast<float>(std::rand()) * factorRand - 0.5f);
        }
    }
}

//Slow, find a clean way to merge it with the GetDerivateEnergy function
// Here we use a softmax layer on the output merged with a cross entropy function to build the error cost function
float NeuralNet::GetEnergy(const int expectedLabel){
    
    float softMaxNormalizer = GetSoftMaxNormalier();
    float output = p_sizeOutput * log(softMaxNormalizer);
    for(int i_neuron = 0; i_neuron < p_sizeOutput; i_neuron++){
        if(i_neuron==expectedLabel){
            output -= p_neuronList[expectedLabel + p_offsetOutput].output;
        }
        else{
            output -= log(softMaxNormalizer - expf(p_neuronList[i_neuron + p_offsetOutput].output));
        }
    }
    return output;
    
}

void NeuralNet::GetDerivateEnergy(const int expectedLabel){
    
    float softMaxNormalizer = GetSoftMaxNormalier();
    
    for(int i_neuron = 0; i_neuron < p_sizeOutput; i_neuron++){
        if(i_neuron == expectedLabel){
            p_trainingNeurons[p_offsetOutput + i_neuron].dE_val = -1 + p_sizeOutput * expf(p_neuronList[expectedLabel + p_offsetOutput].output)/ softMaxNormalizer;
            for(int j = 0; j < p_sizeOutput; j++){
                if(j == expectedLabel)
                    continue;
                p_trainingNeurons[p_offsetOutput + i_neuron].dE_val -= expf(p_neuronList[expectedLabel + p_offsetOutput].output)/ (softMaxNormalizer - expf(p_neuronList[j + p_offsetOutput].output));
            }
        }
        else{
            p_trainingNeurons[p_offsetOutput + i_neuron].dE_val = p_sizeOutput * expf(p_neuronList[i_neuron + p_offsetOutput].output) / softMaxNormalizer;
            for(int j = 0; j < p_sizeOutput; j++){
                if(j== expectedLabel)
                    continue;
                else if(j == i_neuron)
                    continue;
                else{
                    p_trainingNeurons[i_neuron + p_offsetOutput].dE_val -= expf(p_neuronList[i_neuron + p_offsetOutput].output)/(softMaxNormalizer - expf(p_neuronList[i_neuron + p_offsetOutput].output));
                                                                                                                                 
                }
            }
        }
    }

}
bool NeuralNet::Train(const std::vector<std::vector<float> >& inputs, const std::vector<int>& labels){
    
    //Preliminary checks
    const int n_samples = inputs.size();
    if(labels.size() != n_samples){
        std::cout << "NeuralNet::Train::ERROR:: invalid output" << std::endl;
        return false;
    }
    
    // stage one, inits the neurons'coefficients randomly
    InitCoeffWithRandomValue();
    
    //Also initialize the training neurons
    InitTrainingNeurons();
    
    float current_error = std::numeric_limits<float>::max();
    int n_iterations = 10000;
    float error_threshold = 0.5f;
    float accuracyRate = 0;
    
    for(int iteration = 0; iteration < n_iterations; iteration++){
        
        float newError = 0.f;
        // Reset the gradient
        ResetGradient();
        
        int accuracy = 0;
        
        for(int sample = 0; sample < n_samples; sample++){
            
            // Step one, launch the neural network on the input sample
            if(!Evualuate(inputs[sample])){
                std::cout << "NeuralNet::Train::ERROR:: invalid input training data" << std::endl;
                return false;
            }
            
            const int labelOutput = GetOutputwithMaxVal();
            if(labels[sample] == labelOutput)
                accuracy++;
            
            //Step two : deduce the energy of the current state
            newError += GetEnergy(labels[sample]);

            
            //Step three : get the energy gradient using the backpropagation method
            BackPropagation(&inputs[sample][0], labels[sample]);
        }
        
        newError /= n_samples;
        const float newAccuracyRate = static_cast<float>(accuracy) / static_cast<float>(n_samples);
        if(newError > current_error || fabs(current_error ) < error_threshold){
            std::cout <<"NeuralNet::Train:: ending after " << iteration << " iterations" << std::endl;
            return true;
        }
        
        current_error = newError;
        
        UpdateNeuronsWithGradient(n_samples);
        
        if(iteration % 100 == 0){
            std::cout << "Iteration " << iteration << " error : " << current_error << " ; accuracy "<< newAccuracyRate << std::endl;
        }
    }
    
    return true;
    
}


