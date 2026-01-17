#ifndef RECURSIVE_H
#define RECURSIVE_H

#include <board.h>
#include <renderBoard.h>
#include <vector>
#include "chessVector.h"
#include "playerInterface.cpp"
#include <subGeneral.h>

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
    float score;
    enPlayers color;
    scoredMove selected = {{{-1, -1}, {-1, -1}}, -1.0f};
    std::vector<prioEvaluators> priorities = {};
    std::vector<prioEngine> prioritiesEngine = {};
    std::vector<scoredMove> moves = {};

    std::vector<scoredMove> test = {};

    scoredMove getBestMove();
    float evaluateRecursive(Move mv, enPlayers Player, int levels);
    float getEvaluatorResult(enEvaluators engine);
    float getEngineResult(Move mv, enEngines engine);
};

#endif
