//
//  NNPlayer.cpp
//  NN
//
//  Created by Morgane Riviere on 23/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <Player/NNPlayer.h>

NNPlayer::NNPlayer(const PlayerColor color, OthelloBoard* gamingBoard):
Player(color, gamingBoard),
p_network(60){
    m_type = kNN;
    
    p_boardStatus.resize(OthelloBoard::width * OthelloBoard::height);
    p_network.o_learningRate = 10.f;
}

bool NNPlayer::GetMove(){

    std::vector<CoordInt> availablePos = m_board->GetAvailablePos(m_color);
    
    int newStatus = m_color == kPlayerBlack ? -1 : 1;
    
    UpdateBoardStatus();
    
    if(availablePos.empty())
        return false;
    
    CoordInt outCoordinates = availablePos[0];
    if(p_boardStatus[outCoordinates.first * OthelloBoard::width + outCoordinates.second] != 0){
        return false;
    }
    p_boardStatus[outCoordinates.first * OthelloBoard::width + outCoordinates.second] = newStatus;
    
    //m_board->Print();
    
    p_network.Evualuate(p_boardStatus);
    float outValue = p_network.GetOutput();
    
    p_boardStatus[outCoordinates.first * OthelloBoard::width + outCoordinates.second] = 0;
    
    const int n_pos = (int) availablePos.size();
    
    for(int j = 1; j < n_pos; j++){
        CoordInt currCoordinates = availablePos[j];

        if(p_boardStatus[currCoordinates.first * OthelloBoard::width + currCoordinates.second] != 0){
            return false;
        }

        p_boardStatus[currCoordinates.first * OthelloBoard::width + currCoordinates.second] = newStatus;
        p_network.Evualuate(p_boardStatus);
        
        p_boardStatus[currCoordinates.first * OthelloBoard::width + currCoordinates.second] = 0;
        
        if(p_network.GetOutput() > outValue){
            outValue = p_network.GetOutput();
            outCoordinates = currCoordinates;
        }
    }
    
    // Train the neural network using the chosen move
    p_boardStatus[outCoordinates.first * OthelloBoard::width + outCoordinates.second] = newStatus;
    p_network.TrainOnMove(p_boardStatus, kNULL);
    
    return m_board->PlayAt(m_color, outCoordinates);
}

bool NNPlayer::EndGameMove(){
    
    GameOutcome outcome = GetOutcome();
    UpdateBoardStatus();
    return p_network.TrainOnMove(p_boardStatus, outcome);
}

void NNPlayer::UpdateBoardStatus(){
    
    for(int i = 0, ij = 0; i < OthelloBoard::width; i++){
        for(int j = 0; j < OthelloBoard::height; j++, ij++){
            
            CoordInt currentCoords(i,j);
            p_boardStatus[ij] = p_network.GetValueFromStatus(m_board->GetCaseAtCoordinates(currentCoords));
        }
    }
}

void NNPlayer::NewGame(){
    p_network.Reset();
}
