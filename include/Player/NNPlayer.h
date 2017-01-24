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
    
public:
    
    NNPlayer(const PlayerColor color, OthelloBoard* gamingBoard);
    
    virtual bool GetMove();
    
    virtual bool EndGameMove();
    
    virtual void NewGame();
};
#endif /* NNPlayer_h */
