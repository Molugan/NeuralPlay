//
//  Player.h
//  NN
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include "OthelloBoard.h"
#include <stdio.h>

enum PlayerType{
    kHuman,
    kAI
};

class Player{
    
private:
    
public:
    
    Player(const PlayerColor color, OthelloBoard* gamingBoard){
        m_color = color;
        m_board = gamingBoard;
    }
    
    virtual bool GetMove() const = 0;
    
    PlayerColor GetColor() const{ return m_color;}
    
    PlayerType GetType() const {return m_type;}
    
protected:
    
    PlayerType    m_type;
    OthelloBoard* m_board;
    PlayerColor   m_color;
};

#endif /* Player_h*/
