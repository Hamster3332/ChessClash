#include "board.h"
#include "chessVector.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iostream>


bool isLower(unsigned char c) {
    return c & 32;// >> 5;
    // so uhm 32 has this 1 bit which changes in the ascii table
    // when letters go from upper to lowercase. Bitshift to make the result 0 or 1 instead of 32
    // when true.
}

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY) {
    return std::abs(goalX - pieceX) + std::abs(goalY - pieceY);
};

bool Board::isLegalMove(chessVector piece, chessVector goal) {
    unsigned char pieceChar = boardState[piece.y][piece.x];
    unsigned char goalPiece = boardState[goal.y][goal.x];
    unsigned char pieceType = std::tolower(pieceChar);

    bool pieceIsBlack = isLower(pieceChar);
    bool pieceIsWhite = !pieceIsBlack;
    bool goalIsBlack = isLower(goalPiece);
    bool goalIsWhite = !goalIsBlack;

    bool isDifX = (piece.x != goal.x);
    bool isDifY = (piece.y != goal.y);

    chessVector dist = goal.getDistance(piece);
    chessVector changeVector = piece.getTo(goal);
    chessVector change = changeVector.normToInt();

    if (gameState != ONGOING) return false; // game is over

    if (pieceChar == '.') return false; // moving an empty square?

    if (pieceIsBlack && goalIsBlack && goalPiece != '.') return false; // capture your own pieces
    if (pieceIsWhite && goalIsWhite && goalPiece != '.') return false;

    if (activePlayer && pieceIsBlack) return false; // not your turn
    if (!activePlayer && pieceIsWhite) return false;

    if (!isDifX && !isDifY) return false; // piece captures itself

    if (enPassantPossible && activePlayer == enPassantPlayer) {
        enPassantPossible = false;
    }

    switch (pieceType) {
        // king
        case 'k':
        // wants to castle
        if (dist.x == 2 && dist.y == 0) {
            // if white king has not moved
            if (pieceIsWhite && !castlePiecesMoved[0]) {
                // if he castles short (right)
                if ((change.x == 1 && !castlePiecesMoved[1]) ||
                    // or long (left)
                    (change.x == -1 && !castlePiecesMoved[2])) {
                    // if the squares inbetween are free
                    // TODO need to check for checks
                    if (boardState[goal.y][goal.x] != '.' ||
                        boardState[goal.y][goal.x - change.x] != '.') {
                            return false;
                    }
                } else {
                    return false;
                }
            } // if black king has not moved
            else if (!castlePiecesMoved[3]) {
                // if he castles short (right)
                if ((change.x == 1 && !castlePiecesMoved[4]) ||
                    // or long (left)
                    (change.x == -1 && !castlePiecesMoved[5])) {
                    // if the squares inbetween are free
                    // TODO need to check for checks
                    if (boardState[goal.y][goal.x] != '.' ||
                        boardState[goal.y][goal.x - change.x] != '.') {
                            return false;
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
            // all conditions are met

        }
        else if (dist.x > 1 || dist.y > 1) // moved further than 1 square
            return false;
        // king has moved
        castlePiecesMoved[3 * (pieceIsBlack)] = true;
        break;

        // rook
        case 'r':
        // not orthogonal
        if (dist.x != 0 && dist.y != 0){
            return false;
        }
        // move in change Direction
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                return false;
            }
        }
        break;

        // bishop
        case 'b':
        // not diagonal
        if (dist.x != dist.y) {
            return false;
        }
        
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                return false;
            }
        }
        break;

        // queen
        case 'q':
        // not diagonal
        if (dist.x != dist.y) {
            // not orthogonal
            if (dist.x != 0 && dist.y != 0){
                return false;
            }
        }
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                return false;
            }
        }
        break;

        // knight
        case 'n':
        if (dist.manhattanDist() != 3 || dist.x > 2 || dist.y > 2) {
            return false;
        }
        break;

        // pawn (oh no)
        case 'p':
        if (dist.x > 1 || dist.y > 2 || change.y == 0) {
            return false;
        }
        if ((change.x != 0 && boardState[goal.y][goal.x] == '.' && !enPassantPossible) ||
            (change.x != 0 && dist.y != 1)) {
            return false;
        }

        if (pieceIsWhite) {
            if (change.y != -1) {
                return false;
            }
            // so like the board is counted from top to bottom, and 6 would be
            // the 7th row, where the pawn can move 2 squares
            if (piece.y < 6 && dist.y > 1) {
                return false;
            }
        } else {
            if (change.y != 1) {
                return false;
            }
            // so like the board is counted from top to bottom, and 2 would be
            // the 3rd row, where the pawn can move 2 squares
            if (piece.y > 2 && dist.y > 1) {
                return false;
            }
        }

        // wants to en passant
        if (change.x != 0 && boardState[goal.y][goal.x] == '.') {
            if (goal.y - change.y != enPassantY || goal.x != enPassantX) {
                return false;
            } else {
                killPiece({enPassantX, enPassantY});
            }
        }

        if (dist.y == 2) {
            enPassantX = goal.x;
            enPassantY = goal.y;
            enPassantPlayer = activePlayer;
            enPassantPossible = true;
        }
        break;

    }

    if (boardState[goal.y][goal.x] == '.') {
        movesWithoutCapture++;
    } else {
        movesWithoutCapture = 0;
    }

    if (movesWithoutCapture >= 50) {
        gameState = DRAW;
    }

    return true;
}


void Board::movePiece(chessVector piece, chessVector goal, bool changeActivePlayer) {
    unsigned char p = boardState[piece.y][piece.x];
    boardState[goal.y][goal.x] = p;
    boardState[piece.y][piece.x] = '.';
    if (changeActivePlayer) {
        activePlayer = !activePlayer;
    }
}

void Board::killPiece(chessVector piece) {
    boardState[piece.y][piece.x] = '.';
}


inline unsigned char Board::get(chessVector piece){
    return boardState[piece.y][piece.x];
}

inline void Board::set(chessVector piece, unsigned char type){
    boardState[piece.y][piece.x] = type;
}
