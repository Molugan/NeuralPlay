//
//  OthelloBoard.hpp
//  NN
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef OthelloBoard_h
#define OthelloBoard_h

#include <stdio.h>
#include <utility>
#include <vector>
#include <iostream>

typedef std::pair<int, int> CoordInt;
CoordInt operator+(const CoordInt& a, const CoordInt &b);
std::istream& operator>>(std::istream& stream, CoordInt& output);

enum PlayerColor{
    kPlayerWhite = 0,
    kPlayerBlack
};

enum Direction{
    kUp = 0,
    kDown,
    kLeft,
    kRight,
    kDiagp45,
    kDiagp135,
    kDiagm45,
    kDiagm135,
    kDIR_MAX
    
};

enum CaseStatus{
    kWhite = 0,
    kBlack,
    kEmpty
};

enum GameOutcome{
    kVictory = 0,
    kDraw,
    kDefeat,
    kNULL
};

class OthelloBoard{
    
private:
    
    CaseStatus boardStatus[8][8];
    
    int GetValueFromPosition(const PlayerColor currentPlayer, CoordInt targetCoordinate, const Direction direction) const;
    
    inline bool IsCoordValid(const CoordInt& coord) const{
        return (coord.first >= 0 && coord.first < height && coord.second >= 0 && coord.second < width);
    }
    
    inline void SetCaseAtCoordinates(const CoordInt& coord, const CaseStatus& value){
        boardStatus[coord.first][coord.second] = value;
    }
    
    int p_nWhitePawns;
    
    int p_nBlackPawns;
    
public:
    
    static const int width = 8;
    static const int height = 8;
    static const int nCases = width * height;
    
    OthelloBoard();
    
    //Before starting any new game
    void Reset();
    
    //Print the board's status
    void Print();
    
    int GetWhiteScore() const{ return p_nWhitePawns;}
    
    int GetBlackScore() const{ return p_nBlackPawns;}
    
    CaseStatus GetCaseAtCoordinates(const CoordInt& coordinates) const{
        return boardStatus[coordinates.first][coordinates.second];
    }
    
    bool CanPlay(PlayerColor currentPlayer, CoordInt targetCoordinates) const ;
    
    std::vector<CoordInt> GetAvailablePos(const PlayerColor& currentPlayer) const;
    
    bool PlayAt(const PlayerColor& currentPlayer, const CoordInt& targetCoordinates);
    
};

#endif /* OthelloBoard_h */
