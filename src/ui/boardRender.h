#ifndef BOARD_RENDER_H
#define BOARD_RENDER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include "../core/board.h"
#include "../constants.h"

class TextureManager {
public:
    std::map<char, sf::Texture> textures;
    void load();
    sf::Texture& get(char piece);
};

class RenderBoard {
public:
    RenderBoard(sf::RenderWindow& w, const sf::Vector2f& pos);
    void draw(TextureManager &textures, sf::Vector2i &mousePos, unsigned char boardState[8][8]);
    void selectPiece(sf::Vector2i &clickPos, bool isClicking, unsigned char boardState[8][8], Board &board);
    
    std::vector<sf::RectangleShape> cells = {};
    sf::Vector2f boardPos;
    sf::RenderWindow& window;
    float cellSize = boardSize / 8;
    unsigned int clickX = 0, clickY = 0;
    bool pieceSelected;
    unsigned int selPieceX = 999999999, selPieceY = 999999999;
    unsigned int lastOrigX = 999999999, lastOrigY = 999999999;
};

#endif