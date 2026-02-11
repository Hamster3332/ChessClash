#include "player.h"
#include <SFML/System/Angle.hpp>

Player::Player(Board& board, RenderBoard& renderer){
    curBoard = &board;
    curRenderer = &renderer;
}

void Player::startBot(enPlayers _color){

}

bool Player::isReady() {
    return true;
}

void Player::startTurn(Move LastTurn){
    playerMove = {{-1, -1}, {-1, -1}};
}

Move Player::calculate() {
    if (promotionPiece == '0')
        return {{-1, -1}, {-1, -1}};
    if (playerMove.from.x == -1)
        return {{-1, -1}, {-1, -1}};
    if (curBoard->isLegalMove(playerMove.from, playerMove.to)) {
        if (curBoard->isPromotion(playerMove) && promotionPiece == '.'){
            curRenderer->showPromotionWindow = true;
            curRenderer->setPromotionWindowPos({playerMove.to.x * curRenderer->cellSize.x + curRenderer->boardPos.x,
                playerMove.to.y * curRenderer->cellSize.y + curRenderer->boardPos.y });
            promotionPiece = '0';
        } else {
            return playerMove;
        }
    }
    return {{-1, -1}, {-1, -1}};
}

Move Player::fallback(std::vector<Move> &moves){
    return {{-1, -1}, {-1, -1}};
}

unsigned char Player::getPromotion() {
    unsigned char result = promotionPiece;
    promotionPiece = '.';
    return result;
}

void Player::activeMove(Move currentMove){
    playerMove = currentMove;
}

void Player::promotionResult(unsigned char piece) {
    curRenderer->showPromotionWindow = false;
    promotionPiece = piece;
}
