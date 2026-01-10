#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <iterator>
#include <vector>
#include "boardRender.h"


RenderBoard::RenderBoard(sf::RenderWindow& w, const sf::Vector2f& pos)
    : boardPos(pos), window(w)
{
    boardPos = pos;
    bool whiteSquare = true;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            sf::RectangleShape rect = sf::RectangleShape({cellSize, cellSize});
            if (whiteSquare) {
                rect.setFillColor(lightSquareC);
                whiteSquare = false;
            } else {
                rect.setFillColor(darkSquareC);
                whiteSquare = true;
            }
            rect.setPosition({boardPos.x + cellSize * x, boardPos.y + cellSize * y});
            cells.push_back(rect);
            window.draw(rect);
        }
        whiteSquare = !whiteSquare;
    }
}

void RenderBoard::draw(TextureManager &textures, Board &board) {
    for (int c = 0; c < 64; c++) {
        window.draw(cells[c]);
    }

    static sf::Sprite sprite(textures.get('p'));
    sprite.setScale({cellSize / 64.f, cellSize / 64.f});

    static sf::Vector2f pos = {0, 0};
    unsigned char p = '.';

    if (pieceSelected) {
        for (Move possibleMove : possibleMoves) {
            sf::CircleShape circle = sf::CircleShape(cellSize / 5.f);
            circle.setPosition({boardPos.x + cellSize * (static_cast<float>(possibleMove.to.x) + 0.3f),
                             boardPos.y + cellSize * (static_cast<float>(possibleMove.to.y) + 0.3f)});
            circle.setFillColor(possibleMoveC);
            window.draw(circle);
        }
    }

    for (int y = 0; y < 8; y++) {
        pos.y = boardPos.y + cellSize * y;

        for (int x = 0; x < 8; x++) {
            pos.x = boardPos.x + cellSize * x;


            if (x == lastOrigX && y == lastOrigY) {
                sf::RectangleShape rect = sf::RectangleShape({cellSize, cellSize});
                rect.setPosition(pos);
                rect.setFillColor(selectedSquareC);
                window.draw(rect);
            }

            p = board.boardState[y][x];
            if (p == '.') continue;

            sprite.setTexture(textures.get(p));
            if (selectedPiece.equal(x, y) && pieceSelected) {
                selectedPieceChar = p;
            } else {
                sprite.setPosition(pos);
                window.draw(sprite);
            }
        }
    }

    if (pieceSelected) {
        sprite.setPosition(selPieceScreenPos);
        sprite.setTexture(textures.get(selectedPieceChar));

        window.draw(sprite);
    }

    if (showPromotionWindow) {
        sf::RectangleShape outerRect = sf::RectangleShape(promotionButtonSize);
        sf::RectangleShape innerRect = sf::RectangleShape({promotionButtonSize.x - 2 * promoButtonMargin, promotionButtonSize.y - 2 * promoButtonMargin});
        outerRect.setFillColor({120, 120, 120});
        innerRect.setFillColor({80, 80, 80});
        outerRect.setPosition(promotionOuterPos);
        innerRect.setPosition(promotionWindowPos);

        window.draw(outerRect);
        window.draw(innerRect);

        sprite.scale({promoButtonScalar, promoButtonScalar});


        for (int i = 0; i < 4; ++i) {
            sprite.setTexture(textures.get(promotionPieces[i]));
            sprite.setPosition({promotionInnerPos.x, promotionInnerPos.y + i * promoButtonUnit});
            window.draw(sprite);
        }
    }
}

void RenderBoard::onMouseMove(TextureManager &textures, sf::Vector2i &mousePos) {
    selPieceScreenPos = {(float)mousePos.x - cellSize / 2.f, (float)mousePos.y - cellSize / 2.f};
}

onClickReturn RenderBoard::onClick(sf::Vector2i &clickPos, bool isClicking, Board &board) {
    static sf::Vector2f relClickPos;
    onClickReturn funcReturn = onClickReturn();
    if (showPromotionWindow) {
        relClickPos = {clickPos.x - promotionInnerPos.x, clickPos.y - promotionInnerPos.y};
        relClickPos.x /= promoButtonUnit;
        relClickPos.y /= promoButtonUnit;
        if (relClickPos.x < 0.f ||
            relClickPos.y < 0.f ||
            relClickPos.x >= 4.f ||
            relClickPos.y >= 4.f) {
                funcReturn.hasPromoted = false;
                funcReturn.selectedPromotion = 'q';
                pieceSelected = false;
        }
        else {
            int promotionCharThing = (int)relClickPos.y;
            funcReturn.selectedPromotion = promotionPieces[promotionCharThing];
            
            if (board.activePlayer) {
                funcReturn.selectedPromotion = std::toupper(promotionPieces[promotionCharThing]);
            }
            
            funcReturn.hasPromoted = true;
        }
        return funcReturn;
    }
    
    relClickPos = {clickPos.x - boardPos.x, clickPos.y - boardPos.y};
    relClickPos.x /= (boardSize / 8.f);
    relClickPos.y /= (boardSize / 8.f);

    if (relClickPos.x < 0.f ||
        relClickPos.y < 0.f ||
        relClickPos.x >= 8.f ||
        relClickPos.y >= 8.f)
    {
        pieceSelected = false;
        return funcReturn;
    }

    static ChessVector click((int)relClickPos.x, (int)relClickPos.y);
    click.x = (int)relClickPos.x;
    click.y = (int)relClickPos.y;
    if (board.get(click) == '.' && isClicking) {
        return funcReturn;
    }

    if (isClicking && !pieceSelected && board.hasTurn(click)) {
        pieceSelected = true;
        selectedPiece = click;
        possibleMoves = board.getLegalMoves(click);
    } else if (pieceSelected) {
        pieceSelected = false;
        possibleMoves = {};
        //if (board.isLegalMove(selectedPiece, click)) {
        //    movePiece({selectedPiece, click}, board, 'q');
        funcReturn.hasMoved = true;
        funcReturn.move = {selectedPiece, click};
    }
    return funcReturn;
}

void RenderBoard::movePiece(Move move, Board &board, unsigned char promotedTo){
    lastOrigX = move.from.x;
    lastOrigY = move.from.y;
    board.movePiece(move.from, move.to, true, promotedTo);
}

void RenderBoard::setPromotionWindowPos(sf::Vector2f pos) {
    promotionWindowPos = pos;
    promotionInnerPos = {promotionWindowPos.x + promoButtonMargin, promotionWindowPos.y + promoButtonMargin};
    promotionOuterPos = {promotionWindowPos.x - promoButtonMargin, promotionWindowPos.y - promoButtonMargin};
}