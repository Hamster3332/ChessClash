#include "board.h"
#include "chessVector.h"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>

#ifdef DEBUG
#define LOG_ENTER(x) logEnter(x)
#define LOG_EXIT(x)  logExit(x)
#define LOG_INFO(x)  logInfo(x)
#else
#define LOG_ENTER(x)
#define LOG_EXIT(x)
#define LOG_INFO(x)
#endif

Logger logger;


static ChessVector kingDirections[8] = {{1, 0}, {1, 1}, {0, 1}, {-1, 0}, {-1, -1}, {0, -1}, {-1, 1}, {1, -1}};
const ChessVector knightDirections[8] = {{2, 1}, {1, 2}, {-2, 1}, {-1, 2}, {2, -1}, {1, -2}, {-2, -1}, {-1, -2}};
const ChessVector rookDirections[4] = {{0,1},{0,-1},{1,0},{-1,0}};
const ChessVector bishopDirections[4] = {{1,1},{-1,-1},{1,-1},{-1,1}};
const ChessVector pawnDirections[2] = {{1,1},{-1,1}};

inline bool isPlayer(unsigned char c, enPlayers player) {
    return isBlack(c) == (player == enPlayers::Black);
}

enPlayers otherPlayer(enPlayers player) {
    return static_cast<enPlayers>(1 - player);
    logger.log("otherPlayer");
    if (player == enPlayers::White) return enPlayers::Black;
    return enPlayers::White;
}

enPlayers getPlayer(unsigned char c) {
    logger.log("getPlayer");
    if( isBlack(c)){
        return enPlayers::Black;
    }
    return enPlayers::White;
}

inline bool isBlack(unsigned char c) {
    return c & 32;// >> 5;
}

inline bool isWhite(unsigned char c) {
    return !(c & 32);// >> 5;
}

inline unsigned char pieceType(unsigned char p) {
    return p | 32;
}

int manhattanDistance(int pieceX, int pieceY, int goalX, int goalY) {
    logger.log("manhattanDistance");
    return std::abs(goalX - pieceX) + std::abs(goalY - pieceY);
};


std::size_t BoardHash::operator()(const BoardArray& b) const {
    std::size_t h = 0;
    for (const auto& row : b)
        for (unsigned char p : row)
            h = h * 31 + static_cast<std::size_t>(p);
    return h;
};



// checks the move independent of the piece type
bool Board::movePrecheck(const Move& move) {
    logger.log("movePrecheck");
    LOG_INFO("movePrecheck");
    unsigned char pieceChar = get(move.from);
    unsigned char goalChar = get(move.to);
    // work out of Town?
    // moving an empty square?
    if (goalChar == 0 || pieceChar == '.') return false;
    if (isBlack(pieceChar) == isBlack(goalChar) && goalChar != '.') return false; // capture your own pieces

    betweenMove(move);
    bool isAttacked;
    if (pieceType(pieceChar) == 'k') isAttacked = isAttackedByOpponent(move.to, getPlayer(pieceChar));
    else isAttacked = isAttackedByOpponent(KPos[isWhite(pieceChar)], getPlayer(pieceChar));
    betweenMove({move.to, move.from}, goalChar);
    if (isAttacked) return false;

    LOG_INFO("movePrecheck Out -> true");
    return true;
};

void Board::setBoardState(unsigned char newState[8][8]) {
    logger.log("setBoardState");
    LOG_ENTER("setBoardState");
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            boardState[y][x] = newState[y][x];
        }
    }
    castlesPossible[Castles::LONG_BLACK] = get({0,0}) == 'r';
    castlesPossible[Castles::SHORT_BLACK] = get({7,0}) == 'r';
    castlesPossible[Castles::LONG_WHITE] = get({0,7}) == 'R';
    castlesPossible[Castles::SHORT_WHITE] = get({7,7}) == 'R';
    LOG_EXIT("OK");
}

// Hurayy Finished
bool Board::isLegalMove(ChessVector piece, ChessVector goal) {
    logger.log("isLegalMove");
    LOG_ENTER("isLegalMove");
    unsigned char pieceChar = get(piece);
    unsigned char goalChar = get(goal);
    unsigned char pType = pieceType(pieceChar);

    bool pieceIsBlack = isBlack(pieceChar);
    bool pieceIsWhite = !pieceIsBlack;
    bool goalIsBlack = isBlack(goalChar);
    bool goalIsWhite = !goalIsBlack;

    bool isDifX = (piece.x != goal.x);
    bool isDifY = (piece.y != goal.y);

    ChessVector dist = goal.getDistance(piece);
    ChessVector changeVector = piece.getTo(goal);
    ChessVector change = changeVector.normToInt();

    if (gameState != ONGOING) {LOG_EXIT("Game over -> false"); return false;}
    if (activePlayer == isBlack(pieceChar)) {LOG_EXIT("Moved wrong Color -> false"); return false;}// not your turn
    if( !movePrecheck({piece, goal}) ) {LOG_EXIT("Failed Precheck -> false"); return false;}

    // check if still in Check
    betweenMove({piece, goal});
    bool isAttacked;
    if (pType == 'k') isAttacked = isAttackedByOpponent(goal, activePlayer);
    else isAttacked = isAttackedByOpponent(KPos[activePlayer], activePlayer);
    betweenMove({goal, piece}, goalChar);
    if (isAttacked) {LOG_EXIT("King Attacked after move -> false"); return false;}

    switch (pType) {
        // king
        case 'k':
        // wants to castle
        if (dist.x == 2 && dist.y == 0) {
            if (isAttackedByOpponent(piece, activePlayer) ||
                isAttackedByOpponent(piece + change, activePlayer) ||
                isAttackedByOpponent(goal, activePlayer) ||
                get(piece + change) != '.' ||
                get(goal) != '.') {
                LOG_EXIT("King can't castle!! -> false"); return false;
            }
            // short castles
            if (change.x == 1) {
                if(!castlesPossible[Castles::SHORT_BLACK + activePlayer]) {
                    LOG_EXIT("King cant Short castle!! -> false"); return false;
                }
            }
            // long castles
            else if (!castlesPossible[Castles::LONG_BLACK + activePlayer] ||
                isAttackedByOpponent(goal + change, activePlayer) ||
                get(goal + change) != '.') {
                LOG_EXIT("King cant Long castle!! -> false"); return false;
            }

        }
        else if (dist.x > 1 || dist.y > 1) { // moved further than 1 square
            LOG_EXIT("King can only move 1 square -> false"); return false;
        }
        break;

        // rook
        case 'r':
        // not orthogonal
        if (dist.x != 0 && dist.y != 0){
            LOG_EXIT("Rook can only move orthogonal -> false"); return false;
        }
        // check moves
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                LOG_EXIT("Rook cant jump -> false"); return false;
            }
        }
        break;

        // bishop
        case 'b':
        // not diagonal
        if (dist.x != dist.y) {
            LOG_EXIT("Bishop can only move diagonal -> false"); return false;
        }
        // check moves
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                LOG_EXIT("Bishop cant jump -> false"); return false;
            }
        }
        break;

        // queen
        case 'q':
        // not diagonal
        if (dist.x != dist.y) {
            // not orthogonal
            if (dist.x != 0 && dist.y != 0){
                LOG_EXIT("Queen can only move diagonal or orthogonal -> false");return false;
            }
        }
        // check moves
        for (int i = 1; i < dist.matinnatianDist(); ++i) {
            if (get(piece + (change * i)) != '.') {
                LOG_EXIT("Queen cant jump -> false"); return false;
            }
        }
        break;

        // knight
        case 'n':
        if (dist.manhattanDist() != 3 || dist.x > 2 || dist.y > 2) {
            LOG_EXIT("Knight is dumb :c -> false"); return false;
        }
        break;

        // pawn (oh no)
        case 'p':
        // wants to move 2 forward
        if (dist.y == 2) {
            // wants to move to side
            if (dist.x != 0) {LOG_EXIT("Pawn cant side step -> false"); return false;}
            // so like the board is counted from top to bottom, and 6 would be
            // the 7th row, where the pawn can move 2 squares
            // check if it can move 2
            if (pieceIsWhite && piece.y < 6) {LOG_EXIT("Pawn can only go 2 spaces under row 6 -> false"); return false;}
            if (pieceIsBlack && piece.y > 2) {LOG_EXIT("Pawn can only go 2 spaces over row 2 -> false"); return false;}

        // wants to move 1 forward
        } else if (dist.y == 1) {
            // moves in the correct directory
            if (pieceIsWhite && change.y != -1) {
                LOG_EXIT("Pawn moves in wrong direction -> false");
                return false;
            }
            else if (pieceIsBlack && change.y != 1) {
                LOG_EXIT("Pawn moves in wrong direction -> false");
                return false;
            }

            // wants to Side step
            if (dist.x > 1) {LOG_EXIT("Pawn still cant side step -> false"); return false;}
            // wants to advance-d lol
            else if (dist.x == 0 && get(goal) != '.') {LOG_EXIT("Pawn is blocked -> false"); return false;}
            // wants to capture
            else if (dist.x == 1 && get(goal) == '.'){
                if (!enPassantPossible || goal.sub(0, change.y) != enPassant)
                    {LOG_EXIT("Pawn cant en Passant -> false"); return false;}
            }
        } else {LOG_EXIT("Pawn Out -> true"); return false;}
        break;
    }
    LOG_EXIT("Checks Out -> true");
    return true;
}

std::vector<Move> Board::getLegalMoves(ChessVector piece) {
    logger.log("getLegalMoves");
    LOG_ENTER("Board::getLegalMoves");
    unsigned char pieceChar = get(piece);
    unsigned char pType = pieceType(pieceChar);
    std::vector<Move> moves = {};

    int pawnDirection = (2 * isBlack(pieceChar) - 1);
    int pawnDoubleMoveRow = 1 + isWhite(pieceChar) * 5;
    Move move = {{0, 0}, {0, 0}};
    switch (pType) {
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
            !isAttackedByOpponent(piece, getPlayer(pieceChar)) &&
            !isAttackedByOpponent(move.to, getPlayer(pieceChar)) &&
            !isAttackedByOpponent(piece.add(1, 0), getPlayer(pieceChar)))
            moves.emplace_back(move);
        move.to = piece.add(-2, 0);
        if (castlesPossible[LONG_BLACK + isWhite(pieceChar)]  && movePrecheck(move) &&
            get(piece.add(-1,0)) == '.' && get(move.to) == '.' && get(piece.add(-3,0)) == '.' &&
            !isAttackedByOpponent(piece, getPlayer(pieceChar)) &&
            !isAttackedByOpponent(piece.add(-1,0), getPlayer(pieceChar)) &&
            !isAttackedByOpponent(move.to, getPlayer(pieceChar)))
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
    LOG_EXIT("Finished -> moves");
    return moves;
}

bool Board::existsLegalMove(bool checkedForActivePlayer) {
    logger.log("existsLegalMove(bool)");
    LOG_ENTER("Board::existsLegalMove (bool)");
    unsigned char piece;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            piece = get({x,y});
            if (piece != '.' && piece != 0 && isWhite(piece) == checkedForActivePlayer) {
                if (existsLegalMove({x, y})){
                    LOG_EXIT("A legal move exists -> true");
                    return true;
                }
            }
        }
    }
    LOG_EXIT("No legal moves exist -> false");
    return false;
}

bool Board::existsLegalMove(ChessVector piece) {
    logger.log("existsLegalMove(ChessVector)");
    LOG_ENTER("Board::existsLegalMove (piece)");
    unsigned char pieceChar = get(piece);
    unsigned char pType = pieceType(pieceChar);
    int pawnDirection = (2 * isBlack(pieceChar) - 1);
    int pawnDoubleMoveRow = 1 + isWhite(pieceChar) * 5;
    Move move = {{0, 0}, {0, 0}};
    switch (pType) {
        case 'k':
        move = {piece, piece};
        for (ChessVector direction : kingDirections) {
            move = {piece, piece + direction};
            if (movePrecheck(move)) {
                LOG_EXIT("King normal exists -> true");
                return true;
            }
        }
        move.to = piece.add(2, 0);
        if (castlesPossible[SHORT_BLACK + isWhite(pieceChar)] && movePrecheck(move) &&
            get(move.to) == '.' && get(piece.add(1, 0)) == '.' &&
            !isAttackedByOpponent(piece, getPlayer(pieceChar)) &&
            !isAttackedByOpponent(move.to, getPlayer(pieceChar)) &&
            !isAttackedByOpponent(piece.add(1, 0), getPlayer(pieceChar)))
            {LOG_EXIT("King Short Castle exists -> true"); return true;}
        move.to = piece.add(-2, 0);
        if (castlesPossible[LONG_BLACK + isWhite(pieceChar)]  && movePrecheck(move) &&
            get(piece.add(-1,0)) == '.' && get(move.to) == '.' && get(piece.add(-3,0)) == '.' &&
            !isAttackedByOpponent(piece, getPlayer(pieceChar)) &&
            !isAttackedByOpponent(piece.add(-1,0), getPlayer(pieceChar)) &&
            !isAttackedByOpponent(move.to, getPlayer(pieceChar)))
            {LOG_EXIT("King Long Castle exists -> true");return true;}
        break;
        case 'r':
        for (ChessVector change : rookDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move))
                    {LOG_EXIT("Rook Move exists -> true"); return true;}
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
                if (movePrecheck(move))
                    {LOG_EXIT("Bishop Move exists -> true"); return true;}
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
                if (movePrecheck(move))
                    {LOG_EXIT("Queen Move exists diagunal -> true"); return true;}
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        for (ChessVector change : rookDirections) {
            for (int i = 1; i < 8; ++i) {
                Move move = {piece, piece + change * i};
                if (movePrecheck(move))
                    {LOG_EXIT("Queen Move exists ortagunal -> true"); return true;}
                if (get(move.to) != '.') {
                    break;
                }
            }
        }
        break;

        case 'n':
        for (ChessVector direction : knightDirections) {
            Move move = {piece, piece + direction};
            if (movePrecheck(move))
                {LOG_EXIT("Knight Move exists -> true"); return true;}
        }
        break;

        case 'p':
        Move move = {piece, piece.add(0, pawnDirection)};
        bool noPieceInFront = get(move.to) == '.';
        if (movePrecheck(move) && noPieceInFront)
            {LOG_EXIT("Pawn can move single -> true"); return true;}
        move.to = {piece.add(0, pawnDirection * 2)};
        if (movePrecheck(move) && get(move.to) == '.' && noPieceInFront && piece.y == pawnDoubleMoveRow )
            {LOG_EXIT("Pawn can move double -> true"); return true;}

        move.to = {piece.add(1, pawnDirection)};
        if (movePrecheck(move) && (get(move.to) != '.' || (enPassantPossible && enPassant == piece.add(1, 0))))
            {LOG_EXIT("Pawn can move diagunal right -> true"); return true;}

        move.to = {piece.add(-1, pawnDirection)};
        if (movePrecheck(move) && (get(move.to) != '.' || (enPassantPossible && enPassant == piece.add(-1, 0))))
            {LOG_EXIT("Pawn can move diagunal left -> true"); return true;}
        break;
    }
    LOG_EXIT("No moves Found -> false");
    return false;
}

bool Board::hasTurn(ChessVector piece) const{
    return isWhite(get(piece)) == activePlayer;
}
bool Board::isPromotion(Move move) const{
    return tolower(get(move.from)) == 'p' && (move.to.y == 0 || move.to.y == 7);
}

inline bool Board::isAttackedByOpponent(ChessVector square, enPlayers color) const {
    logger.log("isAttackedByOpponent");
    LOG_ENTER("Board::isAttackedByOpponent");
    ChessVector watchDog = {0,0};
    unsigned char p;
    // check K-Night
    for (const ChessVector& Direction : knightDirections) {
        p = get(square + Direction);
        if (!p) continue;
        if (isBlack(p) != color) continue;
        if (pieceType(p) == 'n') {
            LOG_EXIT("Knight attacks piece -> true");return true;
        }
    }
    // King
    for (const ChessVector& Direction : kingDirections) {
        p = get(square + Direction);
        if (!p) continue;
        if (isBlack(p) != color) continue;
        if (pieceType(p) == 'k') {
            LOG_EXIT("Is attacked by king -> true");
            return true;
        }
    }
    // check "THE ROOOOOOK!" - Levy Rozman
    for (const ChessVector& Direction : rookDirections) {
        watchDog = square + Direction;
        p = get(watchDog);
        while (p == '.') {
            watchDog = watchDog + Direction;
            p = get(watchDog);
        }
        if (!p) continue;
        if (isBlack(p) != color) continue;
        if (pieceType(p) == 'r' || pieceType(p) == 'q') {
            LOG_EXIT("Is attacked by orthogonal Stuff -> true");
            return true;
        }
    }
    // check Biscquit
    for (const ChessVector& Direction : bishopDirections) {
        watchDog = square + Direction;
        p = get(watchDog);
        while (p == '.') {
            watchDog = watchDog + Direction;
            p = get(watchDog);
        }
        if (!p) continue;
        if (isBlack(p) != color) continue;
        if (pieceType(p) == 'b' || pieceType(p) == 'q') {
            LOG_EXIT("Is attacked by diagonal Stuff -> true");
            return true;
        }
    }

    const int pawnDir = color ? -1 : 1;

    p = get(square + ChessVector{1, pawnDir});
    if (p && isBlack(p) == color && pieceType(p) == 'p'){
        LOG_EXIT("Is attacked by pawn -> true");
        return true;}

    p = get(square + ChessVector{-1, pawnDir});
    if (p && isBlack(p) == color && pieceType(p) == 'p'){
        LOG_EXIT("Is attacked by pawn -> true");
        return true;}

    LOG_EXIT("Isn't attacked");
    return false;
}

GameState Board::movePiece(ChessVector piece, ChessVector goal, bool changeActivePlayer, unsigned char promotedTo) {
    //logger.log("movePiece");
    //LOG_ENTER("Board::movePiece");
    unsigned char p = get(piece);
    if (get(goal) == '.') {
        movesWithoutCapture++;
    } else {
        movesWithoutCapture = 0;
    }

    // black moves + white moves is one move, so double
    if (movesWithoutCapture >= 100) {
        gameState = DRAW;
        LOG_EXIT("50 moves without a capture -> DRAW");
        return DRAW;
    }

    // en Passant
    enPassantPossible = false;

    if (pieceType(p) == 'p') {
        repetitionCount.clear();
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
            p = pieceType(promotedTo);
        }
    }

    else if (pieceType(p) == 'k') {
        KPos[activePlayer] = goal;
        castlesPossible[SHORT_BLACK + activePlayer] = false;
        castlesPossible[LONG_BLACK + activePlayer] = false;
        ChessVector moveVec = piece.getTo(goal);
        if (std::abs(moveVec.x) == 2) {
            betweenMove({{(moveVec.x == 2) * 7, piece.y}, goal - moveVec.normToInt()});
        }
    }

    else if (pieceType(p) == 'r') {
        if (piece.x == 7 && castlesPossible[SHORT_BLACK + activePlayer]) {
            castlesPossible[SHORT_BLACK + activePlayer] = false;
            repetitionCount.clear();

        }
        else if (piece.x == 0 && castlesPossible[LONG_BLACK + activePlayer]) {
            castlesPossible[LONG_BLACK + activePlayer] = false;
            repetitionCount.clear();
        }
    }

    betweenMove({piece, goal});

    if (changeActivePlayer) {
        if (activePlayer == enPlayers::White)activePlayer = enPlayers::Black;
        else activePlayer = enPlayers::White;
    }
    if(!existsLegalMove(activePlayer)){
        if (isAttackedByOpponent(KPos[activePlayer], activePlayer)){
            if (activePlayer == White){
                std::cout << "BLACK_WINS" << std::endl;
                gameState = BLACK_WINS;
                LOG_EXIT("Black won by checkmate -> BLACK_WINS");
                return BLACK_WINS;
            }
            std::cout << "WHITE_WINS" << std::endl;
            gameState = WHITE_WINS;
            LOG_EXIT("White won by checkmate -> WHITE_WINS");
            return WHITE_WINS;
        } else {
            std::cout << "DRAWWAWWWWWWWWWWW" << std::endl;
            gameState = DRAW;
            LOG_EXIT("Stalemate -> DRAW");
            return DRAW;
        }
    }

    repetitionCount[boardState]++;

    if (repetitionCount[boardState] >= 3) {
        std::cout << "Threefold repetition draw" << std::endl;
        gameState = DRAW;
        LOG_EXIT("Threefold repetition -> DRAW");
        return DRAW;
    }

    LOG_EXIT("Game goes on -> ONGOING");
    return ONGOING;
}

inline void Board::killPiece(ChessVector piece) {
    boardState[piece.y][piece.x] = '.';
}

/*
inline unsigned char Board::get(ChessVector piece) const {
    if (piece.x > 7 || piece.x < 0 || piece.y > 7 || piece.y < 0) return 0;
    return boardState[piece.y][piece.x];
}*/

inline unsigned char Board::get(ChessVector piece) const {
    //logger.log("get");
    if ((unsigned)piece.x > 7 || (unsigned)piece.y > 7) return 0;
    return boardState[piece.y][piece.x];
}

/*
 * moves a piece unrelated to possiblity
 * and returns (re)moved piece
 *
 */
inline unsigned char Board::betweenMove(Move move) {
    logger.log("betweenMove");
    unsigned char killed = get(move.to);
    set(move.to, get(move.from));
    set(move.from, '.');
    return killed;
}

inline unsigned char Board::betweenMove(Move move, unsigned char replacedBy) {
    //logger.log("betweenMove");
    unsigned char killed = get(move.to);
    set(move.to, get(move.from));
    set(move.from, replacedBy);
    return killed;
}

inline void Board::set(ChessVector piece, unsigned char type){
    boardState[piece.y][piece.x] = type;
}
