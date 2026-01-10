#ifndef BOARD_H
#define BOARD_H

#include "chessVector.h"
#include <vector>

inline bool isBlack(unsigned char c);
inline bool isBlack(unsigned char c);
inline bool isWhite(unsigned char c);

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY);

enum GameState {
    ONGOING = 2,
    WHITE_WINS = 1,
    BLACK_WINS = -1,
    DRAW = 0
};

enum Castles {
    LONG_BLACK = 0,
    LONG_WHITE = 1,
    SHORT_BLACK = 2,
    SHORT_WHITE = 3
};

enum enPlayers {
    Black = 0,
    White = 1
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
    bool castlesPossible[4] = {1, 1, 1, 1};

    ChessVector KPos[2] = {{4, 0}, {4, 7}};

    // if it is 50, its a draw
    int movesWithoutCapture = 0;

    ChessVector enPassant = {0, 0};
    bool enPassantPossible = false;

    void setBoardState(unsigned char newState[8][8]);

    bool isLegalMove(ChessVector piece, ChessVector goal);
    std::vector<Move> getLegalMoves(ChessVector piece);
    bool existsLegalMove(ChessVector piece);
    bool existsLegalMove(bool checkedForActivePlayer);

    GameState movePiece(ChessVector piece, ChessVector goal, bool changeActivePlayer, unsigned char promotedTo);
    bool isAtackedByOpponent(ChessVector square, bool checkedForActivePlayer);
    bool hasTurn(ChessVector piece);
    unsigned char get(ChessVector piece);
    inline void set(ChessVector piece, unsigned char type);
    inline unsigned char betweenMove(Move move);
    inline unsigned char betweenMove(Move move, unsigned char replacedBy);

private:
    inline void killPiece(ChessVector piece);
    bool movePrecheck(Move move);
};

#endif
