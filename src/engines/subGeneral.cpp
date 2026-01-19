#include "subGeneral.h"
#include "board.h"
#include "chessVector.h"
#include <cctype>
#include <cstdlib>

float getPieceValue( unsigned char piece){
    switch ( tolower(piece)) {
        case 'q':
        return 1.0f;
        case 'p':
        return 0.1f;
        case 'n':
        return 0.3f;
        case 'b':
        return 0.3f;
        case 'r':
        return 0.5f;
        case 'k':
        return 100.0f;
    }
    return 0.0f;
}

inline float betterGetPieceValue(unsigned char piece){
    switch (tolower(piece)) {
        case 'q':
        return 0.9f;
        case 'p':
        return 0.1f;
        case 'n':
        return 0.3f;
        case 'b':
        return 0.3f;
        case 'r':
        return 0.5f;
        case 'k':
        return 1000.0f;
    }
    return 0.0f;
}

float sub_E_BadSchoolKid(Move mv, Board* board, enPlayers color) {
    float result = 0.0f;
    
    int numPossibleMoves = board->getLegalMoves(mv.from).size();

    float pieceVal = betterGetPieceValue(board->get(mv.from));
    float goalVal = betterGetPieceValue(board->get(mv.to));

    bool isAttacked = board->isAttackedByOpponent(mv.from, color);
    bool goalIsAttacked = board->isAttackedByOpponent(mv.to, color);
    bool isCapture = board->get(mv.to) != '.';

    unsigned char pType = tolower(board->get(mv.from));

    unsigned char p1 = board->betweenMove(mv);
    int numDevelopMoves = board->getLegalMoves(mv.to).size();
    result += (float)(numDevelopMoves - numPossibleMoves) * 0.001f;
    bool canMove = board->existsLegalMove(otherPlayer(color));
    bool isCheck = board->isAttackedByOpponent(board->KPos[otherPlayer(color)], otherPlayer(color));
    unsigned char peike = '.';
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            peike = board->get({x, y});
            if (peike != '.' && isBlack(peike) == color) {
                for (Move mv1 : board->getLegalMoves({x, y})) {

                    if (board->get(mv1.to) != '.' && !board->isAttackedByOpponent(mv1.to, otherPlayer(color))) {
                        result -= betterGetPieceValue(board->get(mv1.to));
                    }
                    unsigned char p2 = board->betweenMove(mv1);
                    bool iCanMove = board->existsLegalMove(color);
                    bool iAmInCheck = board->isAttackedByOpponent(board->KPos[color], color);
                    board->betweenMove({mv1.to, mv1.from}, p2);
                    if (!iCanMove) {
                        result -= 10000;
                        break;
                    }
                    if (iAmInCheck) {
                        result -= 0.001;
                    }
                }
            }
        }
    }
    if (!canMove && isCheck) {//checkimate
        result += 300000;
    }
    if (!canMove && !isCheck) {//draw5
        result += -30000;
    }

    board->betweenMove({mv.to, mv.from}, p1);
    
    if (pType == 'k') {
        result -= 0.01;
    } else if (pType == 'p') {
        result += 0.01;
    }


    if (goalIsAttacked) {
        result -= pieceVal;
    }
    else if (isAttacked) {
        result += pieceVal;
    }

    result += goalVal;
    result += (float)rand() / (float)RAND_MAX * 0.000001;


    return result;
}

float sub_E_Capture(Move mv, Board* board, enPlayers color) {
    float result = 0.0f;
    if (getPlayer(board->get(mv.to)) != color) {
        result = getPieceValue(board->get(mv.to));
    }
    if (board->isAttackedByOpponent(mv.to, color)) {
        result -= getPieceValue(board->get(mv.from)) - 0.1f;
    }
    return result;
}

float sub_E_Bandit(Move mv, Board* board, enPlayers color) {
    float result = 1.0f;
    result -= board->KPos[!color].getDistance(mv.to).absoluteDist() / 16.0f;
    return result;
}

float sub_E_UnBandit(Move mv, Board* board, enPlayers color) {
    float result = 0.0f;
    result += board->KPos[!color].getDistance(mv.to).absoluteDist() / 16.0f;
    return result;
}

float sub_E_Cuddle(Move mv, Board* board, enPlayers color) {
    float result = 1.0f;
    result -= board->KPos[color].getDistance(mv.to).absoluteDist() / 16.0f;
    return result;
}

float sub_E_Kamikaze(Move mv, Board* board, enPlayers color) {
    float result = 0.0f;
    if ( board->isAttackedByOpponent(mv.to, color)) result += 0.5f;
    if( !board->isAttackedByOpponent(mv.from, color)) result += 0.5f;
    return result;
}

float sub_E_UnKamikaze(Move mv, Board* board, enPlayers color) {
    float result = 0.0f;
    if (!board->isAttackedByOpponent(mv.to, color) && board->isAttackedByOpponent(mv.from, color)) result += getPieceValue(board->get(mv.from));
    return result;
}

float sub_E_Random(Move mv, Board* board, enPlayers color) {
    return (float)rand() / (float)RAND_MAX;
}


float sub_Promote(Move mv, Board* board, enPlayers color) {
    if (board->isPromotion(mv)){
        return 1.0f;
    }
    return 0.0f;
}

float sub_EnPassant(Move mv, Board* board, enPlayers color) {
    if (tolower(board->get(mv.from)) == 'p' && mv.from.x != mv.to.x &&
        board->get(mv.to) == '.') {
        return 1.0f;
    }
    return 0.0f;
}

float sub_Ev_Push(Board* board, enPlayers color) {
    float result = 0.0f;
    unsigned char p = '.';
    int x,y;
    for (y = 0; y < 8; ++y) {
        for (x = 0; x < 8; ++x) {
            p = board->get({x, y});
            if (p != '.' && isWhite(p) == color) {
                if (color == enPlayers::White) result += 7 - y;
                else result += y;
            }
        }
    }
    return result / 64.0f;
}

float sub_Ev_Check(Board* board, enPlayers color) {
    if (board->isAttackedByOpponent(board->KPos[color], color)) {
        return -1;
    }else if (board->isAttackedByOpponent(board->KPos[!color], otherPlayer(color))){
        return 1;
    }
    return 0;
}

float sub_Ev_Material(Board* board, enPlayers color) {
    float result = 0.0f;
    unsigned char p = '.';
    int x,y;
    for (y = 0; y < 8; ++y) {
        for (x = 0; x < 8; ++x) {
            p = board->get({x, y});
            if ((y == 0 || y == 7) && isWhite(p) == color && tolower(p) == 'p') {
                result += 0.9;
            } else if (p != '.' && isWhite(p) == color) {
                result += betterGetPieceValue(p);
            } else if (p != '.' && isWhite(p) != color){
                result -= betterGetPieceValue(p);
            }
        }
    }
    return (result + 3.9f) / 7.8f;
}
