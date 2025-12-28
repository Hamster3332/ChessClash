#include "board.h"
#include <cctype>
#include <cstdlib>


bool isLower(unsigned char c) {
    return c & 32;// >> 5; 
    // so uhm 32 has this 1 bit which changes in the ascii table
    // when letters go from upper to lowercase. Bitshift to make the result 0 or 1 instead of 32
    // when true.
}

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY) {
    return std::abs(goalX - pieceX) + std::abs(goalY - pieceY);
};


bool Board::isLegalMove(int pieceX, int pieceY, int goalX, int goalY) {
    unsigned char piece = boardState[pieceY][pieceX];
    unsigned char goalPiece = boardState[goalY][goalX];
    unsigned char pieceType = std::tolower(piece);
    
    bool pieceIsBlack = isLower(piece);
    bool pieceIsWhite = !pieceIsBlack;
    bool goalIsBlack = isLower(goalPiece);
    bool goalIsWhite = !goalIsBlack;
    
    bool isDifX = (pieceX != goalX);
    bool isDifY = (pieceY != goalY);
    int xDif = std::abs(goalX - pieceX);
    int yDif = std::abs(goalY - pieceY);
    
    if (piece == '.') return false; // moving an empty square?
    
    if (pieceIsBlack && goalIsBlack && goalPiece != '.') return false; // capture your own pieces
    if (pieceIsWhite && goalIsWhite && goalPiece != '.') return false;
    
    if (activePlayer && pieceIsBlack) return false; // not your turn
    if (!activePlayer && pieceIsWhite) return false;
    
    if (!isDifX && !isDifY) return false; // piece captures itself
    
    
    if (pieceType == 'k') { // king
        if (xDif > 1 || yDif > 1) // moved further than 1 square
            return false;
    } else if (pieceType == 'r') { // rook
        if (isDifX && isDifY) {
            return false;
        }
        if (pieceX != goalX) {
            if (pieceX < goalX) {
                for (int i = pieceX + 1; i < goalX; ++i) {
                    if (boardState[pieceY][i] != '.') {
                        return false;
                    }
                }
            } else {
                for (int i = pieceX - 1; i > goalX; --i) {
                    if (boardState[pieceY][i] != '.') {
                        return false;
                    }
                }
            }
        } else {
            if (pieceY < goalY) {
                for (int i = pieceY + 1; i < goalY; ++i) {
                    if (boardState[i][pieceX] != '.') {
                        return false;
                    }
                }
            } else {
                for (int i = pieceY - 1; i > goalY; --i) {
                    if (boardState[i][pieceX] != '.') {
                        return false;
                    }
                }
            }
        }
    }
    
    
    boardState[pieceY][pieceX] = '.';
    boardState[goalY][goalX] = piece;
    activePlayer = !activePlayer;
    return true;
}
