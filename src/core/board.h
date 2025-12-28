#ifndef BOARD_H
#define BOARD_H

bool isLower(unsigned char c);

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY);

class Board {
public:
    unsigned char boardState[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    
    bool activePlayer = 1; // 1 is white and 0 is black
    int gameState = 0;
    // Castle stuff
    bool kingMoved = false, leftRookMoved = false, rightRookMoved = false;
    
    // if it is 50, its a draw
    int movesWithoutCapture = 0;
    
    bool isLegalMove(int pieceX, int pieceY, int goalX, int goalY);
    
};

#endif