#include "board.h"
#include "chessVector.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <ostream>


bool isLower(unsigned char c) {
    return c & 32;// >> 5;
}

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY) {
    return std::abs(goalX - pieceX) + std::abs(goalY - pieceY);
};

bool Board::isLegalMove(ChessVector piece, ChessVector goal) {
    unsigned char pieceChar = get(piece);
    unsigned char goalChar = get(goal);
    unsigned char pieceType = std::tolower(pieceChar);

    bool pieceIsBlack = isLower(pieceChar);
    bool pieceIsWhite = !pieceIsBlack;
    bool goalIsBlack = isLower(goalChar);
    bool goalIsWhite = !goalIsBlack;

    bool isDifX = (piece.x != goal.x);
    bool isDifY = (piece.y != goal.y);

    ChessVector dist = goal.getDistance(piece);
    ChessVector changeVector = piece.getTo(goal);
    ChessVector change = changeVector.normToInt();

    if (gameState != ONGOING) return false; // game is over

    if (pieceChar == '.') return false; // moving an empty square?

    if (pieceIsBlack && goalIsBlack && goalChar != '.') return false; // capture your own pieces
    if (pieceIsWhite && goalIsWhite && goalChar != '.') return false;

    if (activePlayer && pieceIsBlack) return false; // not your turn
    if (!activePlayer && pieceIsWhite) return false;

    if (!isDifX && !isDifY) return false; // piece captures itself

    // check if still in Check
    set(piece, '.');
    set(goal, pieceChar);
    bool isAttacked = isAtackedByOpponent(KPos[activePlayer]);
    set(piece, pieceChar);
    set(goal, goalChar);
    if (isAttacked) return false;

    switch (pieceType) {
        // king
        case 'k':
        if (isAtackedByOpponent(goal)){return false;}
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
        // check moves
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
        // check moves
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
        // check moves
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
        // wants to move 2 forward
        if (dist.y == 2) {
            // wants to move to side
            if (dist.x != 0) return false;
            // so like the board is counted from top to bottom, and 6 would be
            // the 7th row, where the pawn can move 2 squares
            // check if it can move 2
            if (pieceIsWhite && piece.y < 6) return false;
            if (pieceIsBlack && piece.y > 2) return false;

        // wants to move 1 forward
        } else if (dist.y == 1) {
            // moves in the correct directory
            if (pieceIsWhite && change.y != -1) {
                return false;
            }
            else if (pieceIsBlack && change.y != 1) {
                return false;
            }

            // wants to Side step
            if (dist.x > 1) return false;
            // wants to advance-d lol
            else if (dist.x == 0 && get(goal) != '.') return false;
            // wants to capture
            else if (dist.x == 1 && get(goal) == '.'){
                if (!enPassantPossible || goal.sub(0, change.y) != enPassant)
                    return false;
            }
        } else return false;
        break;
    }

    return true;
}

bool Board::isAtackedByOpponent(ChessVector square) {
    ChessVector watchDog = {0,0};
    unsigned char p;
    // check K-Night
    ChessVector knightDirections[8]={{1,2},{1,-2},{-1,-2},{-1,2},{2,1},{-2,1},{-2,-1},{-2,1}};for(ChessVector Direction:knightDirections){p=get(square+Direction);if(p==0)continue;if(activePlayer==isLower(p)&&std::tolower(p)=='n')return true;}
    // check "THE ROOOOOOK!" - Levy Rozman
    ChessVector rookDirections[4] = {{0,1},{0,-1},{1,0},{-1,0}};
    for (ChessVector Direction : rookDirections) {
        watchDog = square + Direction;
        while (get(watchDog) == '.') {
            watchDog = watchDog + Direction;
        }
        p = get(watchDog);
        if (p == 0) continue;
        if (activePlayer == isLower(p)) {
            if (std::tolower(p) == 'r') return true;
            if (std::tolower(p) == 'q') return true;
        }
    }
    // check Biscquit
    ChessVector BishopDirections[4] = {{1,1},{-1,-1},{1,-1},{-1,1}};
    for (ChessVector Direction : BishopDirections) {
        watchDog = square + Direction;
        while (get(watchDog) == '.') {
            watchDog = watchDog + Direction;
        }
        p = get(watchDog);
        if (p == 0) continue;
        if (activePlayer == isLower(p)) {
            if (std::tolower(p) == 'b') return true;
            if (std::tolower(p) == 'q') return true;
        }
    }
    // King
    for (ChessVector Direction : rookDirections) {
        p = get(square + Direction);
        if (p == 0) continue;
        if (activePlayer == isLower(p) && std::tolower(p) == 'k') return true;
    }
    for (ChessVector Direction : BishopDirections) {
        p = get(square + Direction);
        if (p == 0) continue;
        if (activePlayer == isLower(p) && std::tolower(p) == 'k') return true;
    }
    // Pawn (oh nooooo)
    ChessVector PawnDirections[2] = {{1,1},{-1,1}};
    for (ChessVector Direction : PawnDirections) {
        Direction.y = 1 - activePlayer * 2;
        p = get(square + Direction);
        if (p == 0) continue;
        if (activePlayer == isLower(p) && std::tolower(p) == 'p') return true;
    }

    return false;
}

GameState Board::movePiece(ChessVector piece, ChessVector goal, bool changeActivePlayer) {
    unsigned char p = get(piece);
    if (get(goal) == '.') {
        movesWithoutCapture++;
    } else {
        movesWithoutCapture = 0;
    }

    if (movesWithoutCapture >= 50) {
        gameState = DRAW;
        return DRAW;
    }

    // en Passant
    enPassantPossible = false;

    if (std::tolower(p) == 'p') {
        if (piece.getDistance(goal).x == 1 && get(goal) == '.') {
            killPiece(enPassant);
        }
        if (piece.getDistance(goal).y == 2) {
            enPassant = goal;
            enPassantPossible = true;
        }
    }

    if (std::tolower(p) == 'k') {
        KPos[activePlayer] = goal;
    }

    set(goal, p);
    set(piece, '.');
    if (changeActivePlayer) {
        activePlayer = !activePlayer;
    }
    return ONGOING;
}

inline void Board::killPiece(ChessVector piece) {
    boardState[piece.y][piece.x] = '.';
}


inline unsigned char Board::get(ChessVector piece) {
    if (piece.x > 7 || piece.x < 0
        || piece.y > 7 || piece.y < 0) {
        return 0;
    }
    return boardState[piece.y][piece.x];
}

inline void Board::set(ChessVector piece, unsigned char type){
    boardState[piece.y][piece.x] = type;
}
