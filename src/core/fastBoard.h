//
// Created by malicopter on 13.02.26.
//

#ifndef CHESSCLASH_FASTBOARD_H
#define CHESSCLASH_FASTBOARD_H
#ifdef __cplusplus
namespace fb {
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
typedef unsigned long long int  mapType;
typedef unsigned char PieceType;

const mapType constMoveMap[] = {
    // knight, bishop, rook, king, pawn
    0xFE,    //rook x+
    0x7F,//rook x-
    0x0101010101010100,//rook y+
    0x0001010101010101,//rook y-
    0x8040201008040200,    //bishop1 x+y+
    0x0102040810204000,    //bishop1 x-y+
    0x0002040810204080,    //bishop1 x+y-
    0x0040201008040201,    //bishop1 x-y-
    0x0000000000070507, //king
    0x0000000A1100110A, //knight
    0x0000000000000500, //pawn attack White
    0x0000000000010100,  //pawn move White
    0x0000000000000005, //pawn attack Black
    0x0000000000000101,  //pawn move Black
};

const int constMapBases[] = {
        // knight, bishop, rook, king, pawn
        0,    //rook1
        7, //rook2
        0, //rook3
        56,//rook4
        0,    //bishop1 x+y+
        7,    //bishop1 x-y+
        56,    //bishop1 x+y-
        63,    //bishop1 x-y-
        9, //king
        18, //knight
        1, //pawn attack White
        0,  //pawn move White
        9, //pawn attack Black
        16,  //pawn move Black
};

const mapType Cuts[] = {
    0x0000000000000000,
    0x0101010101010101,
    0x0303030303030303,
    0x0707070707070707,
    0x0F0F0F0F0F0F0F0F,
    0x1F1F1F1F1F1F1F1F,
    0x3F3F3F3F3F3F3F3F,
    0x7F7F7F7F7F7F7F7F,
    0xFFFFFFFFFFFFFFFF
};

const char pieceToChar[] = {'p', 'r', 'b', 'n', 'k', 'q', '.', '.', 'P', 'R', 'B', 'N', 'K', 'Q', '.', '.'
};

typedef enum {
    enPawnBlack = 0,
    enRookBlack = 1,
    enBishopBlack = 2,
    enKnightBlack = 3,
    enKingBlack = 4,
    enQueenBlack = 5,

    enMapAll = 6,
    enMapBlack = 7,

    enPawnWhite = 8,
    enRookWhite = 9,
    enBishopWhite = 10,
    enKnightWhite = 11,
    enKingWhite = 12,
    enQueenWhite = 13,

    enEmpty = 14,
    enMapWhite = 15,
} enPieces;

enum GameState {
    Idle = 9,
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

typedef enum {
    enBLACK = 0,
    enWHITE = 1
} Color;

typedef struct {
    PieceType board[64];
    mapType bitmaps[16];

    bool activePlayer, enPassantPossible;
    int enPassantI, kingBlackI, kingWhiteI;
    unsigned char castleFlags; // 0b0123___

    enum GameState gameState;
} fastboard;

mapType moveMap(int fromI, int toI, mapType bitmap);

bool isLegalMove(int fromI, int toI, fastboard* board);
bool isAttackedByOpponent(int posI, Color opponent, fastboard* board);

void setPiece(int posI, PieceType piece, fastboard* board);
void resetPiece(int posI, fastboard* board);
PieceType replacePiece(int posI, PieceType piece, fastboard* board);
PieceType movePiece(int fromI, int toI, PieceType replace, fastboard* board);
void setBoard(unsigned char board[8][8], fastboard* fBoard);
void printBoard(fastboard* board);
fastboard* createBoard();

void checkConsistency(fastboard* board);
#ifdef __cplusplus
}
}
#endif

#endif //CHESSCLASH_FASTBOARD_H
