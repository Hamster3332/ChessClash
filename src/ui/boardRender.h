#ifndef BOARD_RENDER_H
#define BOARD_RENDER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <map>
#include <vector>
#include "../core/board.h"
#include "../constants.h"
#include "../core/chessVector.h"
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

class RenderBoard {
public:
    RenderBoard(sf::RenderWindow& w, const sf::Vector2f& pos);
    void draw(TextureManager &textures, Board &board);
    void onMouseMove(TextureManager &textures, sf::Vector2i &mousePos);
    onClickReturn onClick(sf::Vector2i &clickPos, bool isClicking, Board &board);
    void movePiece(Move move, Board &board, unsigned char promotedTo);

    std::vector<sf::RectangleShape> cells = {};
    sf::Vector2f boardPos;
    sf::Vector2f selPieceScreenPos;
    sf::RenderWindow& window;
    float cellSize = boardSize / 8;
    std::vector<Move> possibleMoves;
    bool pieceSelected = false;
    unsigned char selectedPieceChar = 'p';
    ChessVector selectedPiece = ChessVector(999999999,999999999);
    unsigned int lastOrigX = 999999999, lastOrigY = 999999999;

    bool showPromotionWindow = false;
    float promoButtonScalar = .7f;
    float promoButtonUnit = promoButtonScalar * cellSize;
    float promoButtonMargin = 10.f * promoButtonScalar;
    sf::Vector2f promotionWindowPos = {100, 100};
    sf::Vector2f promotionButtonSize = {promoButtonUnit + 4 * promoButtonMargin, 4 * promoButtonUnit + 4 * promoButtonMargin};
    sf::Vector2f promotionInnerPos = {promotionWindowPos.x + promoButtonMargin, promotionWindowPos.y + promoButtonMargin};
    sf::Vector2f promotionOuterPos = {promotionWindowPos.x - promoButtonMargin, promotionWindowPos.y - promoButtonMargin};
    unsigned char promotionPieces[4] = {'n', 'b', 'r', 'q'};
    
    void setPromotionWindowPos(sf::Vector2f pos);
};

#endif
