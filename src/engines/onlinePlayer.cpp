#include "onlinePlayer.h"
#include "board.h"
#include <arpa/inet.h>

// Non-static member function
void PlayerServer::serverThread() {
    std::string Received;
    std::string SendStr;
    char buffer[1024] = {0};

    while (!serverRun) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

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
    SendStr = myColor;
    send(clientSocket, SendStr.c_str(), strlen(SendStr.c_str()), 0);
    std::cout << "Server Running" << std::endl;
    serverON = true;
    while (serverRun) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (serverSend && !serverSendDone){
            //std::cout << "send started" << std::endl;
            send(clientSocket, sendMessage.c_str(), strlen(sendMessage.c_str()), 0);
            serverSendDone = true;
            //std::cout << "send Done" << std::endl;
        }
        if (!serverSend && serverSendDone)
            serverSendDone = false;
        if (serverReceive && !serverReceiveDone){
            std::cout << "rekifed started" << std::endl;
            recv(clientSocket, buffer, sizeof(buffer), 0);
            receivedMessage = buffer;
            serverReceiveDone = true;
            //std::cout << "rekifed" << std::endl;
        }
        if (!serverReceive && serverReceiveDone)
            serverReceiveDone = false;
    }
    std::cout << "Server Stopped" << std::endl;
}

void PlayerServer::RunServer(enPlayers color){
    switch (color) {
        case enPlayers::White:
        myColor = "White";
        break;
        case enPlayers::Black:
        myColor = "Black";
        break;
    }
    serverRun = true;
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

void PlayerServer::Stop() {
    serverRun = false;
}



OnlinePlayer::OnlinePlayer(Board& board, RenderBoard& renderer) {
    curBoard = &board;
    curRenderer = &renderer;
}


void OnlinePlayer::startBot(enPlayers _color){
    ServerThread = new std::thread(&PlayerServer::serverThread, &serverObjekt);
    serverObjekt.RunServer(_color);
    running = true;
}

bool OnlinePlayer::isReady() {
    return serverObjekt.serverON;
}

void OnlinePlayer::startTurn(Move LastTurn){
    promoted = curBoard->isPromotion(LastTurn);
    if(LastTurn.from.x != -1) sendMoveStep = 1;
    receiveMoveStep = 1;
    lastMove = LastTurn;
}

ChessVector StringToMove(std::string s){
    return {s.c_str()[1] - '0', s.c_str()[4] - '0'};
}

Move OnlinePlayer::calculate() {
    if (sendMoveStep != 0){
        //sending Move to Opponent
        switch (sendMoveStep) {
            case 1:
            if (serverObjekt.Ready()) {
                serverObjekt.send_message("["+ std::to_string(lastMove.from.x) +", "+ std::to_string(lastMove.from.y) + "]");
                sendMoveStep += 1;
            }break;
            case 2:
            if (serverObjekt.sendDone()) {
                sendMoveStep += 1;
            }break;
            case 3:
            if (serverObjekt.Ready()) {
                serverObjekt.receive_message();
                sendMoveStep += 1;
            }break;
            case 4:
            if (serverObjekt.receiveDone()) {
                sendMoveStep += 1;
            }break;
            case 5:
            if (serverObjekt.Ready()) {
                serverObjekt.send_message("["+ std::to_string(lastMove.to.x) +", "+ std::to_string(lastMove.to.y) + "]");
                sendMoveStep += 1;
            }break;
            case 6:
            if (serverObjekt.sendDone()) {
                sendMoveStep += 1;
            }break;
            case 7:
            if (serverObjekt.Ready()) {
                serverObjekt.receive_message();
                sendMoveStep += 1;
            }break;
            case 8:
            if (serverObjekt.receiveDone()) {
                if (promoted){
                    sendMoveStep += 1;
                }else{
                    sendMoveStep = 0;
                }
            }break;
            case 9:
            if (serverObjekt.Ready()) {
                std::string opponentPromotion = "D";
                switch(std::tolower(curBoard->get(lastMove.to))) {
                    case 'q':
                    opponentPromotion = "D";
                    break;
                    case 'n':
                    opponentPromotion = "P";
                    break;
                    case 'r':
                    opponentPromotion = "T";
                    break;
                    case 'b':
                    opponentPromotion = "L";
                    break;

                    default:
                    std::cout << "Opponent Promotion Wrong lmao\n" << curBoard->get(lastMove.to) << std::endl;
                    break;
                }
                serverObjekt.send_message(opponentPromotion);
                sendMoveStep += 1;
            }break;
            case 10:
            if (serverObjekt.sendDone()) {
                sendMoveStep = 0;
            }break;
        }
        return {{-1, -1}, {-1, -1}};
    }
    switch (receiveMoveStep) {
        case 0:
        if (serverObjekt.sendDone())receiveMoveStep += 1;
        break;
        case 1:
        if (serverObjekt.Ready()) {
            serverObjekt.receive_message();
            receiveMoveStep += 1;
        }break;
        case 2:
        if (serverObjekt.receiveDone()) {
            std::cout << serverObjekt.received_message() << std::endl;
            currentMove.from = StringToMove(serverObjekt.received_message());
            receiveMoveStep += 1;
        }break;
        case 3:
        if (serverObjekt.Ready()) {
            if (curBoard->existsLegalMove(currentMove.from)){
                serverObjekt.send_message("true");
                receiveMoveStep += 1;
            }else{
                serverObjekt.send_message("false");
                receiveMoveStep = 0;
            }
        }break;
        case 4:
        if (serverObjekt.sendDone())receiveMoveStep += 1;
        break;
        case 5:
        if (serverObjekt.Ready()) {
            serverObjekt.receive_message();
            receiveMoveStep += 1;}
        break;
        case 6:
        if (serverObjekt.receiveDone()) {
            currentMove.to = StringToMove(serverObjekt.received_message());
            receiveMoveStep += 1;
        } break;
        case 7:
        if (serverObjekt.Ready()) {
            if (curBoard->isLegalMove(currentMove.from, currentMove.to) || currentMove.from == currentMove.to){
                serverObjekt.send_message("true");
                receiveMoveStep += 1;
            }else{
                serverObjekt.send_message("false");
                receiveMoveStep = 4;
            }
        }
        case 8:
        if (serverObjekt.sendDone()) {
            std::cout << std::to_string(currentMove.from.x) << "," << std::to_string(currentMove.from.y) << ",to:";
            std::cout << std::to_string(currentMove.to.x) << "," << std::to_string(currentMove.to.y) << std::endl;
            if (currentMove.from == currentMove.to){
                receiveMoveStep = 1;
            } else if (curBoard->isPromotion(currentMove)){
                receiveMoveStep = 18;
                std::cout << "promoting";
            }else{
                receiveMoveStep += 1;
                return currentMove;
            }
        }
        break;

        case 18:
        if (serverObjekt.Ready()) {
            serverObjekt.receive_message();
            receiveMoveStep += 1;}
        break;
        case 19:
        if (serverObjekt.receiveDone()) {
            std::cout << "promotedTo" << serverObjekt.received_message() << std::endl;
            switch(serverObjekt.received_message().c_str()[0]) {
                case 'D':
                promoteTo = 'q';
                break;
                case 'P':
                promoteTo = 'n';
                break;
                case 'T':
                promoteTo = 'r';
                break;
                case 'L':
                promoteTo = 'b';
                break;

                default:
                    std::cout << "Promotion Wrong";
                    break;
            }
            receiveMoveStep += 1;
            return currentMove;
        }
        break;
    }
    return {{-1, -1}, {-1, -1}};
}

unsigned char OnlinePlayer::getPromotion(){
    return promoteTo;
}

Move OnlinePlayer::fallback(std::vector<Move> &moves){
    return {{-1, -1}, {-1, -1}};
}

OnlinePlayer::~OnlinePlayer() {
    if (running){
        ServerThread->join();
        delete ServerThread;
    }
}
