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
    // in order:
    // white: king moved 1 rook moved, 8 rook moved,
    // black: king moved, 1 rook moved, 8 rook moved
    bool piecesHaveMoved[6] = {0};
    
    // if it is 50, its a draw
    int movesWithoutCapture = 0;
    
    int enPassantX = 0, enPassantY = 0;
    bool enPassantPlayer = 0;
    bool enPassantPossible = false;
    
    bool isLegalMove(int pieceX, int pieceY, int goalX, int goalY);
    
private:
    bool kingMove(int pieceX, int pieceY, int goalX, int goalY);
    bool rookMove(int pieceX, int pieceY, int goalX, int goalY);
    bool bishopMove(int pieceX, int pieceY, int goalX, int goalY);
    bool pawnMove(int pieceX, int pieceY, int goalX, int goalY);
    bool knightMove(int pieceX, int pieceY, int goalX, int goalY);
    bool queenMove(int pieceX, int pieceY, int goalX, int goalY);
};

#endif