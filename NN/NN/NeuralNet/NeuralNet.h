//
//  NeuralNet.h
//  NN
//
//  Created by Morgane Riviere on 16/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef NeuralNet_h
#define NeuralNet_h

#include <stdio.h>
#include <math.h>
#include <vector>
#include <fstream>


struct Neuron{
    
    std::vector<float> coeffs;
    
    //TODO : move thoses fileds in the Training neuron struct and store indermediate results in a vector
    float currentVal;
    
    float output;
    
    void Init(){
        currentVal = coeffs.back();
    }
};

//This struct is only used during the training stage
struct TrainingNeuron{
    
    std::vector<float> dcoeffs;
    
    float df_val;
    float dE_val;
    
};

//Structure for a fully connected feed forward neural network
class NeuralNet{
    
    int                 p_sizeInput;
    
    std::vector<Neuron> p_neuronList;
    
    std::vector<TrainingNeuron> p_trainingNeurons;
    
    std::vector<int>    p_sizeLayer;
    
    int                 p_nLayers;
    
    int                 p_sizeOutput;
    
    int                 p_offsetOutput;
    
    
    
public:
    
    //!Constructor
    NeuralNet();
    
    void Init(const unsigned int sizeInput,
              const unsigned int sizeHidden,
              const unsigned int nHiddens,
              const unsigned int sizeOutput);
    
    bool Evualuate(const std::vector<float> &input);
    
    bool Save(const std::string outPath) const;
    
    void GetOutput(std::vector<float>& output) const;
    
    bool Train(const std::vector<std::vector<float> >& inputs, const std::vector<int>& labels);
    
    float o_learningRate;
    
    int GetOutputwithMaxVal() const;
    
private:
    
    float GetEnergy(const int expectedLabel);
    
    void GetDerivateEnergy(const int expectedLabel);
    
    void BackPropagation(const float* input, const int expectedLabel);
    
    void UpdateNeuronsWithGradient(const int n_samples);
    
    void ResetGradient();
    
    void ApplyActivationFunctionAtNeuron(int idx_neuron);
    
    void ApplyActivationDiffAtNeuron(int idx_neuron);
    
    void InitTrainingNeurons();
    
    void InitCoeffWithRandomValue();
    
    float GetSoftMaxNormalier() const;
    
    
};
#endif /* NeuralNet_hpp */
