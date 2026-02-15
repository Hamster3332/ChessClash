// GITHUB: https://github.com/Hamster3332/ChessClash
// recursion: 3
// average calculating time after x optimizations:
// 0: 6.36587
// 1: 5.79479 (No logs: no quadrillion GB logs.json)
// 2: 5.22501 (inline isAttackedByOpponent)
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <recursive.h>
#include "chessVector.h"
#include "constants.h"

#include <board.h>
#include <logger.h>
#include <general.h>
#include "playerInterface.cpp"
#include "activePlayerInterface.cpp"
#include <soundManager.h>
#include "subGeneral.h"
#include "textureManager.h"
#include <onlinePlayer.h>
#include <player.h>
#include <renderBoard.h>

#include "menu.h"


class ChessClashApp {
public:
    sf::RenderWindow window;
    Board board;

    TextureManager textures;
    SoundManager sounds;
    SoundPlayer soundPlayer;
    sf::Clock clock;
    std::vector<sf::Time> calcTimes;

    RenderBoard renderBoard;
    MainMenu menu;
    std::vector<WindowInterface *> UIElements;

    std::chrono::system_clock::time_point frameStartTime;
    std::chrono::system_clock::time_point timeOfLastMove;

    double time = 0.5f;
    double timeSinceLastTurn = 1.0f;

    PlayerInterface *playerWhite = nullptr;
    PlayerInterface *playerBlack = nullptr;
    activePlayerInterface *activePlayer = nullptr;

    sf::Vector2i clickPos;
    sf::Vector2i mousePos;
    Move lastMove = {{-1, -1}, {-1, -1}};
    E_Recursive bot1;

    ChessClashApp() : soundPlayer(SoundPlayer(sounds)),
    renderBoard(RenderBoard(window, board, textures)),
    menu(MainMenu(window, textures)),
    window(sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Chess Clash")),
    bot1(E_Recursive(board, renderBoard)){}

    void run() {

        logInitiate();
        logID(0);
        //srand(time(0)); // Make Random random

        window.setFramerateLimit(180);

        board.setBoardState(standardBoard);

        textures.load();

        sounds.load();

        //PlayerInterface player = OnlinePlayer(board, renderBoard, enPlayers::Black);
        E_General bot2(board, renderBoard);


        /*
         * BEST BOT so far:
         * enEngines::unKamikaze, 10.0f
         * enEngines::bandit, 10.0f
         * enEngines::random, 1.0f
         */
        bot1.generalSet(enEvaluators::material, 100.0f);

        bot2.generalSet(enEngines::badSchoolKid, 1.0f);

        Player player(board, renderBoard);
        OnlinePlayer oPlayer(board, renderBoard);

        playerWhite = &player;
        playerBlack = &player;

        playerWhite -> startBot(enPlayers::White);
        playerBlack -> startBot(enPlayers::Black);
        activePlayer = &player;

        while (!playerWhite -> isReady() || !playerBlack -> isReady()) {
            std::cout << "plz Wait" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        playerWhite -> startTurn({{-1, -1}, {-1, -1}});

        UIElements.push_back(&renderBoard);
        UIElements.push_back(&menu);

        sf::Vector2f pos = {0.f, 0.f};
        sf::Vector2f size = {WINDOW_SIZE.x, WINDOW_SIZE.y};
        renderBoard.initialize(pos, size);
        menu.initialize(pos, size);

        frameStartTime = std::chrono::system_clock::now();
        timeOfLastMove = std::chrono::system_clock::now();

        mainLoop();

        logClose();
        playerWhite = nullptr;
        playerBlack = nullptr;
        activePlayer = nullptr;
    }

private:

    void mainLoop() {
        onClickReturn clickReturn = onClickReturn();
        while (window.isOpen()) // mainLoop
        {

            if (menu.enableGame) {
                renderBoard.active = true;
                menu.enableGame = false;
            }

            renderBoard.drawing = menu.showGame;

            frameStartTime = std::chrono::system_clock::now();
            timeSinceLastTurn = static_cast<std::chrono::duration<double>>(frameStartTime - timeOfLastMove).count();

            clickReturn = renderBoard.getClickReturn();

            if (clickReturn.hasMoved) {
                activePlayer -> activeMove(clickReturn.move);
            }

            if (clickReturn.hasPromoted) {
                activePlayer -> promotionResult(clickReturn.selectedPromotion);
            }

            eventHandler();

            if (renderBoard.active)
                updateBoard();


            window.clear();

            frameStartTime = std::chrono::system_clock::now();
            timeSinceLastTurn = static_cast<std::chrono::duration<double>>(frameStartTime - timeOfLastMove).count();
            for (WindowInterface *UIElement: UIElements) {
                if (!UIElement->drawing) continue;
                UIElement->draw(timeSinceLastTurn / time);
            }

            window.display();
        }
    }

    void eventHandler() {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                }
            }
            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    for (WindowInterface *UIElement: UIElements) {
                        if (!UIElement->active) continue;
                        clickPos = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                        UIElement->onClick(clickPos, true);
                    }
                }
            } else if (const auto *mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                    for (WindowInterface *UIElement: UIElements) {
                        if (!UIElement->active) continue;
                        clickPos = {mouseButtonReleased->position.x, mouseButtonReleased->position.y};
                        UIElement->onClick(clickPos, false);
                    }
                }
            }
            if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                for (WindowInterface *UIElement: UIElements) {
                    if (!UIElement->active) continue;
                    mousePos = {mouseMoved->position.x, mouseMoved->position.y};
                    UIElement->onMouseMove(mousePos);
                }
            }
        }
    }

    void updateBoard() {
        if (board.gameState != GameState::ONGOING) {
        } else if (timeSinceLastTurn < time) {
        } else if (board.activePlayer == White && renderBoard.active) {
            clock.restart();
            lastMove = playerWhite -> calculate();
            calcTimes.push_back(clock.getElapsedTime());

            float avT = 0.0f;
            for (sf::Time t: calcTimes) {
                avT += t.asSeconds();
            }
            avT /= calcTimes.size();

            //std::cout << "White takes on average " << avT << " seconds to calculate.\n";


            //std::cout << lastMove.from.x << lastMove.from.y << lastMove.to.x << lastMove.to.y << std::endl;
            if (lastMove.from.x != -1) {
                if (board.isLegalMove(lastMove.from, lastMove.to)) {
                    soundPlayer.getToMove(lastMove, board, sounds);
                    playerBlack -> startTurn(lastMove);
                    renderBoard.movePiece(lastMove, playerWhite -> getPromotion());
                    soundPlayer.getAfterMove(board, sounds);
                    soundPlayer.play();
                    timeOfLastMove = std::chrono::system_clock::now();
                } else {
                    std::cout << "Matinnate was right... what a bummer\n";
                    std::cout << std::to_string(lastMove.from.x) << "," << std::to_string(lastMove.from.y) <<
                            ",to:";
                    std::cout << std::to_string(lastMove.to.x) << "," << std::to_string(lastMove.to.y) << std::endl;
                }
            } else {
                //std::cout << "DeclinedMoveWhite" << moveToString(lastMove) << std::endl;
            }
        } else if (board.activePlayer == Black && renderBoard.active) {
            clock.restart();
            lastMove = playerBlack -> calculate();
            //std::cout << "Black took " << clock.getElapsedTime().asSeconds() << " seconds to calculate.\n";
            //std::cout << lastMove.from.x << lastMove.from.y << lastMove.to.x << lastMove.to.y << std::endl;
            if (lastMove.from.x != -1) {
                if (board.isLegalMove(lastMove.from, lastMove.to)) {
                    soundPlayer.getToMove(lastMove, board, sounds);
                    playerWhite -> startTurn(lastMove);
                    renderBoard.movePiece(lastMove, playerBlack -> getPromotion());
                    soundPlayer.getAfterMove(board, sounds);
                    soundPlayer.play();
                    timeOfLastMove = std::chrono::system_clock::now();
                } else {
                    std::cout << "Matinnate was right... what a bummer\n";
                    std::cout << lastMove.from.x << "x, " << lastMove.from.y << "y, to:";
                    std::cout << lastMove.to.x << "x, " << lastMove.to.y << "y\n";
                }
            } else {
                std::cout << "DeclinedMoveBlack" << moveToString(lastMove) << std::endl;
            }
        }
    }
};

int main() {
    std::cout << "lol" << std::endl;
    ChessClashApp clash_app;
    clash_app.run();

    try {
        //clash_app.run();
    } catch (std::exception e) {
        throw e.what();
    }

    return 0;
}
