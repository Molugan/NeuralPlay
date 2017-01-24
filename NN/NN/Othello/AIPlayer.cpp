//
//  AIPlayer.cpp
//  NN
//
//  Created by Morgane Riviere on 22/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include "AIPlayer.h"

AIPlayer::AIPlayer(const PlayerColor color, OthelloBoard* gamingBoard):
Player(color, gamingBoard){
    m_type = kAI;
}

bool AIPlayer::GetMove(){
    
    std::vector<CoordInt> availablePos = m_board->GetAvailablePos(m_color);
    
    if(availablePos.empty())
        return false;
    
    CoordInt outCoordinates = availablePos[0];
    int outValue = p_caseValue[outCoordinates.second][outCoordinates.first];
    
    const int n_pos = (int) availablePos.size();
    
    for(int j = 1; j < n_pos; j++){
        if(p_caseValue[availablePos[j].second][availablePos[j].first] > outValue){
            outValue = p_caseValue[availablePos[j].second][availablePos[j].first];
            outCoordinates = availablePos[j];
        }
    }
    
    return m_board->PlayAt(m_color, outCoordinates);
    
}
