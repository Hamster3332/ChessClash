#ifndef BOARD_RENDER_H
#define BOARD_RENDER_H

#include "windowInterface.cpp"
#include <vector>
#include <board.h>
#include "../constants.h"
#include <chessVector.h>
#include "textureManager.h"


struct onClickReturn {
    bool hasMoved = false;
    Move move = {{0,0},{0,0}};
    bool hasPromoted = false;
    unsigned char selectedPromotion = '.';

    onClickReturn(bool hasMoved, Move move, bool hasPromoted, unsigned char selectedPromotion)
      : hasMoved(hasMoved), move(move), hasPromoted(hasPromoted), selectedPromotion(selectedPromotion){}
    onClickReturn()
      : hasMoved(false), move({{0,0},{0,0}}), hasPromoted(false), selectedPromotion('.'){}
};


class RenderBoard : public WindowInterface {
public:
    RenderBoard(sf::RenderWindow& w, Board &board, TextureManager &textures);
    void initialize(const sf::Vector2f &pos, const sf::Vector2f &size) override;
    void draw(const double timeSinceStart) override;
    void onMouseMove(const sf::Vector2i &mousePos) override;
    void onClick(const sf::Vector2i &clickPos, bool isClicking) override;

    void movePiece(Move move, unsigned char promotedTo);
    onClickReturn getClickReturn() const;

    Board &gameBoard;
    TextureManager &textureManager;
    onClickReturn clickReturn = onClickReturn();

    std::vector<sf::RectangleShape> cells = {};
    sf::Vector2f boardPos = {0.f, 0.f};
    sf::Vector2f boardSize = {0.f, 0.f};

    sf::Vector2f selPieceScreenPos;
    sf::RenderWindow& window;
    sf::Vector2f cellSize = {0.f, 0.f};
    std::vector<Move> possibleMoves;
    bool pieceSelected = false;
    unsigned char selectedPieceChar = 'p';
    ChessVector selectedPiece = ChessVector(999999999,999999999);
    unsigned int lastOrigX = 999999999, lastOrigY = 999999999;
    ChessVector lastOrig = {-1, -1};
    ChessVector lastDestination = {-1, -1};

    bool showPromotionWindow = false;
    float promoButtonScalar = .7f;
    float promoButtonUnit = promoButtonScalar * cellSize.x;
    float promoButtonMargin = 10.f * promoButtonScalar;
    sf::Vector2f promotionWindowPos = {100, 100};
    sf::Vector2f promotionButtonSize = {promoButtonUnit + 4 * promoButtonMargin, 4 * promoButtonUnit + 4 * promoButtonMargin};
    sf::Vector2f promotionInnerPos = {promotionWindowPos.x + promoButtonMargin, promotionWindowPos.y + promoButtonMargin};
    sf::Vector2f promotionOuterPos = {promotionWindowPos.x - promoButtonMargin, promotionWindowPos.y - promoButtonMargin};
    unsigned char promotionPieces[4] = {'n', 'b', 'r', 'q'};

    void setPromotionWindowPos(sf::Vector2f pos);
};

#endif
