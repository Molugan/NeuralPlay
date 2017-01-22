//
//  HumanPlayer.h
//  NN
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef HumanPlayer_h
#define HumanPlayer_h

#include <stdio.h>
#include "Player.h"

class HumanPlayer : public Player{
    
public:
    HumanPlayer(const PlayerColor color, OthelloBoard* gamingBoard);
    
    virtual bool GetMove() const;
};

#endif /* HumanPlayer_h */
