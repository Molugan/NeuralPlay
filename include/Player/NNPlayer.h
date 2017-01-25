//
//  NNPlayer.h
//  NN
//
//  Created by Morgane Riviere on 23/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef NNPlayer_h
#define NNPlayer_h

#include <stdio.h>
#include "Player.h"
#include <NeuralNetwork/TDNeuralNet.h>

class NNPlayer : public Player{
    
private:
    
    TDNeuralNet p_network;
    
    std::vector<float> p_boardStatus;
    
    void UpdateBoardStatus();
    
    bool        p_isLearning;

    bool p_isSwitching;

    void SwitchBoard();
    
    float p_lastOutputBlack;
    
    float p_lastOutputWhite;

public:
    
    NNPlayer(const PlayerColor color, OthelloBoard* gamingBoard);
    
    virtual bool GetMove();
    
    virtual bool EndGameMove();
    
    virtual void NewGame();

    virtual PlayerColor GetColor() const{
        if(p_isSwitching){
            return GetOppositeColor(m_color);
        }
        else{
            return m_color;
        }
    }

    virtual void StopLearning(){ p_isLearning = false; p_isSwitching = false;}
    
    void StopSwitching(){ p_isSwitching = false;}

    virtual void SlowLearning(){ p_network.o_learningRate /=2;}

    void Switch(){ p_isSwitching = !p_isSwitching;}
    
};
#endif /* NNPlayer_h */
