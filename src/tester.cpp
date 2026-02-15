// GITHUB: https://github.com/Hamster3332/ChessClash
// Tests Recursion depth 3
// Mesurement 1
// times: 0.0084183 | 1.345 | 0.111245 | 2.55348
// Mesurement 2 (to:Lower replaced)
// times: 0.0076502 | 1.32767 | 0.100256 | 2.52206
// Mesurement 3 (to:early breakof)
// times: 0.0082622 | 1.33052 | 0.100274 | 2.49181
#include <iostream>
#include <ostream>
#include <recursive.h>
#include "chessVector.h"
#include "constants.h"
#include <soundManager.h>
#include "subGeneral.h"
#include "fastBoard.h"
#include <renderBoard.h>
#include <fastBoard.h>

inline unsigned char test[8][8] = {
    {'.', '.', '.', '.', 'k', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', 'p', '.', '.', '.'},
    {'.', '.', '.', '.', '.', 'P', '.', '.'},
    {'.', '.', '.', '.', 'K', '.', '.', '.'}
};

sf::Clock myClock;
std::vector<sf::Time> calcTimes = {};

//auto window = sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Clash Royal Deluxe"); // create Window
Board board;
sf::Vector2f boardPos = {(float)WINDOW_SIZE.y * 0.05f, (float)WINDOW_SIZE.y * 0.05f};
sf::Vector2f boardSize = {(float)WINDOW_SIZE.y * 0.9f, (float)WINDOW_SIZE.y * 0.9f};
sf::Vector2i cameraPos = {0, 0};
TextureManager textures;
//RenderBoard renderBoard(window, board, textures);
//E_Recursive myE(board, renderBoard);
sf::Font f = sf::Font("assets/fonts/Lost Tumbler.ttf");
sf::Text g = sf::Text(f, "dwdawd", 30);
/*
void layoutTest(unsigned char layout[8][8]){
    calcTimes.clear();
    board.setBoardState(layout);
    float avT = 0.0f;
    Move m = {{-1, -1},{-1, -1}};
    for (int i = 0; i < 10; i++){
        myClock.restart();
        m = myE.calculate();
        calcTimes.push_back(myClock.getElapsedTime());
    }
    for (sf::Time t : calcTimes) {
        avT += t.asSeconds();
    }
    avT /= calcTimes.size();
    std::cout << "Engine takes on average " << avT << " seconds to calculate.\n";


    std::cout << moveToString(m) << std::endl << std::endl;
}

void display() {
    textures.load();
    renderBoard.initialize(boardPos, boardSize);
    //renderBoard.transform( boardPos, boardSize / 2.0f);
    bool clicked = false;
    sf::Vector2i clickPosRelative;
    std::chrono::system_clock::time_point timerStart;
    std::chrono::system_clock::time_point timerNow;
    double time = 0.5f;
    double timeSinceLastTurn = 1.0f;
    timerStart = std::chrono::system_clock::now();

    std::vector<int> iterPos = {0};
    bool alreadyMoved = false;
    std::vector<unsigned char> iterPiece = {};

    while (window.isOpen() && !iterPos.empty()) // mainLoop
    {
        timerNow = std::chrono::system_clock::now();
        timeSinceLastTurn = static_cast<std::chrono::duration<double>>(timerNow - timerStart).count();
        if (timeSinceLastTurn > 0.5f){
            moveRecsursives now;
            auto upperlist= myE.recursiveReturn;
            auto list = myE.recursiveReturn;
            for (int pos : iterPos) {
                //std::cout << pos << std::endl;
                upperlist = list;
                now = list.at(pos);
                list = now.later;
            }
            g.setString( std::to_string(now.score) + " | " + std::to_string(now.alpha) + " | " + std::to_string(now.beta));
            //std::cout << moveToString(now.move)<< std::endl;
            if (!alreadyMoved){
                iterPiece.push_back(board.betweenMove(now.move, '.'));
                //has lower move
                if (list.size() > 0){
                    iterPos.push_back(0);
                    //std::cout << "push" << std::endl;
                }
                else{
                    alreadyMoved = true;
                    //std::cout << "moved" << std::endl;
                }
            }
            else {
                board.betweenMove({now.move.to, now.move.from}, iterPiece.back());
                iterPiece.pop_back();
                *(iterPos.end()-1) = iterPos.back() + 1;

                if (iterPos.back() >= upperlist.size()){
                    iterPos.pop_back();
                    //std::cout << "pop" << std::endl;
                }
                else{
                    alreadyMoved = false;
                    //std::cout << "not moved" << std::endl;
                }
            }
            timerStart = std::chrono::system_clock::now();
        }
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto *mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2i clickPos = {mouseButtonPressed->position.x, mouseButtonPressed->position.y};
                    clickPosRelative = {cameraPos.x - clickPos.x, cameraPos.y - clickPos.y};
                    clicked = true;
                }
            } else if (const auto *mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                    sf::Vector2i clickPos = {mouseButtonReleased->position.x, mouseButtonReleased->position.y};
                    clicked = false;
                }
            }
            if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                sf::Vector2i mousePos = {mouseMoved->position.x, mouseMoved->position.y};
                if (clicked) {
                    renderBoard.transform({(float)(clickPosRelative.x + mousePos.x), (float)(clickPosRelative.y + mousePos.y)}, renderBoard.boardSize);
                }
            }
        }
        window.clear(sf::Color::Black);
        renderBoard.draw(0.0f);

        window.draw(g);
        window.display();
    }
}
//*/

int main()
{
    fb::fastboard* board2 = fb::createBoard();
    fb::setBoard(standardBoard, board2);
    board2->activePlayer = fb::enWHITE;
    board2->gameState = fb::ONGOING;
    std::cout << "is valid move: " << fb::isLegalMove( 1 + 7 * 8, 0 + 5 * 8, board2) << std::endl;

    fb::movePiece(1 + 7 * 8, 0 + 5 * 8, fb::enEmpty, board2);
    fb::checkConsistency(board2);
    board.setBoardState(standardBoard);
    std::cout << (fb::isAttackedByOpponent(8, fb::enWHITE, board2) == 1) << std::endl;
    std::cout << (board.isAttackedByOpponent({0, 1}, enPlayers::Black) == 1) << std::endl;
    calcTimes.clear();

    fb::printBoard(board2);
    fb::checkConsistency(board2);
    /*
    float avT = 0.0f;
    for (int i = 0; i < 10; i++){
        myClock.restart();
        for (int j = 0; j < 10000; j++){
            fb::PieceType p = fb::movePiece(4, 20, fb::enEmpty, board2);
            fb::movePiece(20, 4, p, board2);
        }
        calcTimes.push_back(myClock.getElapsedTime());
    }
    for (sf::Time t : calcTimes) {
        avT += t.asSeconds();
    }
    avT /= calcTimes.size();
    std::cout << "fastboard takes on average " << avT << " seconds to calculate.\n";

    calcTimes.clear();
    float avT2 = 0.0f;
    for (int i = 0; i < 10; i++){
        myClock.restart();
        for (int j = 0; j < 10000; j++){
            unsigned char piece = board.betweenMove({{4, 0}, {4, 2}}, '.');
            board.betweenMove({{4, 2}, {4, 0}}, piece);
        }
        calcTimes.push_back(myClock.getElapsedTime());
    }
    for (sf::Time t : calcTimes) {
        avT2 += t.asSeconds();
    }
    avT2 /= calcTimes.size();
    std::cout << "board takes on average " << avT2 << " seconds to calculate.\n";

    std::cout << "time multiplier: " << avT2 / avT << " times faster\n";
    /*
    logInitiate();
    logID(1);
    window.setFramerateLimit(144);
    window.close();

    board.setBoardState(standardBoard);

    myE.startBot(enPlayers::White);
    myE.generalSet(enEvaluators::push, 10.f);
    myE.generalSet(enEvaluators::check, 10.f);
    myE.generalSet(enEvaluators::material, 10.f);
    std::cout << moveToString(myE.calculate()) << std::endl;

    //display();
    logClose();

    logger.results();
    //*/
}
