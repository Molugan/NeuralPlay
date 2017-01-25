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
    p_network.o_learningRate = 0.005f;

    p_isLearning = true;

    p_isSwitching = false;
    
    p_lastOutputWhite = 0;
    p_lastOutputBlack = 0;
}

bool NNPlayer::GetMove(){

    std::vector<CoordInt> availablePos = m_board->GetAvailablePos(GetColor());
    
    int newStatus = m_color == kPlayerBlack ? -1 : 1;
    
    UpdateBoardStatus();

    if(p_isSwitching){
        newStatus = -newStatus;
        SwitchBoard();
    }
    
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

    if(p_isLearning)
        p_network.TrainOnMove(p_boardStatus, -1, GetColor() == kPlayerBlack ? p_lastOutputBlack : p_lastOutputWhite);

    std::cout << outValue <<" ; ";
    
    p_network.Evualuate(p_boardStatus);
    
    if(GetColor() == kPlayerBlack)
        p_lastOutputBlack = p_network.GetOutput();
    else
        p_lastOutputWhite = p_network.GetOutput();
    
    return m_board->PlayAt(GetColor(), outCoordinates);
}

bool NNPlayer::EndGameMove(){
    
    PlayerColor color = GetColor();
    int score = color == kPlayerBlack ? m_board->GetBlackScore() : m_board->GetWhiteScore();
    UpdateBoardStatus();

    if(p_isSwitching)
        SwitchBoard();

    return p_network.TrainOnMove(p_boardStatus, score, GetColor() == kPlayerBlack ? p_lastOutputBlack : p_lastOutputWhite);
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
    p_lastOutputBlack = 0;
    p_lastOutputWhite = 0;
    p_network.Reset();
}

void NNPlayer::SwitchBoard(){
    for(int i = 0; i < p_boardStatus.size(); i++){
        p_boardStatus[i] *= -1;
    }
}
