#include "board.h"
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

bool Board::isLegalMove(int xPiece, int yPiece, int xGoal, int yGoal) {
    unsigned char piece = boardState[yPiece][xPiece];
    unsigned char goalPiece = boardState[yGoal][xGoal];
    unsigned char pieceType = std::tolower(piece);
    
    bool pieceIsBlack = isLower(piece);
    bool pieceIsWhite = !pieceIsBlack;
    bool goalIsBlack = isLower(goalPiece);
    bool goalIsWhite = !goalIsBlack;
    
    bool isDifX = (xPiece != xGoal);
    bool isDifY = (yPiece != yGoal);
    int xDist = std::abs(xGoal - xPiece);
    int yDist = std::abs(yGoal - yPiece);
    int changeX = std::max(std::min(xGoal - xPiece, 1), -1);
    int changeY = std::max(std::min(yGoal - yPiece, 1), -1);
    
    if (piece == '.') return false; // moving an empty square?
    
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
        if (xDist > 1 || yDist > 1) // moved further than 1 square
            return false;
        break;
        
        // rook
        case 'r':
        // if y and x have changed -> not orthogonal
        if (isDifX && isDifY) {
            return false;
        }
        // moved left / right
        if (changeX != 0) {
            for (int x = xPiece + changeX; x != xGoal; x += changeX) {
                if (boardState[yPiece][x] != '.') {
                    return false;
                }
            }
        } // moved up / down
        else {
            for (int y = yPiece; y != yGoal; y += changeY) {
                if (boardState[y][xPiece] != '.') {
                    return false;
                }
            }
        }
        break;
            
        // bishop
        case 'b':
        // not diagonal
        if (xDist != yDist) {
            return false;
        }
        for (int i = 1; i < xDist; ++i) {
            if (boardState[yPiece + i * changeY][xPiece + i * changeX] != '.') {
                return false;
            }
        }
        break;
            
        // queen
        case 'q':
        if (isDifX && isDifY) {
            if (xDist != yDist) {
                return false;
            }
            for (int i = 1; i < xDist; ++i) {
                if (boardState[yPiece + i * changeY][xPiece + i * changeX] != '.') {
                    return false;
                }
            }
        } else if (changeY == 0) {
            for (int x = xPiece + changeX; x != xGoal; x += changeX) {
                if (boardState[yPiece][x] != '.') {
                    return false;
                }
            }
        } else {
            for (int y = yPiece + changeY; y != yGoal; y += changeY) {
                if (boardState[y][xPiece] != '.') {
                    return false;
                }
            }
        }
        break;
        
        // knight
        case 'n':
        if (xDist + yDist != 3 || xDist > 2 || yDist > 2) {
            return false;
        }
        break;
        
        // pawn (oh no)
        case 'p':
        if (xDist > 1 || yDist > 2 || changeY == 0) {
            return false;
        }
        if ((changeX != 0 && boardState[yGoal][xGoal] == '.' && !enPassantPossible) || 
            (changeX != 0 && yDist != 1)) {
            return false;
        }
        
        if (pieceIsWhite) {
            if (changeY != -1) {
                return false;
            }
            // so like the board is counted from top to bottom, and 6 would be
            // the 7th row, where the pawn can move 2 squares
            if (yPiece < 6 && yDist > 1) {
                return false;
            }
        } else {
            if (changeY != 1) {
                return false;
            }
            // so like the board is counted from top to bottom, and 2 would be
            // the 3rd row, where the pawn can move 2 squares
            if (yPiece > 2 && yDist > 1) {
                return false;
            }
        }
        
        // wants to en passant
        if (changeX != 0 && boardState[yGoal][xGoal] == '.') {
            if (yGoal - changeY != enPassantY || xGoal != enPassantX) {
                return false;
            } else {
                boardState[enPassantY][enPassantX] = '.';
            }
        }
        
        if (yDist == 2) {
            enPassantX = xGoal;
            enPassantY = yGoal;
            enPassantPlayer = activePlayer;
            enPassantPossible = true;
        }
        break;
        
    }
    
    
    boardState[yPiece][xPiece] = '.';
    boardState[yGoal][xGoal] = piece;
    activePlayer = !activePlayer;
    return true;
}

bool Board::kingMove(int pieceX, int pieceY, int goalX, int goalY) {
    int xDif = std::abs(goalX - pieceX);
    int yDif = std::abs(goalY - pieceY);
    
    if (xDif > 1 || yDif > 1) {
        return false;
    }
    return true;
}
