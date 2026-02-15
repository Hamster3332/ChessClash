//
// Created by malicopter on 13.02.26.
//
#include "fastBoard.h"

#define Knight 9
#define King 8
#define PawnAttackWhite 10
#define PawnAttackBlack 12
#define Rook 0
#define Bishop 4
#define Map1 (mapType)1
//#define FB_Log

#ifdef FB_Log
#define PRINT(x) printf(x)
#define ENDEEEE break;
#define ENDEEEE2
#else
#define PRINT(x)
#define ENDEEEE return true;
#define ENDEEEE2 return true;
#endif

#define ADD_BITS |=
#define RM_BITS &=~

mapType moveMap(const int fromI, int toI, mapType bitmap){
    int movLeft, offset, delta;
    delta = toI - fromI;
    bitmap = (delta < 0) * (bitmap >> -delta) +
            (delta >= 0) * (bitmap << delta);
    offset = delta & 0x7;
    // (offset + offset2) < 8: Move to left
    movLeft = ((offset + (fromI & 7)) < 8);
    // invert cut if movLeft
    bitmap &= (~Cuts[offset] * movLeft) + Cuts[offset] * (!movLeft);
    return bitmap;
}

void printMap(mapType map){
    printf("\n");
    char c;
    for(int i = 0; i < 8 ; i++){
        for(int j = 0; j < 8 ; j++) {
            if (map % 2 == 0) c = '.';
            else c = 'o';
            printf("%c " , c);
            map >>= 1;
        }
        printf("\n");
    }
    printf("\n");
}

void printBoard(fastboard* board){
    for(int i = 0; i < 8 ; i++){
        for(int j = 0; j < 8 ; j++) {
            printf("%c ", pieceToChar[board->board[j + i * 8]]);
        }
        printf("\n");
    }
    printf("\n");
}

bool isLegalMove(int fromI, int toI, fastboard* board){
    unsigned char fromX, fromY, toX, toY, dX, dY;
    fromX = fromI % 8;
    fromY = fromI >> 3;
    toX = toI % 8;
    toY = toI >> 3;
    dX = abs(fromX - toX);
    dY = abs(fromY - toY);
    //printf("%i %i %i %i\n", fromX, fromY, toX, toY);
    PieceType piece = board->board[fromI];
    mapType pieceMap;
    if (fromI == toI) return false; // needs to move
    if (board->board[fromI] == enEmpty) return false;
    if ((board->board[fromI] & 8) == (board->board[toI] & 8) && board->board[toI] != enEmpty) return false; // not same Color
    if (board->activePlayer != (board->board[fromI] >> 3)) return false; // not the correct player
    if (board->gameState != ONGOING) return false; // game not ongoing
    int dir;
    mapType mapTo = Map1 << fromI;

    switch (piece) {
        case enPawnWhite:
        case enPawnBlack:
            break;

        case enKingBlack:
        case enKingWhite: // rochade
            if (dX == 2) {
                if (dY != 0) return false;

                if (fromX < toX) {
                    for (int i = fromX + 1; i < toX; ++i) {
                        if (board->board[fromY * 8 + i] != enEmpty || !isAttackedByOpponent(fromY * 8 + i, !board->activePlayer, board)) return false;
                    }
                } else {
                    for (int i = toX + 1; i < fromX; ++i) {
                        if (board->board[fromY * 8 + i] != enEmpty || !isAttackedByOpponent(fromY * 8 + i, !board->activePlayer, board)) return false;
                    }
                }
            }

            return (dX > 1 || dY > 1 );

        case enKnightBlack:
        case enKnightWhite:
            return ((dX + dY == 3) && dX != 3 && dY != 3);

        case enRookBlack:
        case enRookWhite:
            if ((fromX != toX) && (fromY != toY)) return false;
            if (fromX > toX) {
                for (int i = toX + 1; i < fromX; ++i) {
                    if (board->board[i + fromY * 8] != enEmpty) return false;
                }
            } else if (fromX < toX) {
                for (int i = fromX + 1; i < toX; ++i) {
                    if (board->board[i + fromY * 8] != enEmpty) return false;
                }
            } else if (fromY > toY) {
                for (int i = toY + 1; i < fromY; ++i) {
                    if (board->board[i * 8 + fromX] != enEmpty) return false;
                }
            } else if (fromY < toY) {
                for (int i = fromY + 1; i < toY; ++i) {
                    if (board->board[i * 8 + fromX] != enEmpty) return false;
                }
            } else return false;
            break;

        case enBishopBlack:
        case enBishopWhite:
            if (dX != dY) return false;
            dir = (toI - fromI) / dX;
            fromI += dir; // goto first
            mapTo  = (dir < 0) * (mapTo >> -dir) + (dir > 0) * (mapTo << dir); // goto first
            while (fromI != toI) {
                if ((mapTo & board->bitmaps[enMapAll]) != 0) return false; // check
                fromI += dir;
                mapTo  = (dir < 0) * (mapTo >> -dir) + (dir > 0) * (mapTo << dir);
            }
            break;

        case enQueenBlack:
        case enQueenWhite:
            if ((fromX != toX) && (fromY != toY) && (dX != dY)) return false;
            dir = (toI - fromI) / (dX | dY);
            fromI += dir; // goto first
            mapTo  = (dir < 0) * (mapTo >> -dir) + (dir > 0) * (mapTo << dir); // goto first
            while (fromI != toI) {
                if ((mapTo & board->bitmaps[enMapAll]) != 0) return false; // check
                fromI += dir;
                mapTo  = (dir < 0) * (mapTo >> -dir) + (dir > 0) * (mapTo << dir);
            }
            break;

        default:
            printf("Ouch.");
            break;
    }
    return true;
}

bool isAttackedByOpponent(int posI, Color opponent, fastboard* board){
    mapType checking;
    mapType piece;
    mapType rangeCheck;

    // pawn needs to check direction
    if (opponent){
        checking = moveMap(constMapBases[PawnAttackWhite] , posI , constMoveMap[PawnAttackWhite]);
        piece = board->bitmaps[enPawnWhite];
        if ((piece & checking) != 0){PRINT("Pawn");ENDEEEE2;}
    }else{
        checking = moveMap(constMapBases[PawnAttackBlack] , posI , constMoveMap[PawnAttackBlack]);
        piece = board->bitmaps[enPawnBlack];
        if ((piece & checking) != 0){PRINT("Pawn");ENDEEEE2;}
    }

    checking = moveMap(constMapBases[Knight] , posI , constMoveMap[Knight]);
    piece = board->bitmaps[enKnightBlack | (opponent * 0b1000)];
    if ((piece & checking) != 0){PRINT("Knight");ENDEEEE2;}

    checking = moveMap(constMapBases[King] , posI , constMoveMap[King]);
    piece = board->bitmaps[enKingBlack | (opponent * 8)];
    if ((piece & checking) != 0){PRINT("King");ENDEEEE2;}

    checking = moveMap(constMapBases[Bishop] , posI , constMoveMap[Bishop]); //bishop1 x+y+
    piece = board->bitmaps[enBishopBlack | (opponent * 8)] | board->bitmaps[enQueenBlack | (opponent * 8)]; // bishop or Queen
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck <<= 9;
            if ((piece & rangeCheck) != 0){
                PRINT("Bishop");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }

    checking = moveMap(constMapBases[Bishop + 1] , posI , constMoveMap[Bishop + 1]); //bishop1 x-y+
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck <<= 7;
            if ((piece & rangeCheck) != 0){
                PRINT("Bishop");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }

    checking = moveMap(constMapBases[Bishop + 2] , posI , constMoveMap[Bishop + 2]); //bishop1 x+y-
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck >>= 7;
            if ((piece & rangeCheck) != 0){
                PRINT("Bishop");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }

    checking = moveMap(constMapBases[Bishop + 3] , posI , constMoveMap[Bishop + 3]); //bishop1 x-y-
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck >>= 9;
            if ((piece & rangeCheck) != 0){
                PRINT("Bishop");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }

    checking = moveMap(constMapBases[Rook] , posI , constMoveMap[Rook]); // x+
    piece = board->bitmaps[enRookBlack | (opponent * 8)] | board->bitmaps[enQueenBlack | (opponent * 8)]; // Rook or Queen
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck <<= 1;
            if ((piece & rangeCheck) != 0){
                PRINT("Rook");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }

    checking = moveMap(constMapBases[Rook + 1] , posI , constMoveMap[Rook + 1]); // x-
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck >>= 1;
            if ((piece & rangeCheck) != 0){
                PRINT("Rook");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }

    checking = moveMap(constMapBases[Rook + 2] , posI , constMoveMap[Rook + 2]); // y+
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck <<= 8;
            if ((piece & rangeCheck) != 0){
                PRINT("Rook");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }

    checking = moveMap(constMapBases[Rook + 3] , posI , constMoveMap[Rook + 3]); // y-
    if ((piece & checking) != 0){
        rangeCheck = Map1 << posI;
        do {
            rangeCheck >>= 8;
            if ((piece & rangeCheck) != 0){
                PRINT("Rook");
                ENDEEEE;
            }
        }while (((rangeCheck & board->bitmaps[enMapAll]) == 0));
    }
    PRINT("\n");
    return false;
}

void setPiece(int posI, PieceType piece, fastboard* board){
    board-> board[posI] = piece;
    mapType pieceM = Map1 << posI;
    board-> bitmaps[enMapAll] |= pieceM;
    board-> bitmaps[piece] |= pieceM;
    board-> bitmaps[piece | 7] |= pieceM;
}

void resetPiece(int posI, fastboard* board){
    PieceType piece = board -> board[posI];
    board-> board[posI] = enEmpty;
    mapType pieceM = ~(Map1 << posI);
    board-> bitmaps[enMapAll] &= pieceM;
    board-> bitmaps[piece] &= pieceM;
    board-> bitmaps[piece | 7] &= pieceM;
}

PieceType replacePiece(int posI, PieceType piece, fastboard* board){
    PieceType capture = board->board[posI];
    mapType pieceMap = (Map1 << posI);
    board->board[posI] = piece;
    if (piece == enEmpty) {
        board-> bitmaps[enMapAll] RM_BITS pieceMap;
        board->bitmaps[capture] RM_BITS pieceMap;
        board->bitmaps[capture | 7] RM_BITS pieceMap;
    }
    else{
        board->bitmaps[capture] RM_BITS pieceMap;
        board->bitmaps[capture | 7] RM_BITS pieceMap;
        board->bitmaps[piece] ADD_BITS pieceMap;
        board->bitmaps[piece | 7] ADD_BITS pieceMap;
    }
    return capture;
}

PieceType movePiece(int fromI, int toI, PieceType replace, fastboard* board)
{
    PieceType piece = board->board[fromI];
    PieceType capture = board->board[toI];
    mapType pieceFrom = (Map1 << fromI);
    mapType pieceTo = (Map1 << toI);
    if (replace == enEmpty){
        board->board[fromI] = enEmpty;
        board->board[toI] = piece;

        board-> bitmaps[enMapAll] RM_BITS pieceFrom; // all Pieces rm from
        board-> bitmaps[enMapAll] ADD_BITS pieceTo; // all Pieces set to

        board-> bitmaps[capture | 7] RM_BITS pieceTo; // mapP2 rm To
        board-> bitmaps[capture] RM_BITS pieceTo; // capture rm To

        board-> bitmaps[piece | 7] RM_BITS pieceFrom; // mapP1 rm From
        board-> bitmaps[piece] RM_BITS pieceFrom; // move rm From

        board-> bitmaps[piece | 7] ADD_BITS pieceTo; // mapP1 add To

        board-> bitmaps[piece] ADD_BITS pieceTo; // move add To
    }
    else{
        board->board[fromI] = replace;
        board->board[toI] = piece;

        // all Pieces rm from
        board-> bitmaps[enMapAll] ADD_BITS pieceTo; // all Pieces set to
        board-> bitmaps[capture | 7] RM_BITS pieceTo; // mapP2 rm To
        board-> bitmaps[capture] RM_BITS pieceTo; // capture rm To

        board-> bitmaps[piece | 7] RM_BITS pieceFrom; // mapP1 rm From
        board-> bitmaps[piece] RM_BITS pieceFrom; // move rm From

        board-> bitmaps[replace | 7] ADD_BITS pieceFrom; // mapP1 add To
        board-> bitmaps[replace] ADD_BITS pieceFrom; // move add To

        board-> bitmaps[piece | 7] ADD_BITS pieceTo; // mapP1 add To
        board-> bitmaps[piece] ADD_BITS pieceTo; // move add To
    }
    return capture;
}

fastboard* createBoard(){
    fastboard* new = (fastboard * ) malloc(sizeof(fastboard));
    for(int i = 0; i < 64 ; i++){
        new->board[i] = 14;
    }
    for(int i = 0; i < 15 ; i++){
        new->bitmaps[i] = 0;
    }
    new-> activePlayer = enWHITE;
    new-> enPassantPossible = false;
    new-> enPassantI = 0;
    new-> kingBlackI = 0;
    new-> kingWhiteI = 0;
    new-> castleFlags = 0;
    new-> gameState = Idle;
    return new;
}

/*
(2, 2) (7, 4)

 7 - 2  = 5
 18 % 8 = 2
1 0 0 0 1 0 1 0
0 0 0 1 0 0 0 1
0 0 0 0 0 0 0 0
0 0 0 1 0 0 0 1
0 0 0 0 1 0 1 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0

0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0
0 0 1 0 0 0 0 0
0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 0
0 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0

1 1 1 0 0 0 0 0

*/
void checkConsistency(fastboard* board){
    PieceType piece;
    mapType maps[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    mapType posMap = 0;
    for (unsigned char i = 0; i < 64; i++){
        posMap = Map1 << i;
        piece = board->board[i];
        if (piece != enEmpty){
            maps[enMapAll] ADD_BITS posMap;
            maps[piece | 7] ADD_BITS posMap;
            maps[piece] ADD_BITS posMap;
        }
    }
    for (unsigned char i = 0; i < 16; i++){
        if (maps[i] != board->bitmaps[i]){
            printf("ERROR Map %i %c wrong: Should be", i, pieceToChar[i]);
            printMap(maps[i]);
            printf("But Is");
            printMap(board->bitmaps[i]);
            printf("ERROR End\n");

        }
    }
}

void setBoard(unsigned char board[8][8], fastboard* fBoard){
    for (int x = 0; x < 8; x++){
        for (int y = 0; y < 8; y++){
            switch (board[y][x]) {
                case '.':
                    //setPiece(x, y, enEmpty, fBoard);
                    break;
                case 'p':
                    setPiece(x + y * 8, enPawnBlack, fBoard);
                    break;
                case 'r':
                    setPiece(x + y * 8, enRookBlack, fBoard);
                    break;
                case 'b':
                    setPiece(x + y * 8, enBishopBlack, fBoard);
                    break;
                case 'n':
                    setPiece(x + y * 8, enKnightBlack, fBoard);
                    break;
                case 'k':
                    setPiece(x + y * 8, enKingBlack, fBoard);
                    break;
                case 'q':
                    setPiece(x + y * 8, enQueenBlack, fBoard);
                    break;
                case 'P':
                    setPiece(x + y * 8, enPawnWhite, fBoard);
                    break;
                case 'R':
                    setPiece(x + y * 8, enRookWhite, fBoard);
                    break;
                case 'B':
                    setPiece(x + y * 8, enBishopWhite, fBoard);
                    break;
                case 'N':
                    setPiece(x + y * 8, enKnightWhite, fBoard);
                    break;
                case 'K':
                    setPiece(x + y * 8, enKingWhite, fBoard);
                    break;
                case 'Q':
                    setPiece(x + y * 8, enQueenWhite, fBoard);
                    break;
            }
        }
    }
}