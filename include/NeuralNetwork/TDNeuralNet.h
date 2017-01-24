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
#include <NeuralNetwork/NeuralNet.h>
#include <Othello/OthelloBoard.h>
#include <vector>

class TDNeuralNet : public NeuralNet{
    
public:
    
    static const int kSizeInput = OthelloBoard::width * OthelloBoard::height;
    static const int kSizeOutput = 1;
    static const int kNHiddens   = 2;
    
    TDNeuralNet(const int sizeHidden);
    
    void Reset();
    
    inline float GetValueFromStatus(const CaseStatus& status) const{
        switch (status) {
            case kEmpty:
                return 0;
            case kWhite:
                return 1;
            case kBlack:
                return -1;
        }
    }
    
    inline float GetValueFromOutcome(const int outcome) const{
        if(outcome >= 0)
            return outcome;
        else
            return p_neuronList[p_offsetOutput].output;
    }
    
    float GetOutput(){
        return p_neuronList[p_offsetOutput].output;
    }
    
    bool TrainOnMove(const std::vector<float>& boardStatus, const int endGameScore);
    
private:
    
    float p_lambda;
    
    float p_lastOutput;
    
    bool InitWithBoardStatus(const std::vector<CaseStatus>& boardStatus);
    
    virtual void GetDerivateEnergy(const int);
    
    void UpdateNeuronsWithGradient(const float newOutput);
    
    void ApplyLambdaMultiplier();
    
    /*virtual void ApplyActivationDiffAtNeuron(int idx_neuron);
    
    virtual void ApplyActivationFunctionAtNeuron(int idx_neuron);*/
    
    
};
#endif /* TDNeuralNet_h */
