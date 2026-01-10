#ifndef PLAYER_H
#define PLAYER_H

#include "../core/board.h"
#include "../ui/boardRender.h"

class Player {
public:
    Player(Board& board, RenderBoard& renderer);
    void startTurn(Move LastTurn);
    Move calculate(Move playerMove);
    Move promotionResult(unsigned char piece);
    bool isReady();

    Board* curBoard;
    RenderBoard* curRenderer;
    Move promotionDelayedMove = {{-1, -1}, {-1, -1}};
};

#endif
