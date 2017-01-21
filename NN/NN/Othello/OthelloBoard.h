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

enum CaseStatus{
    kEmpty = 0,
    kWhite,
    kBlack
};

class OthelloBoard{
    
private:
    
    CaseStatus boardStatus[8][8];
    
public:
    
    static const int width = 8;
    static const int height = 8;
    
    void Reset();
    
    void Print();
    
    
};

#endif /* OthelloBoard_h */
