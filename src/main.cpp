// GITHUB: https://github.com/Hamster3332/ChessClash

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "constants.h"

#include "core/board.h"
#include "ui/boardRender.h"


int main()
{
    srand(time(0)); // Make Random random

    auto window = sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Chess Clash"); // create Window
    window.setFramerateLimit(144);

    Board board; // Create Board
    board.setBoardState(checkmateBoard);

    sf::Vector2f boardPos = {(float)WINDOW_SIZE.y * 0.05f, (float)WINDOW_SIZE.y * 0.05f};
    RenderBoard renderBoard(window, boardPos);

    TextureManager textures;
    textures.load();

    sf::Vector2i clickPos = {};
    sf::Vector2i mousePos = {};

    while (window.isOpen()) // mainLoop
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
                    renderBoard.onClick(clickPos, true, board);
                }
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    clickPos = {mouseButtonReleased->position.x, mouseButtonReleased->position.y};
                    renderBoard.onClick(clickPos, false, board);
                }
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                mousePos = {mouseMoved->position.x, mouseMoved->position.y};
                renderBoard.onMouseMove(textures, mousePos);
            }

        }

        window.clear();

        renderBoard.draw(textures, board);

        window.display();
    }
}
