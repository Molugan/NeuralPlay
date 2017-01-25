//
//  main.cpp
//  OthelloBoard
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <iostream>
#include <Othello/OthelloBoard.h>
#include <Player/HumanPlayer.h>
#include <Player/AIPlayer.h>
#include <Player/NNPlayer.h>

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
    
    Player* whitePlayer = new AIPlayer(kPlayerWhite, &testBoard);
    Player* blackPlayer = new NNPlayer(kPlayerBlack, &testBoard);
    
    Player* playerList[] = {whitePlayer, blackPlayer};

    //There is a bug with the AI playing against itself
    NNPlayer superPlayer = NNPlayer(kPlayerBlack, &testBoard);
    bool isSuperPlayerTraining = true;
    
    const bool isHumanPlaying = playerList[0]->GetType() == kHuman || playerList[1]->GetType()==kHuman;
    const int n_games = 100000;

    int samplingVictory = 0;
    const int samplingRate = 100;

    //TODO : train the neural network against himself (and not another neural net) using a board reversion
    
    for(int game = 0; game < n_games; game++){
    
        int indexPlayer = 0;

        for(;;){

            Player& currentPlayer =  isSuperPlayerTraining ? superPlayer : *playerList[indexPlayer];
            PlayerColor currentColor = currentPlayer.GetColor();

            if(isHumanPlaying)
            std::cout << PlayerColorToStr(currentColor) << " player's turn" << std::endl;

            std::vector<CoordInt> availablePos = testBoard.GetAvailablePos(currentColor);

            if(availablePos.empty()){
                availablePos = testBoard.GetAvailablePos(playerList[1 - indexPlayer]->GetColor());
                if(availablePos.empty()){
                    //std::cout << "End of the game" << std::endl;
                    break;
                }
                else{
                    if(isSuperPlayerTraining)
                        superPlayer.Switch();
                    indexPlayer = 1 - indexPlayer;
                    continue;
                }
            }

            if(currentPlayer.GetType() == kHuman){
                testBoard.Print();
                    while(!currentPlayer.GetMove()){
                    std::cin.clear();
                    std::cin.ignore(512, '\n');
                    std::cout << "Invalid input, try again" << std::endl;
                }
            }

            else if(!currentPlayer.GetMove()){
                std::cout << "AI ERROR, game stopped" << std::endl;
                return AI_ERROR;
            }

            indexPlayer = 1 - indexPlayer;
            superPlayer.Switch();
        }

        if(game > 0 && game % samplingRate == 0){
            std::cout << static_cast<float>(samplingVictory) / static_cast<float>(samplingRate) << std::endl;
            samplingVictory = 0;
        }
        else{
            if(testBoard.GetBlackScore() > testBoard.GetWhiteScore()){
                samplingVictory++;
            }
        }

        if(isSuperPlayerTraining){
            superPlayer.EndGameMove();
            superPlayer.Switch();
            superPlayer.EndGameMove();
        }
        else{
            for(int n_player = 0; n_player < 2; n_player++){
                playerList[n_player]->EndGameMove();
                playerList[n_player]->NewGame();
            }
        }

        if(isHumanPlaying){
            testBoard.Print();
            break;
        }
        else /*if(!isSuperPlayerTraining)*/{
            //testBoard.Print();
            std::cout << "White : " << testBoard.GetWhiteScore() << " " << testBoard.GetBlackScore() << std::endl;
        }
        
        testBoard.Reset();

    }
    return 0;
}
