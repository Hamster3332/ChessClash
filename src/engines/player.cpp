#include "player.h"
#include <SFML/System/Angle.hpp>

Player::Player(Board& board, RenderBoard& renderer, enPlayers color){
    curBoard = &board;
    curRenderer = &renderer;
}

void Player::startTurn(Move LastTurn){
    playerMove = {{-1, -1}, {-1, -1}};
}

void Player::activeMove(Move currentMove){
    playerMove = currentMove;
}

Move Player::calculate(){
    if (promotionPiece == '0')
        return {{-1, -1}, {-1, -1}};
    if (playerMove.from.x == -1)
        return {{-1, -1}, {-1, -1}};
    if (curBoard->isLegalMove(playerMove.from, playerMove.to)) {
        if (curBoard->isPromotion(playerMove) && promotionPiece == '.'){
            curRenderer->showPromotionWindow = true;
            curRenderer->setPromotionWindowPos({playerMove.to.x * curRenderer->cellSize + curRenderer->boardPos.x,
                playerMove.to.y * curRenderer->cellSize + curRenderer->boardPos.y });
            promotionPiece = '0';
        } else {
            curRenderer->movePiece(playerMove, *curBoard, promotionPiece);
            promotionPiece = '.';
            return playerMove;
        }
    }
    return {{-1, -1}, {-1, -1}};
}

void Player::promotionResult(unsigned char piece) {
    curRenderer->showPromotionWindow = false;
    promotionPiece = piece;
}

bool Player::isReady() {
    return true;
}
