//
//  main.cpp
//  OthelloBoard
//
//  Created by Morgane Riviere on 21/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#include <iostream>
#include "OthelloBoard.h"

int main(int argc, const char * argv[]) {
    
    OthelloBoard testBoard;
    
    testBoard.Reset();
    testBoard.Print();
    return 0;
}
