// GITHUB: https://github.com/Hamster3332/ChessClash

#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "constants.h"

#include "core/board.h"
#include "core/logger.h"
#include "engines/player.h"
#include "ui/boardRender.h"


int main()
{
    logInitiate();
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
    onClickReturn clickReturn = onClickReturn();

    Player first(board, renderBoard);
    Player second(board, renderBoard);
    bool firstHasTurn = true;
    first.startTurn({{-1, -1}, {-1, -1}});
    Move lastMove = {{-1, -1}, {-1, -1}};

    while (window.isOpen()) // mainLoop
    {
        clickReturn = onClickReturn();
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
                    clickReturn = renderBoard.onClick(clickPos, true, board);
                }
            }
            else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    clickPos = {mouseButtonReleased->position.x, mouseButtonReleased->position.y};
                    clickReturn = renderBoard.onClick(clickPos, false, board);
                }
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                mousePos = {mouseMoved->position.x, mouseMoved->position.y};
                renderBoard.onMouseMove(textures, mousePos);
            }

        }

        if (clickReturn.hasMoved){
            lastMove = first.calculate(clickReturn.move);
            if (lastMove.from.x != -1){
                first.startTurn(lastMove);
            }
        }

        if (clickReturn.hasPromoted){
            lastMove = first.promotionResult(clickReturn.selectedPromotion);
            if (lastMove.from.x != -1){
                first.startTurn(lastMove);
            }
        }


        window.clear();

        renderBoard.draw(textures, board);

        window.display();
    }
    logClose();
}
