#include "renderBoard.h"

#include <algorithm>

#include "interpolations.cpp"

RenderBoard::RenderBoard(sf::RenderWindow& w, Board &board, TextureManager &textures): window(w), gameBoard(board), textureManager(textures){}

void RenderBoard::initialize(const sf::Vector2f &pos, const sf::Vector2f &size) {
    boardPos = pos;
    boardSize = size;
    cellSize = size / 8.0f;
    bool whiteSquare = true;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            auto rect = sf::RectangleShape(cellSize);
            if (whiteSquare) {
                rect.setFillColor(lightSquareC);
                whiteSquare = false;
            } else {
                rect.setFillColor(darkSquareC);
                whiteSquare = true;
            }
            rect.setPosition({boardPos.x + cellSize.x * x, boardPos.y + cellSize.y * y});
            cells.push_back(rect);
        }
        whiteSquare = !whiteSquare;
    }
};

void RenderBoard::transform(const sf::Vector2f pos, const sf::Vector2f size){
    boardPos = pos;
    boardSize = size;
    cellSize = size / 8.0f;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            cells.at(y*8+ x).setSize( cellSize);
            cells.at(y*8+ x).setPosition({boardPos.x + cellSize.x * x, boardPos.y + cellSize.y * y});
        }
    }

}

void RenderBoard::draw(const double timeSinceStart) {
    for (int c = 0; c < 64; c++) {
        window.draw(cells[c]);
    }

    static sf::Sprite sprite(textureManager.get('p'));
    sprite.setScale(cellSize / 64.f);

    static sf::Vector2f pos = {0, 0};
    unsigned char p = '.';

    if (pieceSelected) {
        for (Move possibleMove : possibleMoves) {
            sf::CircleShape circle = sf::CircleShape(cellSize.x / 5.f);
            circle.setPosition({boardPos.x + cellSize.x * (static_cast<float>(possibleMove.to.x) + 0.3f),
                             boardPos.y + cellSize.y * (static_cast<float>(possibleMove.to.y) + 0.3f)});
            circle.setFillColor(possibleMoveC);
            window.draw(circle);
        }
    }

    for (int y = 0; y < 8; y++) {
        pos.y = boardPos.y + cellSize.y * y;

        for (int x = 0; x < 8; x++) {
            pos.x = boardPos.x + cellSize.x * x;
            pos.y = boardPos.y + cellSize.y * y;


            if (x == lastOrig.x && y == lastOrig.y) {
                sf::RectangleShape rect = sf::RectangleShape(cellSize);
                rect.setPosition(pos);
                rect.setFillColor(sf::Color(selectedSquareC.r, selectedSquareC.g, selectedSquareC.b, selectedSquareC.a * easeInOutQuart(std::clamp((float)timeSinceStart * 3.0f, 0.0f, 1.0f)))); // <- Animieren bitte
                window.draw(rect);
            }

            p = gameBoard.boardState[y][x];
            if (p == '.') continue;

            sprite.setTexture(textureManager.get(p));
            if (selectedPiece.equal(x, y) && pieceSelected) {
                selectedPieceChar = p;
            } else {
                if (lastDestination.equal(x,y) && timeSinceStart <= 1.f) {
                    const float aP = easeInOutQuart(static_cast<float>(timeSinceStart));

                    pos.x = static_cast<float>(lastOrig.x) + aP * static_cast<float>(x - lastOrig.x);
                    pos.y = static_cast<float>(lastOrig.y) + aP * static_cast<float>(y - lastOrig.y);
                    pos.x = boardPos.x + cellSize.x * pos.x;
                    pos.y = boardPos.y + cellSize.y * pos.y;
                }
                sprite.setPosition(pos);
                window.draw(sprite);
            }
        }
    }

    if (pieceSelected) {
        sprite.setPosition(selPieceScreenPos);
        sprite.setTexture(textureManager.get(static_cast<char>(selectedPieceChar)));

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
            sprite.setTexture(textureManager.get(static_cast<char>(promotionPieces[i])));
            sprite.setPosition({promotionInnerPos.x, promotionInnerPos.y + i * promoButtonUnit});
            window.draw(sprite);
        }
    }
}

void RenderBoard::onMouseMove(const sf::Vector2i &mousePos) {
    selPieceScreenPos = {static_cast<float>(mousePos.x) - cellSize.x / 2.f, static_cast<float>(mousePos.y) - cellSize.y / 2.f};
}

void RenderBoard::onClick(const sf::Vector2i &clickPos, const bool isClicking) {
    static sf::Vector2f relClickPos;
    clickReturn.hasMoved = false;
    clickReturn.hasPromoted = false;
    if (showPromotionWindow) {
        relClickPos = {static_cast<float>(clickPos.x) - promotionInnerPos.x, static_cast<float>(clickPos.y) - promotionInnerPos.y};
        relClickPos.x /= promoButtonUnit;
        relClickPos.y /= promoButtonUnit;
        if (relClickPos.x < 0.f ||
            relClickPos.y < 0.f ||
            relClickPos.x >= 4.f ||
            relClickPos.y >= 4.f) {
                clickReturn.hasPromoted = false;
                clickReturn.selectedPromotion = 'q';
                pieceSelected = false;
        }
        else {
            const int promotionCharThing = static_cast<int>(relClickPos.y);
            clickReturn.selectedPromotion = promotionPieces[promotionCharThing];

            if (gameBoard.activePlayer) {
                clickReturn.selectedPromotion = std::toupper(promotionPieces[promotionCharThing]);
            }

            clickReturn.hasPromoted = true;
        }
        return;
    }

    relClickPos = {static_cast<float>(clickPos.x) - boardPos.x, static_cast<float>(clickPos.y) - boardPos.y};
    relClickPos.x /= (boardSize.x / 8.f);
    relClickPos.y /= (boardSize.y / 8.f);

    if (relClickPos.x < 0.f ||
        relClickPos.y < 0.f ||
        relClickPos.x >= 8.f ||
        relClickPos.y >= 8.f)
    {
        pieceSelected = false;
        return;
    }

    static ChessVector click(static_cast<int>(relClickPos.x), static_cast<int>(relClickPos.y));
    click.x = static_cast<int>(relClickPos.x);
    click.y = static_cast<int>(relClickPos.y);
    if (gameBoard.get(click) == '.' && isClicking) {
        return;
    }

    if (isClicking && !pieceSelected && gameBoard.hasTurn(click)) {
        pieceSelected = true;
        selectedPiece = click;
        possibleMoves = gameBoard.getLegalMoves(click);
    } else if (pieceSelected) {
        pieceSelected = false;
        possibleMoves = {};
        //if (board.isLegalMove(selectedPiece, click)) {
        //    movePiece({selectedPiece, click}, board, 'q');
        clickReturn.hasMoved = true;
        clickReturn.move = {selectedPiece, click};
    }
}

onClickReturn RenderBoard::getClickReturn() const {
    return clickReturn;
}

void RenderBoard::movePiece(const Move move, const unsigned char promotedTo) {
    lastOrig = move.from;
    lastDestination = move.to;
    gameBoard.movePiece(move.from, move.to, true, promotedTo);
}

void RenderBoard::setPromotionWindowPos(sf::Vector2f pos) {
    promotionWindowPos = pos;
    promotionInnerPos = {promotionWindowPos.x + promoButtonMargin, promotionWindowPos.y + promoButtonMargin};
    promotionOuterPos = {promotionWindowPos.x - promoButtonMargin, promotionWindowPos.y - promoButtonMargin};
}
