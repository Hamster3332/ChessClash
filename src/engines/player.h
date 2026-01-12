#ifndef PLAYER_H
#define PLAYER_H

#include "../core/board.h"
#include "../ui/boardRender.h"
#include "../core/playerInterface.cpp"
#include "../core/activePlayerInterface.cpp"
#include <SFML/Window/Keyboard.hpp>

class Player {
public:
    Player(Board& board, RenderBoard& renderer, enPlayers color);
    void activeMove(Move currentMove);
    void promotionResult(unsigned char piece);
    void startTurn(Move LastTurn);
    Move calculate();
    bool isReady();

    Board* curBoard;
    RenderBoard* curRenderer;
    Move playerMove = {{-1, -1}, {-1, -1}};
    unsigned char promotionPiece = '0';
};

#endif
