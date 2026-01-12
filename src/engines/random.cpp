#include "random.h"
#include <cctype>
#include <cstdlib>
#include <vector>

E_Random::E_Random(Board& board, RenderBoard& renderer, enPlayers c) {
    curBoard = &board;
    curRenderer = &renderer;
    color = c;
}

void E_Random::startTurn(Move LastTurn){

}

Move E_Random::calculate(){
    std::vector<Move> moves = {};
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (curBoard->get({x, y}) != '.' && std::isupper(curBoard->get({x, y})) == color) {
                for (Move mv : curBoard->getLegalMoves({x, y})) {
                    moves.push_back(mv);
                }
            }
        }
    }
    Move selected = moves[rand() % (moves.size() - 1)];
    std::cout << selected.from.x << selected.from.y << selected.to.x << selected.to.y << std::endl;
    curRenderer->movePiece(selected, *curBoard, 'q');
    return selected;
}

Move E_Random::promotionResult(unsigned char piece) {
    return promotionDelayedMove;
}

bool E_Random::isReady(){
    return true;
}
