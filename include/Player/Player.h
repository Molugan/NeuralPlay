//
//  Player.h
//  NN
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <Othello/OthelloBoard.h>
#include <stdio.h>

enum PlayerType{
    kHuman,
    kAI,
    kNN
};

class Player{
    
private:
    
public:
    
    Player(const PlayerColor color, OthelloBoard* gamingBoard){
        m_color = color;
        m_board = gamingBoard;
    }
    
    virtual bool GetMove()= 0;
    
    PlayerColor GetColor() const{ return m_color;}
    
    PlayerType GetType() const {return m_type;}
    
    virtual bool EndGameMove(){ return true; }
    
    inline GameOutcome GetOutcome(){
        int whiteScore = m_board->GetWhiteScore();
        int blackScore = m_board->GetBlackScore();
        
        if(m_color == kPlayerBlack){
            if(whiteScore > blackScore)
                return kDefeat;
            if(whiteScore == blackScore)
                return kDraw;
            else
                return kVictory;
        }
        else{
            if(whiteScore > blackScore)
                return kVictory;
            if(whiteScore == blackScore)
                return kDraw;
            else
                return kDefeat;
        }
    }
    
    virtual void NewGame(){}

    virtual void StopLearning(){};

    virtual void SlowLearning(){};
    
protected:
    
    PlayerType    m_type;
    OthelloBoard* m_board;
    PlayerColor   m_color;
};

#endif /* Player_h*/
