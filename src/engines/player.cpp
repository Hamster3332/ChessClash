#include "player.h"

Player::Player(Board& board, RenderBoard& renderer){
    curBoard = &board;
    curRenderer = &renderer;
}

void Player::startTurn(Move LastTurn){

}

Move Player::calculate(Move playerMove){
    if (curBoard->isLegalMove(playerMove.from, playerMove.to)) {
        if (curBoard->isPromotion(playerMove)){
            curRenderer->showPromotionWindow = true;
            curRenderer->setPromotionWindowPos({playerMove.to.x * curRenderer->cellSize + curRenderer->boardPos.x,
                playerMove.to.y * curRenderer->cellSize + curRenderer->boardPos.y });
            promotionDelayedMove = playerMove;
        }else{
            curRenderer->movePiece(playerMove, *curBoard, '.');
            return playerMove;
        }
    }
    return {{-1, -1}, {-1, -1}};
}

Move Player::promotionResult(unsigned char piece) {
    curRenderer->showPromotionWindow = false;
    curRenderer->movePiece(promotionDelayedMove, *curBoard, piece);
    return promotionDelayedMove;
}

bool Player::isReady() {
    return true;
}
