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
    
    int                 p_sizeOutput;
    
    
public:
    
    //!Constructor
    NeuralNet();
    
    void Init(const unsigned int sizeInput,
              const unsigned int sizeHidden,
              const unsigned int nHiddens,
              const unsigned int sizeOutput);
    
    bool Evualuate(const std::vector<float> &input);
    
    bool Save(const std::string outPath) const;
    
    bool Load(const std::string inPath);
    
    void GetOutput(std::vector<float>& output) const;
    
    bool Train(const std::vector<std::vector<float> >& inputs, const std::vector<int>& labels);
    
    float o_learningRate;
    
    int GetOutputwithMaxVal() const;
    
private:
    
    void UpdateNeuronsWithGradient(const int n_samples);
    
    
    float GetSoftMaxNormalier() const;
    
protected:
    
    virtual float GetEnergy(const int expectedLabel);
    
    virtual void GetDerivateEnergy(const int expectedLabel);
    
    virtual void ResetGradient();
    
    virtual void ApplyActivationDiffAtNeuron(int idx_neuron);
    
    virtual void ApplyActivationFunctionAtNeuron(int idx_neuron);
    
    void BackPropagation(const float* input, const int expectedLabel);
    
    void InitTrainingNeurons();
    
    void InitCoeffWithRandomValue();
    
    std::vector<TrainingNeuron> p_trainingNeurons;
    
    std::vector<Neuron>         p_neuronList;
    
    std::vector<int>            p_sizeLayer;
    
    int                         p_offsetOutput;
    
    int                 p_nLayers;
    
};
#endif /* NeuralNet_hpp */
