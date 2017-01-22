//
//  main.cpp
//  OthelloBoard
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <iostream>
#include "OthelloBoard.h"
#include "HumanPlayer.h"

#define AI_ERROR 1

namespace {
    enum PlayerInput{
        Human,
        AI
    };
    
    std::string PlayerColorToStr(PlayerColor color){
        if(color == kPlayerBlack){
            return "Black";
        }
        else{
            return "White";
        }
    }
    
}

int main(int argc, const char * argv[]) {
    
    OthelloBoard testBoard;
    
    testBoard.Reset();
    
    Player* whitePlayer = new HumanPlayer(kPlayerWhite, &testBoard);
    Player* blackPlayer = new HumanPlayer(kPlayerBlack, &testBoard);
    
    Player* playerList[] = {whitePlayer, blackPlayer};
    
    int indexPlayer = 0;
    
    for(;;){
        Player& currentPlayer = *playerList[indexPlayer];
        PlayerColor currentColor = currentPlayer.GetColor();
        
        std::cout << PlayerColorToStr(currentColor) << " player's turn" << std::endl;
        std::vector<CoordInt> availablePos = testBoard.GetAvailablePos(currentColor);
        
        if(availablePos.empty()){
            availablePos = testBoard.GetAvailablePos(playerList[1 - indexPlayer]->GetColor());
            if(availablePos.empty()){
                std::cout << "End of the game" << std::endl;
                break;
            }
            else{
                indexPlayer = 1 - indexPlayer;
                continue;
            }
        }
        
        if(currentPlayer.GetType() == kHuman){
            testBoard.Print();
            while(!currentPlayer.GetMove()){
                std::cout << "Invalid input, try again" << std::endl;
            }
        }
        
        else if(!currentPlayer.GetMove()){
            std::cout << "AI ERROR, game stopped" << std::endl;
            return AI_ERROR;
        }
        
        indexPlayer = 1 - indexPlayer;
    }
    
    testBoard.Print();
    return 0;
}
