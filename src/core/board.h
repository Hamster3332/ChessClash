#ifndef BOARD_H
#define BOARD_H

#include "chessVector.h"

bool isLower(unsigned char c);

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY);

enum GameState {
    ONGOING = 2,
    WHITE_WINS = 1,
    BLACK_WINS = -1,
    DRAW = 0
};

enum Castles{
    SHORT_BLACK = 0,
    LONG_BLACK = 1,
    SHORT_WHITE = 2,
    LONG_WHITE = 3
};

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
    
    GameState gameState = ONGOING;
    // in order:
    // white: king moved 1 rook moved, 8 rook moved,
    // black: king moved, 1 rook moved, 8 rook moved
    bool castlePiecesMoved[6] = {0};
    
    // if it is 50, its a draw
    int movesWithoutCapture = 0;
    
    int enPassantX = 0, enPassantY = 0;
    bool enPassantPlayer = 0;
    bool enPassantPossible = false;
    
    bool isLegalMove(chessVector piece, chessVector goal);
    void movePiece(chessVector piece, chessVector goal, bool changeActivePlayer);
    
    inline unsigned char get(chessVector piece);
    inline void set(chessVector piece, unsigned char type);
    
private:
    void killPiece(chessVector piece);
};

#endif