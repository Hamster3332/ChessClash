// GITHUB: https://github.com/Hamster3332/ChessClash

#include <SFML/Graphics.hpp>
#include "constants.h"

#include "core/board.h"
#include "ui/boardRender.h"


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Chess Clash");
    window.setFramerateLimit(144);
    
    sf::Vector2f boardPos = {(float)WINDOW_SIZE.y * 0.05f, (float)WINDOW_SIZE.y * 0.05f};
    Board board;
    RenderBoard renderBoard(window, boardPos);
    TextureManager textures;
    textures.load();
    sf::Vector2i clickPos = {};
    sf::Vector2i mousePos = {};
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left)
                {
                    clickPos = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    renderBoard.selectPiece(clickPos, true, board.boardState, board);
                }
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    clickPos = {mouseButtonReleased->position.x, mouseButtonReleased->position.y};
                    renderBoard.selectPiece(clickPos, false, board.boardState, board);
                }
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                mousePos = {mouseMoved->position.x, mouseMoved->position.y};
            }

        }

        window.clear();
        
        renderBoard.draw(textures, mousePos, board.boardState);
        
        window.display();
    }
}
