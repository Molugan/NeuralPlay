//
//  OthelloBoard.cpp
//  NN
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <Othello/OthelloBoard.h>
#include <iostream>

namespace  {
    int GetDxFromDirection(const Direction dir){
        switch (dir){
            case kUp:
            case kDown:
                return 0;
            break;
            case kLeft:
            case kDiagp135:
            case kDiagm135:
                return -1;
            default:
                return 1;
        }
    }
    
    int GetDyFromDirection(const Direction dir){
        switch (dir) {
            case kUp:
            case kDiagp45:
            case kDiagp135:
                return -1;
            case kDown:
            case kDiagm135:
            case kDiagm45:
                return 1;
            default:
                return 0;
        }
    }
    
    
}

CoordInt operator+(const CoordInt& a, const CoordInt& b){
    return CoordInt(a.first + b.first, a.second + b.second);
}

std::istream& operator>>(std::istream& stream, CoordInt& output){
    stream >> output.first;
    stream >> output.second;
    
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const CoordInt& input){

    stream << input.first << " " << input.second;
    return stream;
}

OthelloBoard::OthelloBoard(){
    Reset();
}

void OthelloBoard::Reset(){
    
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            boardStatus[i][j] = kEmpty;
        }
    }
    
    boardStatus[3][3] = kWhite;
    boardStatus[3][4] = kBlack;
    boardStatus[4][3] = kBlack;
    boardStatus[4][4] = kWhite;
    
    p_nWhitePawns = 2;
    
    p_nBlackPawns = 2;
}

void OthelloBoard::Print(){
    
    std::cout << "  ";
    for(int j = 0; j < width; j++){
        std::cout << j << " ";
    }
    
    std::cout << std::endl;
    for(int i = 0; i < height; i++){
        std::cout << i << " ";
        for(int j = 0; j < width; j++){
            switch (boardStatus[i][j]) {
                case kEmpty:
                    std::cout << ". ";
                    break;
                case kBlack:
                    std::cout << "● ";
                    break;
                case kWhite:
                    std::cout << "○ ";
                    break;
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "White : " << p_nWhitePawns << std::endl;
    std::cout << "Black : " << p_nBlackPawns << std::endl;
}

int OthelloBoard::GetValueFromPosition(const PlayerColor currentPlayer, CoordInt targetCoordinate, const Direction direction) const{
    
    CaseStatus playerCase = currentPlayer == kPlayerBlack ? kBlack : kWhite;
    CaseStatus oppositeCase = currentPlayer == kPlayerBlack ? kWhite : kBlack;
    
    CoordInt dCoord(GetDyFromDirection(direction), GetDxFromDirection(direction));
    
    targetCoordinate = targetCoordinate + dCoord;
    
    if(!IsCoordValid(targetCoordinate)){
        return 0;
    }
    
    if(GetCaseAtCoordinates(targetCoordinate) != oppositeCase){
        return 0;
    }
    
    for(int value = 1; IsCoordValid(targetCoordinate); targetCoordinate = targetCoordinate+dCoord){
        if(GetCaseAtCoordinates(targetCoordinate) == playerCase){
            return value;
        }
    }
    
    return 0;
}

bool OthelloBoard::CanPlay(const PlayerColor currentPlayer, const CoordInt targetCoordinates) const{
    
    if(GetCaseAtCoordinates(targetCoordinates) != kEmpty){
        return false;
    }
    
    for(int dir = kUp; dir < kDIR_MAX; dir++){
        if(GetValueFromPosition(currentPlayer, targetCoordinates, (Direction) dir) > 0){
            return true;
        }
    }
    
    return false;
    
}

bool OthelloBoard::PlayAt(const PlayerColor &currentPlayer, const CoordInt &targetCoordinates){
    
    if(!IsCoordValid(targetCoordinates)){
        return false;
    }
    
    int diffScore = 0;
    
    const CaseStatus playerPawn = currentPlayer == kPlayerBlack ? kBlack : kWhite;
    
    // Is the player trying to play in an empty case ?
    if(GetCaseAtCoordinates(targetCoordinates) != kEmpty){
        return false;
    }
    
    //In all 8 directions, checks if pawns should be flipped
    for(int dir = kUp; dir < kDIR_MAX; dir++){
        int newShift = GetValueFromPosition(currentPlayer, targetCoordinates, (Direction) dir);
        if(newShift > 0){
            diffScore+= newShift;
            CoordInt dCoord = CoordInt(GetDyFromDirection((Direction) dir), GetDxFromDirection((Direction) dir));
            CoordInt currentCoord = targetCoordinates + dCoord;
            for(int shift = 0; shift < newShift; shift++, currentCoord = currentCoord + dCoord){
                SetCaseAtCoordinates(currentCoord, playerPawn);
            }
        }
    }
    
    if(diffScore == 0)
        return false;
    
    SetCaseAtCoordinates(targetCoordinates, playerPawn);
    
    //Update the scores
    if(currentPlayer == kPlayerBlack){
        p_nBlackPawns+= diffScore + 1;
        p_nWhitePawns-= diffScore;
    }
    else{
        p_nWhitePawns+= diffScore + 1;
        p_nBlackPawns-= diffScore;
    }
    return true;
}

std::vector<CoordInt> OthelloBoard::GetAvailablePos(const PlayerColor& currentPlayer) const{
    
    std::vector<CoordInt> output;
    
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(CanPlay(currentPlayer, CoordInt(i,j))){
                output.push_back(CoordInt(i,j));
            }
        }
    }
    
    return output;
}
