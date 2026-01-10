#include "board.h"
#include "chessVector.h"
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>
#include "logger.h"


inline bool isBlack(unsigned char c) {
    return c & 32;// >> 5;
}

inline bool isWhite(unsigned char c) {
    return !(c & 32);// >> 5;
}

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY) {
    return std::abs(goalX - pieceX) + std::abs(goalY - pieceY);
};

// checks the move independent of the piece type
bool Board::movePrecheck(Move move) {
    logEnter("movePrecheck");
    unsigned char pieceChar = get(move.from);
    unsigned char goalChar = get(move.to);

    if (goalChar == 0) {logExit("Doesn't Work out of Town -> false"); return false;} // work out of Town?
    if (pieceChar == '.') {logExit("Can't Move Empty Square -> false"); return false;} // moving an empty square?
    if (isBlack(pieceChar) == isBlack(goalChar) && goalChar != '.')
        {logExit("Can't capture your own pieces -> false"); return false;} // capture your own pieces

    betweenMove(move);
    bool isAttacked;
    if (std::tolower(pieceChar) == 'k') isAttacked = isAtackedByOpponent(move.to, isWhite(pieceChar));
    else isAttacked = isAtackedByOpponent(KPos[isWhite(pieceChar)], isWhite(pieceChar));
    betweenMove({move.to, move.from}, goalChar);
    if (isAttacked) {logExit("King Attacked after move -> false"); return false;}

    logExit("Checks Out -> true");
    return true;
};

void Board::setBoardState(unsigned char newState[8][8]) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            boardState[y][x] = newState[y][x];
        }
    }
    castlesPossible[Castles::LONG_BLACK] = get({0,0}) == 'r';
    castlesPossible[Castles::SHORT_BLACK] = get({7,0}) == 'r';
    castlesPossible[Castles::LONG_WHITE] = get({0,7}) == 'R';
    castlesPossible[Castles::SHORT_WHITE] = get({7,7}) == 'R';
}

// Hurayy Finished
bool Board::isLegalMove(ChessVector piece, ChessVector goal) {
    logEnter("isLegalMove");
    unsigned char pieceChar = get(piece);
    unsigned char goalChar = get(goal);
    unsigned char pieceType = std::tolower(pieceChar);

    bool pieceIsBlack = isBlack(pieceChar);
    bool pieceIsWhite = !pieceIsBlack;
    bool goalIsBlack = isBlack(goalChar);
    bool goalIsWhite = !goalIsBlack;

    bool isDifX = (piece.x != goal.x);
    bool isDifY = (piece.y != goal.y);

    ChessVector dist = goal.getDistance(piece);
    ChessVector changeVector = piece.getTo(goal);
    ChessVector change = changeVector.normToInt();

    if (gameState != ONGOING) {logExit("Game over -> false"); return false;}
    if (activePlayer == isBlack(pieceChar)) {logExit("Moved wrong Color -> false"); return false;}// not your turn
    if( !movePrecheck({piece, goal}) ) {logExit("Failed Precheck -> false"); return false;}

    // check if still in Check
    betweenMove({piece, goal});
    bool isAttacked;
    if (pieceType == 'k') isAttacked = isAtackedByOpponent(goal, activePlayer);
    else isAttacked = isAtackedByOpponent(KPos[activePlayer], activePlayer);
    betweenMove({goal, piece}, goalChar);
    if (isAttacked) {logExit("King Attacked after move -> false"); return false;}

    switch (pieceType) {
        // king
        case 'k':
        // wants to castle
        if (dist.x == 2 && dist.y == 0) {
            if (isAtackedByOpponent(piece, activePlayer) ||
                isAtackedByOpponent(piece + change, activePlayer) ||
                isAtackedByOpponent(goal, activePlayer) ||
                get(piece + change) != '.' ||
                get(goal) != '.') {
                logExit("King can't castle!! -> false"); return false;
            }
            // short castles
            if (change.x == 1) {
                if(!castlesPossible[Castles::SHORT_BLACK + activePlayer]) {
                    logExit("King cant Short castle!! -> false"); return false;
                }
            }
            // long castles
            else if (!castlesPossible[Castles::LONG_BLACK + activePlayer] ||
                isAtackedByOpponent(goal + change, activePlayer) ||
                get(goal + change) != '.') {
                logExit("King cant Long castle!! -> false"); return false;
            }

        }
        else if (dist.x > 1 || dist.y > 1) { // moved further than 1 square
            logExit("King can only move 1 square -> false"); return false;
        }
        break;

        // rook
        case 'r':
        // not orthogonal
        if (dist.x != 0 && dist.y != 0){
            logExit("Rook can only move orthogonal -> false"); return false;
        }
        // check moves
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                logExit("Rook cant jump -> false"); return false;
            }
        }
        break;

        // bishop
        case 'b':
        // not diagonal
        if (dist.x != dist.y) {
            logExit("Bishop can only move diagonal -> false"); return false;
        }
        // check moves
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                logExit("Bishop cant jump -> false"); return false;
            }
        }
        break;

        // queen
        case 'q':
        // not diagonal
        if (dist.x != dist.y) {
            // not orthogonal
            if (dist.x != 0 && dist.y != 0){
                logExit("Queen can only move diagonal or orthogonal -> false");return false;
            }
        }
        // check moves
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                logExit("Queen cant jump -> false"); return false;
            }
        }
        break;

        // knight
        case 'n':
        if (dist.manhattanDist() != 3 || dist.x > 2 || dist.y > 2) {
            logExit("Knight is dumb :c -> false"); return false;
        }
        break;

        // pawn (oh no)
        case 'p':
        // wants to move 2 forward
        if (dist.y == 2) {
            // wants to move to side
            if (dist.x != 0) {logExit("Pawn cant side step -> false"); return false;}
            // so like the board is counted from top to bottom, and 6 would be
            // the 7th row, where the pawn can move 2 squares
            // check if it can move 2
            if (pieceIsWhite && piece.y < 6) {logExit("Pawn can only go 2 spaces under row 6 -> false"); return false;}
            if (pieceIsBlack && piece.y > 2) {logExit("Pawn can only go 2 spaces over row 2 -> false"); return false;}

        // wants to move 1 forward
        } else if (dist.y == 1) {
            // moves in the correct directory
            if (pieceIsWhite && change.y != -1) {
                logExit("Pawn moves in wrong direction -> false");
                return false;
            }
            else if (pieceIsBlack && change.y != 1) {
                logExit("Pawn moves in wrong direction -> false");
                return false;
            }

            // wants to Side step
            if (dist.x > 1) {logExit("Pawn still cant side step -> false"); return false;}
            // wants to advance-d lol
            else if (dist.x == 0 && get(goal) != '.') {logExit("Pawn is blocked -> false"); return false;}
            // wants to capture
            else if (dist.x == 1 && get(goal) == '.'){
                if (!enPassantPossible || goal.sub(0, change.y) != enPassant)
                    {logExit("Pawn cant en Passant -> false"); return false;}
            }
        } else {logExit("Pawn Out -> true"); return false;}
        break;
    }
    logExit("Checks Out -> true");
    return true;
}

std::vector<Move> Board::getLegalMoves(ChessVector piece) {
    unsigned char pieceChar = get(piece);
    unsigned char pieceType = std::tolower(pieceChar);
    std::vector<Move> moves = {};
    static ChessVector kingDirections[8] = {{1, 0}, {1, 1}, {0, 1}, {-1, 0}, {-1, -1}, {0, -1}, {-1, 1}, {1, -1}};
    static ChessVector rookDirections[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    static ChessVector knightDirections[8] = {{2, 1}, {1, 2}, {-2, 1}, {-1, 2}, {2, -1}, {1, -2}, {-2, -1}, {-1, -2}};
    static ChessVector bishopDirections[4] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    int pawnDirection = (2 * isBlack(pieceChar) - 1);
    int pawnDoubleMoveRow = 1 + isWhite(pieceChar) * 5;
    Move move = {{0, 0}, {0, 0}};
    switch (pieceType) {
        case 'k':
        move = {piece, piece};
        for (ChessVector direction : kingDirections) {
            move = {piece, piece + direction};
            if (movePrecheck(move)) {
                moves.emplace_back(move);
            }
        }
        move.to = piece.add(2, 0);
        if (castlesPossible[SHORT_BLACK + isWhite(pieceChar)] && movePrecheck(move) &&
            get(move.to) == '.' && get(piece.add(1, 0)) == '.' &&
            !isAtackedByOpponent(piece, isWhite(pieceChar)) &&
            !isAtackedByOpponent(move.to, isWhite(pieceChar)) &&
            !isAtackedByOpponent(piece.add(1, 0), isWhite(pieceChar)))
            moves.emplace_back(move);
        move.to = piece.add(-2, 0);
        if (castlesPossible[LONG_BLACK + isWhite(pieceChar)]  && movePrecheck(move) &&
            get(piece.add(-1,0)) == '.' && get(move.to) == '.' && get(piece.add(-3,0)) == '.' &&
            !isAtackedByOpponent(piece, isWhite(pieceChar)) &&
            !isAtackedByOpponent(piece.add(-1,0), isWhite(pieceChar)) &&
            !isAtackedByOpponent(move.to, isWhite(pieceChar)))
            moves.emplace_back(move);
        break;
        case 'r':
        for (ChessVector change : rookDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    moves.emplace_back(move);
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        break;

        case 'b':
        for (ChessVector change : bishopDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    moves.emplace_back(move);
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        break;

        case 'q':
        for (ChessVector change : bishopDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    moves.emplace_back(move);
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        for (ChessVector change : rookDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    moves.emplace_back(move);
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        break;

        case 'n':
        for (ChessVector direction : knightDirections) {
            Move move = {piece, piece + direction};
            if (movePrecheck(move)) {
                moves.emplace_back(move);
            }
        }
        break;

        case 'p':
        Move move = {piece, piece.add(0, pawnDirection)};
        bool noPieceInFront = get(move.to) == '.';
        if (movePrecheck(move) && noPieceInFront)
            moves.emplace_back(move);
        move.to = {piece.add(0, pawnDirection * 2)};
        if (movePrecheck(move) && get(move.to) == '.' && noPieceInFront && piece.y == pawnDoubleMoveRow )
            moves.emplace_back(move);

        move.to = {piece.add(1, pawnDirection)};
        if (movePrecheck(move) && (get(move.to) != '.' || (enPassantPossible && enPassant == piece.add(1, 0)))) {
            moves.emplace_back(move);
        }

        move.to = {piece.add(-1, pawnDirection)};
        if (movePrecheck(move) && (get(move.to) != '.' || (enPassantPossible && enPassant == piece.add(-1, 0)))) {
            moves.emplace_back(move);
        }

        break;
    }
    return moves;
}

bool Board::existsLegalMove(bool checkedForActivePlayer) {
    unsigned char piece;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            piece = get({x,y});
            if (piece != '.' && piece != 0 && isWhite(piece) == checkedForActivePlayer) {
                if (existsLegalMove({x, y})){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Board::existsLegalMove(ChessVector piece) {
    unsigned char pieceChar = get(piece);
    unsigned char pieceType = std::tolower(pieceChar);
    static ChessVector kingDirections[8] = {{1, 0}, {1, 1}, {0, 1}, {-1, 0}, {-1, -1}, {0, -1}, {-1, 1}, {1, -1}};
    static ChessVector rookDirections[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    static ChessVector knightDirections[8] = {{2, 1}, {1, 2}, {-2, 1}, {-1, 2}, {2, -1}, {1, -2}, {-2, -1}, {-1, -2}};
    static ChessVector bishopDirections[4] = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}};
    int pawnDirection = (2 * isBlack(pieceChar) - 1);
    int pawnDoubleMoveRow = 1 + isWhite(pieceChar) * 5;
    Move move = {{0, 0}, {0, 0}};
    switch (pieceType) {
        case 'k':
        move = {piece, piece};
        for (ChessVector direction : kingDirections) {
            move = {piece, piece + direction};
            if (movePrecheck(move)) {
                return true;
            }
        }
        move.to = piece.add(2, 0);
        if (castlesPossible[SHORT_BLACK + isWhite(pieceChar)] && movePrecheck(move) &&
            get(move.to) == '.' && get(piece.add(1, 0)) == '.' &&
            !isAtackedByOpponent(piece, isWhite(pieceChar)) &&
            !isAtackedByOpponent(move.to, isWhite(pieceChar)) &&
            !isAtackedByOpponent(piece.add(1, 0), isWhite(pieceChar)))
            return true;
        move.to = piece.add(-2, 0);
        if (castlesPossible[LONG_BLACK + isWhite(pieceChar)]  && movePrecheck(move) &&
            get(piece.add(-1,0)) == '.' && get(move.to) == '.' && get(piece.add(-3,0)) == '.' &&
            !isAtackedByOpponent(piece, isWhite(pieceChar)) &&
            !isAtackedByOpponent(piece.add(-1,0), isWhite(pieceChar)) &&
            !isAtackedByOpponent(move.to, isWhite(pieceChar)))
            return true;
        break;
        case 'r':
        for (ChessVector change : rookDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    return true;
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        break;

        case 'b':
        for (ChessVector change : bishopDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    return true;
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        break;

        case 'q':
        for (ChessVector change : bishopDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    return true;
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        for (ChessVector change : rookDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move)) {
                    return true;
                }
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        break;

        case 'n':
        for (ChessVector direction : knightDirections) {
            Move move = {piece, piece + direction};
            if (movePrecheck(move)) {
                return true;
            }
        }
        break;

        case 'p':
        Move move = {piece, piece.add(0, pawnDirection)};
        bool noPieceInFront = get(move.to) == '.';
        if (movePrecheck(move) && noPieceInFront)
            return true;
        move.to = {piece.add(0, pawnDirection * 2)};
        if (movePrecheck(move) && get(move.to) == '.' && noPieceInFront && piece.y == pawnDoubleMoveRow )
            return true;

        move.to = {piece.add(1, pawnDirection)};
        if (movePrecheck(move) && (get(move.to) != '.' || (enPassantPossible && enPassant == piece.add(1, 0)))) {
            return true;
        }

        move.to = {piece.add(-1, pawnDirection)};
        if (movePrecheck(move) && (get(move.to) != '.' || (enPassantPossible && enPassant == piece.add(-1, 0)))) {
            return true;
        }
        break;
    }

    return false;

}

bool Board::hasTurn(ChessVector piece){
    return isWhite(get(piece)) == activePlayer;
}
bool Board::isPromotion(Move move){
    return tolower(get(move.from)) == 'p' && (move.to.y == 0 || move.to.y == 7);
}

bool Board::isAtackedByOpponent(ChessVector square, bool checkedForActivePlayer) {
    ChessVector watchDog = {0,0};
    unsigned char p;
    // check K-Night
    ChessVector knightDirections[8]={{1,2},{1,-2},{-1,-2},{-1,2},{2,1},{-2,1},{-2,-1},{-2,1}};for(ChessVector Direction:knightDirections){p=get(square+Direction);if(p==0)continue;if(activePlayer==isBlack(p)&&std::tolower(p)=='n')return true;}
    // check "THE ROOOOOOK!" - Levy Rozman
    ChessVector rookDirections[4] = {{0,1},{0,-1},{1,0},{-1,0}};
    for (ChessVector Direction : rookDirections) {
        watchDog = square + Direction;
        while (get(watchDog) == '.') {
            watchDog = watchDog + Direction;
        }
        p = get(watchDog);
        if (p == 0) continue;
        if (checkedForActivePlayer == isBlack(p)) {
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
        if (checkedForActivePlayer == isBlack(p)) {
            if (std::tolower(p) == 'b') return true;
            if (std::tolower(p) == 'q') return true;
        }
    }
    // King
    for (ChessVector Direction : rookDirections) {
        p = get(square + Direction);
        if (p == 0) continue;
        if (checkedForActivePlayer == isBlack(p) && std::tolower(p) == 'k') return true;
    }
    for (ChessVector Direction : BishopDirections) {
        p = get(square + Direction);
        if (p == 0) continue;
        if (checkedForActivePlayer == isBlack(p) && std::tolower(p) == 'k') return true;
    }
    // Pawn (oh nooooo)
    ChessVector PawnDirections[2] = {{1,1},{-1,1}};
    for (ChessVector Direction : PawnDirections) {
        Direction.y = 1 - checkedForActivePlayer * 2;
        p = get(square + Direction);
        if (p == 0) continue;
        if (checkedForActivePlayer == isBlack(p) && std::tolower(p) == 'p') return true;
    }

    return false;
}

GameState Board::movePiece(ChessVector piece, ChessVector goal, bool changeActivePlayer, unsigned char promotedTo) {
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
        if (goal.y == 0 && isWhite(p)) {
            p = std::toupper(promotedTo);
        } else if (goal.y == 7 && isBlack(p)) {
            p = std::tolower(promotedTo);
        }
    }

    else if (std::tolower(p) == 'k') {
        KPos[activePlayer] = goal;
        castlesPossible[SHORT_BLACK + activePlayer] = false;
        castlesPossible[LONG_BLACK + activePlayer] = false;
        ChessVector moveVec = piece.getTo(goal);
        if (std::abs(moveVec.x) == 2) {
            betweenMove({{(moveVec.x == 2) * 7, piece.y}, goal - moveVec.normToInt()});
        }
    }

    else if (std::tolower(p) == 'r') {
        if (piece.x == 7) castlesPossible[SHORT_BLACK + activePlayer] = false;
        else if (piece.x == 0) castlesPossible[LONG_BLACK + activePlayer] = false;
    }

    set(goal, p);
    set(piece, '.');
    if (changeActivePlayer) {
        activePlayer = !activePlayer;
    }
    if(!existsLegalMove(activePlayer)){
        if (isAtackedByOpponent(KPos[activePlayer], activePlayer)){
            if (activePlayer == White){
                std::cout << "BLACK_WINS" << std::endl;
                gameState = BLACK_WINS;
                return BLACK_WINS;
            }
            std::cout << "WHITE_WINS" << std::endl;
            gameState = WHITE_WINS;
            return WHITE_WINS;
        } else {
            std::cout << "DRAWWAWWWWWWWWWWW" << std::endl;
            gameState = DRAW;
            return DRAW;
        }
    }

    return ONGOING;
}

inline void Board::killPiece(ChessVector piece) {
    boardState[piece.y][piece.x] = '.';
}


unsigned char Board::get(ChessVector piece) {
    if (piece.x > 7 || piece.x < 0
        || piece.y > 7 || piece.y < 0) {
        return 0;
    }
    return boardState[piece.y][piece.x];
}

/*
 * moves a piece unrelated to possiblity
 * and returns (re)moved piece
 *
 */
 inline unsigned char Board::betweenMove(Move move) {
     unsigned char killed = get(move.to);
     set(move.to, get(move.from));
     set(move.from, '.');
     return killed;
 }

 inline unsigned char Board::betweenMove(Move move, unsigned char replacedBy) {
     unsigned char killed = get(move.to);
     set(move.to, get(move.from));
     set(move.from, replacedBy);
     return killed;
 }

inline void Board::set(ChessVector piece, unsigned char type){
    boardState[piece.y][piece.x] = type;
}
