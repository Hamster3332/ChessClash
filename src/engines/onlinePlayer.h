#ifndef ONLINE_PLAYER_H
#define ONLINE_PLAYER_H

#include "../core/board.h"
#include "../ui/boardRender.h"
#include <string>
#include <thread>
#include <bits/stdc++.h>
#include <cstring>

class PlayerServer {
public:
    bool serverReceive = false;
    std::string receivedMessage;
    bool serverReceiveDone = false;

    bool serverSend = false;
    std::string sendMessage;
    bool serverSendDone = false;

    bool serverRun = true;
    bool serverON = false;
    void serverThread();
    void send_message(std::string message);
    bool sendDone();
    void receive_message();
    bool receiveDone();
    bool Ready();
    void Stop();
    std::string received_message();
};

class OnlinePlayer {
public:
    PlayerServer serverObjekt;
    OnlinePlayer(Board& board, RenderBoard& renderer);
    ~OnlinePlayer();
    void startTurn(Move LastTurn);
    Move calculate();
    bool isReady();
    std::thread* ServerThread;

    Move lastMove = {{0, 0}, {0, 0}};
    Move currentMove = {{0, 0}, {0, 0}};
    int sendMoveStep = 0;
    int receiveMoveStep = 0;

    Board* curBoard;
    RenderBoard* curRenderer;
    Move promotionDelayedMove = {{-1, -1}, {-1, -1}};
};

#endif
