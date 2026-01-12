#ifndef RANDOM_H
#define RANDOM_H

#include "../core/board.h"
#include "../ui/boardRender.h"
#include "../core/playerInterface.cpp"

class E_Random : public PlayerInterface {
public:
    E_Random(Board& board, RenderBoard& renderer, enPlayers color);
    void startTurn(Move LastTurn) override;
    Move calculate() override;
    Move promotionResult(unsigned char piece);
    bool isReady() override;

    Board* curBoard;
    RenderBoard* curRenderer;
    Move promotionDelayedMove = {{-1, -1}, {-1, -1}};
    enPlayers color;
};

#endif
