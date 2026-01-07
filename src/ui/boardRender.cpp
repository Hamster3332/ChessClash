#include <iostream>
#include <iterator>
#include <ostream>
#include "boardRender.h"

void TextureManager::load() {
        if (!textures['p'].loadFromFile("assets/b_pawn.png"))
            std::cerr << "Failed to load b_pawn.png." << std::endl;
        if (!textures['P'].loadFromFile("assets/w_pawn.png"))
            std::cerr << "Failed to load w_pawn.png." << std::endl;
        if (!textures['n'].loadFromFile("assets/b_knight.png"))
            std::cerr << "Failed to load b_knight.png." << std::endl;
        if (!textures['N'].loadFromFile("assets/w_knight.png"))
            std::cerr << "Failed to load w_knight.png." << std::endl;
        if (!textures['b'].loadFromFile("assets/b_bishop.png"))
            std::cerr << "Failed to load b_bishop.png." << std::endl;
        if (!textures['B'].loadFromFile("assets/w_bishop.png"))
            std::cerr << "Failed to load w_bishop.png." << std::endl;
        if (!textures['r'].loadFromFile("assets/b_rook.png"))
            std::cerr << "Failed to load b_rook.png." << std::endl;
        if (!textures['R'].loadFromFile("assets/w_rook.png"))
            std::cerr << "Failed to load w_rook.png." << std::endl;
        if (!textures['q'].loadFromFile("assets/b_queen.png"))
            std::cerr << "Failed to load b_queen.png." << std::endl;
        if (!textures['Q'].loadFromFile("assets/w_queen.png"))
            std::cerr << "Failed to load w_queen.png." << std::endl;
        if (!textures['k'].loadFromFile("assets/b_king.png"))
            std::cerr << "Failed to load b_king.png." << std::endl;
        if (!textures['K'].loadFromFile("assets/w_king.png"))
            std::cerr << "Failed to load w_king.png." << std::endl;
    }

sf::Texture& TextureManager::get(char piece) {
    return textures.at(piece);
}


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

void RenderBoard::draw(TextureManager &textures, sf::Vector2i &mousePos, unsigned char boardState[8][8]) {
    for (int c = 0; c < 64; c++) {
        //std::cout << cells.size() << std::endl;
        window.draw(cells[c]);
    }

    sf::Sprite sprite(textures.get('p'));
    sf::Vector2f pos = {0, 0};

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

            char p = boardState[y][x];
            if (p == '.') continue;

            sprite.setTexture(textures.get(p));
            if (x == selPieceX && y == selPieceY && pieceSelected) {
                sprite.setPosition({(float)mousePos.x - cellSize / 2.f, (float)mousePos.y - cellSize / 2.f});
            } else {
                sprite.setPosition(pos);
            }
            sprite.setScale({cellSize / 64.f, cellSize / 64.f});

            window.draw(sprite);
        }
    }
}

void RenderBoard::selectPiece(sf::Vector2i &clickPos, bool isClicking, Board &board) {
    sf::Vector2f relClickPos = {clickPos.x - boardPos.x, clickPos.y - boardPos.y};
    relClickPos.x /= (boardSize / 8.f);
    relClickPos.y /= (boardSize / 8.f);

    if (relClickPos.x < 0.f ||
        relClickPos.y < 0.f ||
        relClickPos.x > 8.f ||
        relClickPos.y > 8.f)
    {
        pieceSelected = false;
        return;
    }

    clickX = (int)relClickPos.x;
    clickY = (int)relClickPos.y;

    char p = board.boardState[clickY][clickX];
    if (p == '.' && isClicking) {
        return;
    }

    if (isClicking && !pieceSelected) {
        pieceSelected = true;
        selPieceX = clickX;
        selPieceY = clickY;
    } else if (pieceSelected) {
        pieceSelected = false;
        if (board.isLegalMove({selPieceX, selPieceY}, {clickX, clickY})) {
            lastOrigX = selPieceX;
            lastOrigY = selPieceY;
            board.movePiece({selPieceX, selPieceY}, {clickX, clickY}, true);
        }
    }
}
