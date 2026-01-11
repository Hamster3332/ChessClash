#include "onlinePlayer.h"
#include <asm-generic/errno.h>
#include <chrono>
#include <arpa/inet.h>
#include <iostream>
#include <ostream>
#include <string>

// Non-static member function
void PlayerServer::serverThread() {
    std::string Received;
    std::string SendStr;
    char buffer[1024] = {0};

    bool clientReady;

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(25565);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int result = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 1);
    std::cout << "Server Listening" << std::endl;
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    recv(clientSocket, buffer, sizeof(buffer), 0);
    Received = buffer;
    SendStr = "OK";
    send(clientSocket, SendStr.c_str(), strlen(SendStr.c_str()), 0);
    SendStr = "White2";
    send(clientSocket, SendStr.c_str(), strlen(SendStr.c_str()), 0);
    std::cout << "Server Running" << std::endl;
    serverON = true;
    while (serverRun) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (serverSend && !serverSendDone){
            std::cout << "send started" << std::endl;
            send(clientSocket, sendMessage.c_str(), strlen(sendMessage.c_str()), 0);
            serverSendDone = true;
            std::cout << "send Done" << std::endl;
        }
        if (!serverSend && serverSendDone)
            serverSendDone = false;
        if (serverReceive && !serverReceiveDone){
            std::cout << "rekifed started" << std::endl;
            recv(clientSocket, buffer, sizeof(buffer), 0);
            receivedMessage = buffer;
            serverReceiveDone = true;
            std::cout << "rekifed" << std::endl;
        }
        if (!serverReceive && serverReceiveDone)
            serverReceiveDone = false;
    }
    std::cout << "Server Stopped" << std::endl;
}

bool PlayerServer::Ready(){
    return !serverSendDone && !serverReceiveDone;
}
void PlayerServer::send_message(std::string message){
    if (!serverSendDone){
        serverSend = true;
        sendMessage = message;
    }else{
        std::cout << "Server send Error" << std::endl;
    }
}

bool PlayerServer::sendDone(){
    if (serverSendDone){
        serverSend = false;
        return true;
    }
    return false;
}

void PlayerServer::receive_message(){
    if (!serverReceiveDone){
        serverReceive = true;
    }else{
        std::cout << "Server receive Error" << std::endl;
    }
}
bool PlayerServer::receiveDone(){
    if (serverReceiveDone){
        serverReceive = false;
        return true;
    }
    return false;
}
std::string PlayerServer::received_message(){
    return receivedMessage;
}

void PlayerServer::Stop(){
    serverRun = false;
}



OnlinePlayer::OnlinePlayer(Board& board, RenderBoard& renderer) {
    curBoard = &board;
    curRenderer = &renderer;
    ServerThread = new std::thread(&PlayerServer::serverThread, &serverObjekt);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void OnlinePlayer::startTurn(Move LastTurn){
    sendMoveStep = 1;
    receiveMoveStep = 0;
    lastMove = LastTurn;
}

ChessVector StringToMove(std::string s){
    return {s.c_str()[1] - '0', s.c_str()[4] - '0'};
}

Move OnlinePlayer::calculate(){
    if (sendMoveStep != 0){
        //sending Move to Opponent
        switch (sendMoveStep) {
            case 1:
            if (serverObjekt.Ready()){
                std::cout << "send Move1" << std::endl;
                serverObjekt.send_message("["+ std::to_string(lastMove.from.x) +", "+ std::to_string(lastMove.from.y) + "]");
                sendMoveStep += 1;
            }break;
            case 2:
            if (serverObjekt.sendDone()){
                std::cout << "send Move2" << std::endl;
                sendMoveStep += 1;
            }break;
            case 3:
            if (serverObjekt.Ready()){
                std::cout << "send Move3" << std::endl;
                serverObjekt.receive_message();
                sendMoveStep += 1;
            }break;
            case 4:
            if (serverObjekt.receiveDone()){
                std::cout << "send Move4" << serverObjekt.received_message() << std::endl;
                sendMoveStep += 1;
            }break;
            case 5:
            if (serverObjekt.Ready()){
                std::cout << "send Move5" << std::endl;
                serverObjekt.send_message("["+ std::to_string(lastMove.to.x) +", "+ std::to_string(lastMove.to.y) + "]");
                sendMoveStep += 1;
            }break;
            case 6:
            if (serverObjekt.sendDone()){
                std::cout << "send Move6" << std::endl;
                sendMoveStep += 1;
            }break;
            case 7:
            if (serverObjekt.Ready()){
                serverObjekt.receive_message();
                sendMoveStep += 1;
            }break;
            case 8:
            if (serverObjekt.receiveDone()){
                sendMoveStep = 0;
            }break;
        }
        return {{-1, -1}, {-1, -1}};
    }
    switch (receiveMoveStep) {
        case 0:
        if (serverObjekt.Ready()){
            serverObjekt.receive_message();
            receiveMoveStep += 1;
        }break;
        case 1:
        if (serverObjekt.receiveDone()){
            std::cout << serverObjekt.received_message() << std::endl;
            currentMove.from = StringToMove(serverObjekt.received_message());
            receiveMoveStep += 1;
        }break;
        case 2:
        if (serverObjekt.Ready()){
            serverObjekt.send_message("true");
            receiveMoveStep += 1;
        }break;
        case 3:
        if (serverObjekt.sendDone())receiveMoveStep += 1;
        break;
        case 4:
        if (serverObjekt.Ready()){
            serverObjekt.receive_message();
            receiveMoveStep += 1;}
        break;
        case 5:
        if (serverObjekt.receiveDone()){
            std::cout << serverObjekt.received_message() << std::endl;
            currentMove.to = StringToMove(serverObjekt.received_message());
            receiveMoveStep += 1;
        }break;
        case 6:
        if (serverObjekt.Ready()){
            serverObjekt.send_message("true");
            receiveMoveStep += 1;
        }
        case 7:
        if (serverObjekt.sendDone()){
            receiveMoveStep += 1;
            std::cout << std::to_string(currentMove.from.x) << "," << std::to_string(currentMove.from.y) << ",to:";
            std::cout << std::to_string(currentMove.to.x) << "," << std::to_string(currentMove.to.y) << std::endl;
            curRenderer->movePiece(currentMove, *curBoard, 'q');
            return currentMove;
        }
        break;
    }
    return {{-1, -1}, {-1, -1}};
}

bool OnlinePlayer::isReady() {
    return serverObjekt.serverON;
}

OnlinePlayer::~OnlinePlayer() {
    ServerThread->join();
    delete ServerThread;
}
