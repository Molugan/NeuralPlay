//
//  HumanPlayer.cpp
//  NN
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(const PlayerColor color, OthelloBoard* gamingBoard)
: Player(color, gamingBoard){
    m_type = kHuman;
}

bool HumanPlayer::GetMove() const{
    
    CoordInt coordsToPlay;
    std::cin >> coordsToPlay;
    return m_board->PlayAt(m_color, coordsToPlay);
}
