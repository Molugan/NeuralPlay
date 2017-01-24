//
//  AIPlayer.hpp
//  NN
//
//  Created by Morgane Riviere on 22/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef AIPlayer_h
#define AIPlayer_h

#include <stdio.h>
#include "Player.h"

class AIPlayer : public Player{
    
private:
    
    int p_caseValue[8][8] =
    {
        {120, -20, 20,  5,  5, 20, -20, 120},
        {-20, -40, -5, -5, -5, -5, -40, -20},
        { 20,  -5, 15,  3,  3, 15,  -5,  20},
        {  5,  -5,  3,  3,  3,  3,  -5,   5},
        {  5,  -5,  3,  3,  3,  3,  -5,   5},
        { 20,  -5, 15,  3,  3, 15,  -5,  20},
        {-20, -40, -5, -5, -5, -5, -40, -20},
        {120, -20, 20,  5,  5, 20, -20, 120}
    };
    
public:
    
    AIPlayer(const PlayerColor color, OthelloBoard* gamingBoard);
    
    virtual bool GetMove();
};
#endif /* AIPlayer_h */
