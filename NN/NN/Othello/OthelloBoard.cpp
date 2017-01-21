//
//  OthelloBoard.cpp
//  NN
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include "OthelloBoard.h"
#include <iostream>


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
}

void OthelloBoard::Print(){
    
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            switch (boardStatus[i][j]) {
                case kEmpty:
                    std::cout << "E ";
                    break;
                case kBlack:
                    std::cout << "B ";
                    break;
                case kWhite:
                    std::cout << "W ";
                    break;
            }
        }
        std::cout << std::endl;
    }
}
