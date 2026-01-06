#include "board.h"
#include "chessVector.h"
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


GameState Board::movePiece(chessVector piece, chessVector goal, bool changeActivePlayer) {
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

    set(goal, p);
    set(piece, '.');
    if (changeActivePlayer) {
        activePlayer = !activePlayer;
    }
    return ONGOING;
}

inline void Board::killPiece(chessVector piece) {
    boardState[piece.y][piece.x] = '.';
}


inline unsigned char Board::get(chessVector piece){
    return boardState[piece.y][piece.x];
}

inline void Board::set(chessVector piece, unsigned char type){
    boardState[piece.y][piece.x] = type;
}
