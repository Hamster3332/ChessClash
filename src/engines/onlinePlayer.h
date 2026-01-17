#ifndef ONLINE_PLAYER_H
#define ONLINE_PLAYER_H

#include <board.h>
#include <renderBoard.h>
#include "playerInterface.cpp"
#include <string>
#include <bits/stdc++.h>

class PlayerServer {
public:
    bool serverReceive = false;
    std::string receivedMessage;
    bool serverReceiveDone = false;

    bool serverSend = false;
    std::string sendMessage;
    bool serverSendDone = false;

    bool serverRun = false;
    std::string myColor;
    bool serverON = false;

    void serverThread();
    void RunServer(enPlayers color);
    void send_message(std::string message);
    bool sendDone();
    void receive_message();
    bool receiveDone();
    bool Ready();
    void Stop();
    std::string received_message();
};

class OnlinePlayer : public PlayerInterface {
public:
    PlayerServer serverObjekt;
    OnlinePlayer(Board& board, RenderBoard& renderer);
    void startBot(enPlayers _color) override;
    bool isReady() override;
    void startTurn(Move LastTurn) override;
    Move calculate() override;
    unsigned char getPromotion() override;
    Move fallback(std::vector<Move> &moves) override;

    ~OnlinePlayer();
    std::thread* ServerThread;

    Move lastMove = {{0, 0}, {0, 0}};
    Move currentMove = {{0, 0}, {0, 0}};
    int sendMoveStep = 0;
    int receiveMoveStep = 0;
    unsigned char promoteTo = 'q';
    bool promoted = false;

    Board* curBoard;
    bool running = false;
    RenderBoard* curRenderer;
    Move promotionDelayedMove = {{-1, -1}, {-1, -1}};
};

#endif
