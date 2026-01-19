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


int main()
{
    logInitiate();
    logID(0);
    srand(time(0)); // Make Random random

    auto window = sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Chess Clash"); // create Window
    window.setFramerateLimit(144);

    Board board; // Create Board
    board.setBoardState(standardBoard);

    sf::Vector2f boardPos = {(float)WINDOW_SIZE.y * 0.05f, (float)WINDOW_SIZE.y * 0.05f};
    RenderBoard renderBoard(window, boardPos);

    TextureManager textures;
    textures.load();

    SoundManager sounds;
    sounds.load();
    SoundPlayer soundPlayer(sounds);


    sf::Vector2i clickPos = {};
    sf::Vector2i mousePos = {};
    onClickReturn clickReturn = onClickReturn();

    //PlayerInterface player = OnlinePlayer(board, renderBoard, enPlayers::Black);
    E_Recursive bot1(board, renderBoard);
    E_General bot2(board, renderBoard);


    /*
     * BEST BOT so far:
     * enEngines::unKamikaze, 10.0f
     * enEngines::bandit, 10.0f
     * enEngines::random, 1.0f
     */
    //bot1.generalSet(enEngines::unKamikaze, 40.0f);
    bot1.generalSet(enEvaluators::material, 100.0f);
    //bot1.generalSet(enEvaluators::push, 0.01f);
    //bot1.generalSet(enEvaluators::check, 1.f);
    //bot1.generalSet(enEngines::capture, 40.0f);
    //bot1.generalSet(enEngines::enPassant, 1000.0f);
    //bot1.generalSet(enEngines::promote, 1000.0f);
    //bot1.generalSet(enEngines::badSchoolKid, 1.0f);
    //+/

    //bot1.generalSet(enEngines::random, 1.0f);


    bot2.generalSet(enEngines::badSchoolKid, 1.0f);

    //OnlinePlayer MMMM(board, renderBoard, enPlayers::White);
    Player player(board, renderBoard);
    OnlinePlayer oPlayer(board, renderBoard);

    PlayerInterface& playerWhite = bot2;
    PlayerInterface& playerBlack = bot1;

    playerWhite.startBot(enPlayers::White);
    playerBlack.startBot(enPlayers::Black);

    while (!playerWhite.isReady() || !playerBlack.isReady()) {
        std::cout << "plz Wait" << std::endl;
    }
    activePlayerInterface& activePlayer = player;
    playerWhite.startTurn({{-1, -1}, {-1, -1}});
    Move lastMove = {{-1, -1}, {-1, -1}};
    auto timeOfLastMove = std::chrono::system_clock::now();
    auto frameStartTime = std::chrono::system_clock::now();
    double time = 0.5f;
    double timeSinceLastTurn = 1.0f;

    sf::Clock clock;

    std::vector<sf::Time> calcTimes = {};

    //std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    while (window.isOpen()) // mainLoop
    {
        frameStartTime = std::chrono::system_clock::now();
        timeSinceLastTurn = ((std::chrono::duration<double>)(frameStartTime - timeOfLastMove)).count();
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

        if (clickReturn.hasMoved) {
            activePlayer.activeMove(clickReturn.move);
        }

        if (clickReturn.hasPromoted) {
            activePlayer.promotionResult(clickReturn.selectedPromotion);
        }


        //std::cout << (frameStartTime - timeOfLastMove).count() << std::endl;
        if (board.gameState != GameState::ONGOING) {
        }
        else if (timeSinceLastTurn < time){}
        else if (board.activePlayer == White) {
            clock.restart();
            lastMove = playerWhite.calculate();
            calcTimes.push_back(clock.getElapsedTime());

            float avT = 0.0f;
            for (sf::Time t : calcTimes) {
                avT += t.asSeconds();
            }
            avT /= calcTimes.size();
            //std::cout << "White takes on average " << avT << " seconds to calculate.\n";


            //std::cout << lastMove.from.x << lastMove.from.y << lastMove.to.x << lastMove.to.y << std::endl;
            if (lastMove.from.x != -1) {
                if (board.isLegalMove(lastMove.from, lastMove.to)){
                    soundPlayer.getToMove(lastMove, board, sounds);
                    playerBlack.startTurn(lastMove);
                    renderBoard.movePiece(lastMove, board, playerWhite.getPromotion());
                    soundPlayer.getAfterMove(board, sounds);
                    soundPlayer.play();
                    timeOfLastMove = std::chrono::system_clock::now();
                } else {
                    std::cout << "Matinnate was right... what a bummer\n";
                    std::cout << std::to_string(lastMove.from.x) << "," << std::to_string(lastMove.from.y) << ",to:";
                    std::cout << std::to_string(lastMove.to.x) << "," << std::to_string(lastMove.to.y) << std::endl;
                }
            } else {
                //std::cout << "DeclinedMoveWhite" << moveToString(lastMove) << std::endl;
            }
        }
        else if (board.activePlayer == Black) {
            clock.restart();
            lastMove = playerBlack.calculate();
            //std::cout << "Black took " << clock.getElapsedTime().asSeconds() << " seconds to calculate.\n";
            //std::cout << lastMove.from.x << lastMove.from.y << lastMove.to.x << lastMove.to.y << std::endl;
            if (lastMove.from.x != -1) {
                if (board.isLegalMove(lastMove.from, lastMove.to)){
                    soundPlayer.getToMove(lastMove, board, sounds);
                    playerWhite.startTurn(lastMove);
                    renderBoard.movePiece(lastMove, board, playerBlack.getPromotion());
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

        window.clear();

        frameStartTime = std::chrono::system_clock::now();
        timeSinceLastTurn = ((std::chrono::duration<double>)(frameStartTime - timeOfLastMove)).count();
        renderBoard.draw(textures, board, timeSinceLastTurn / time);

        window.display();
    }
    logClose();
}
