#ifndef RECURSIVE_H
#define RECURSIVE_H

#include <board.h>
#include <renderBoard.h>
#include <vector>
#include "chessVector.h"
#include "playerInterface.cpp"
#include <subGeneral.h>
#include <iostream>

struct moveRecsursives {
    float score = 0;
    float alpha = 0;
    float beta = 0;
    Move move = {{-1,-1}, {-1,-1}};
    std::vector<struct  moveRecsursives> later = {};
};

class E_Recursive : public PlayerInterface {
public:
    E_Recursive(Board& board, RenderBoard& renderer);
    void generalSet(enEvaluators engine, float priority);
    void generalSet(enEngines engine, float priority);
    void generalReset();
    void startBot(enPlayers _color) override;
    bool isReady() override;
    void startTurn(Move LastTurn) override;
    Move calculate() override;
    unsigned char getPromotion() override;
    Move fallback(std::vector<Move> &moves) override;

    Board* curBoard;
    enPlayers color;
    scoredMove selected = {{{-1, -1}, {-1, -1}}, -1.0f};
    std::vector<prioEvaluators> priorities = {};
    std::vector<prioEngine> prioritiesEngine = {};
    std::vector<scoredMove> moves = {};

    std::vector<scoredMove> test = {};

    std::vector<moveRecsursives> recursiveReturn = {};

    scoredMove getBestMove();
    float evaluateRecursive(enPlayers Player, float alpha, float beta, int levels, moveRecsursives &recDis);
    float getEvaluatorResult(enEvaluators engine) const;
    float getEngineResult(Move mv, enEngines engine) const;
};

#endif
