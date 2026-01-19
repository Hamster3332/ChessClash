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

#include <board.h>
#include <logger.h>
#include <general.h>

#include <soundManager.h>
#include "subGeneral.h"

#include <onlinePlayer.h>
#include <player.h>
#include <renderBoard.h>

inline unsigned char test[8][8] = {
    {'.', '.', '.', '.', 'k', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', 'K', '.', '.', '.'}
};

sf::Clock myClock;
std::vector<sf::Time> calcTimes = {};

auto window = sf::RenderWindow(sf::VideoMode(WINDOW_SIZE), "Clash Royal Deluxe"); // create Window
Board board;
sf::Vector2f boardPos = {(float)WINDOW_SIZE.y * 0.05f, (float)WINDOW_SIZE.y * 0.05f};
RenderBoard renderBoard(window, boardPos);
E_Recursive myE(board, renderBoard);

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

int main()
{
    logInitiate();
    logID(1);
    window.setFramerateLimit(144);

    board.setBoardState(test);
    myE.startBot(enPlayers::White);
    myE.generalSet(enEvaluators::push, 10.f);
    myE.generalSet(enEvaluators::check, 10.f);
    myE.generalSet(enEvaluators::material, 10.f);
    myE.calculate();
    //layoutTest(test);
    //layoutTest(standardBoard);
    //layoutTest(checkmateBoard);
    //layoutTest(castlingBoard);

    logClose();
}
