#ifndef SUB_GENERAL_H
#define SUB_GENERAL_H

#include <board.h>
#include <renderBoard.h>


enum class enEngines {
    random,
    capture,
    kamikaze,
    unKamikaze,
    bandit,
    unBandit,
    cuddle,
    enPassant,
    promote,
    badSchoolKid
};


enum class enEvaluators {
    push,
    material,
    check
};

struct prioEngine {
    enEngines engine;
    float priority;
};

struct prioEvaluators {
    enEvaluators engine;
    float priority;
};

float sub_E_BadSchoolKid(Move mv, Board* board, enPlayers color);
float sub_E_Capture(Move mv, Board* board, enPlayers color);
float sub_E_Bandit(Move mv, Board* board, enPlayers color);
float sub_E_UnBandit(Move mv, Board* board, enPlayers color);
float sub_E_Cuddle(Move mv, Board* board, enPlayers color);
float sub_E_Kamikaze(Move mv, Board* board, enPlayers color);
float sub_E_UnKamikaze(Move mv, Board* board, enPlayers color);
float sub_E_Random(Move mv, Board* board, enPlayers color);
float sub_Promote(Move mv, Board* board, enPlayers color);
float sub_EnPassant(Move mv, Board* board, enPlayers color);


float sub_Ev_Check(Board* board, enPlayers color);
float sub_Ev_Push(Board* board, enPlayers color);
float sub_Ev_Material(Board* board, enPlayers color);
#endif
