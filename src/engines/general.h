#ifndef GENERAL_H
#define GENERAL_H

#include <board.h>
#include <renderBoard.h>
#include <vector>
#include "playerInterface.cpp"
#include <subGeneral.h>

class E_General : public PlayerInterface {
public:
    E_General(Board& board, RenderBoard& renderer);
    void generalSet(enEngines engine, float priority);
    void generalReset();
    void startBot(enPlayers _color) override;
    bool isReady() override;
    void startTurn(Move LastTurn) override;
    Move calculate() override;
    unsigned char getPromotion() override;
    Move fallback(std::vector<Move> &moves) override;

    Board* curBoard;
    scoredMove selected = {{{-1, -1}, {-1, -1}}, -1.0f};
    enPlayers color;
    std::vector<prioEngine> priorities = {};
    std::vector<scoredMove> moves = {};
private:
    float getEngineResult(Move mv, enEngines engine);
};

#endif
