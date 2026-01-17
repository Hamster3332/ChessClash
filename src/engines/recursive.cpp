#include "recursive.h"
#include "board.h"
#include "chessVector.h"
#include "logger.h"
#include "subGeneral.h"
#include <string>


E_Recursive::E_Recursive(Board& board, RenderBoard& renderer) {
    curBoard = &board;
}

void E_Recursive::generalSet(enEvaluators engine, float priority){
    priorities.push_back({engine, priority});
}

void E_Recursive::generalSet(enEngines engine, float priority){
    prioritiesEngine.push_back({engine, priority});
}

void E_Recursive::generalReset(){
    priorities.clear();
    prioritiesEngine.clear();
}

void E_Recursive::startBot(enPlayers _color) {
    color = _color;
}

bool E_Recursive::isReady() {
    return true;
}

void E_Recursive::startTurn(Move LastTurn){

}

scoredMove E_Recursive::getBestMove() {
    scoredMove BestMove = {{{-1, -1}, {-1, -1}}, -1000.0f};
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (curBoard->get({x, y}) != '.' && isWhite(curBoard->get({x, y})) == color) {
                for (Move mv : curBoard->getLegalMoves({x, y})) {
                    score = 0.0f;
                    for (prioEvaluators engine : priorities) {
                        score += getEvaluatorResult(engine.engine) * engine.priority;
                    }
                    if (score > BestMove.score){
                        BestMove = {mv, score};
                    }
                }
            }
        }
    }

    return BestMove;
}

float E_Recursive::evaluateRecursive(Move mv, enPlayers Player, int levels) {
    logEnter("evaluateRecursive " + moveToString(mv) +"|"+ std::to_string(Player) +"|"+ std::to_string(levels), 1);
    if (levels == 0){
        float score = 0.0f;
        for (prioEvaluators engine : priorities) {
            score += getEvaluatorResult(engine.engine) * engine.priority;
        }
        logExit("Skibidi Matinnate forgort this :skull:", 1);
        return score;
    }
    float reverser = -1.f;
    if (Player == color)//get Best
        reverser = 1.f;
    float maxima = 0.0f;
    bool found = false;
    unsigned char betweenMoved;
    for (int y1 = 0; y1 < 8; ++y1) {
        for (int x1 = 0; x1 < 8; ++x1) {
            unsigned char p1 = curBoard->get({x1, y1});
            if (p1 != '.' && isWhite(p1) == Player) {
                for (Move mv : curBoard->getLegalMoves({x1, y1})) {
                    betweenMoved = curBoard->betweenMove(mv);
                    float score = evaluateRecursive(mv, otherPlayer(Player), levels - 1);
                    if (score * reverser > maxima * reverser || !found){
                        maxima = score;
                        found = true;
                    }
                    curBoard->betweenMove({mv.to, mv.from}, betweenMoved);
                }
            }
        }
    }
    logExit(std::to_string(maxima), 1);
    return maxima;
}


Move E_Recursive::calculate() {
    scoredMove BestMove = {{{-1, -1}, {-1, -1}}, -10000000.0f};
    float score;
    bool found = false;
    unsigned char p1, bp1;
    int y1,x1;
    for (y1 = 0; y1 < 8; ++y1) {
        for (x1 = 0; x1 < 8; ++x1) {
            p1 = curBoard->get({x1, y1});
            if (p1 != '.' && isWhite(p1) == color) {
                for (Move mv : curBoard->getLegalMoves({x1, y1})) {
                    bp1 = curBoard->betweenMove(mv);
                    score = evaluateRecursive(mv, otherPlayer(color), 3);
                    curBoard->betweenMove({mv.to, mv.from}, bp1);
                    for (prioEngine engine : prioritiesEngine) {
                        score += getEngineResult(mv, engine.engine) * engine.priority;
                    }
                    test.push_back({mv, score});
                    if ((score > BestMove.score && score ) || !found) {
                        BestMove.score = score;
                        BestMove.move = mv;
                        found = true;
                    }
                }
            }
        }
    }
    return BestMove.move;
}

Move E_Recursive::fallback(std::vector<Move> &moves){
    return {{-1, -1}, {-1, -1}};
}

unsigned char E_Recursive::getPromotion() {
    return 'q';
}

float E_Recursive::getEngineResult(Move mv, enEngines engine){
    switch (engine) {
        case enEngines::kamikaze:
        return sub_E_Kamikaze(mv, curBoard, color);
        case enEngines::random:
        return sub_E_Random(mv, curBoard, color);
        case enEngines::unKamikaze:
        return sub_E_UnKamikaze(mv, curBoard, color);
        case enEngines::bandit:
        return sub_E_Bandit(mv, curBoard, color);
        case enEngines::capture:
        return sub_E_Capture(mv, curBoard, color);
        case enEngines::unBandit:
        return sub_E_UnBandit(mv, curBoard, color);
        case enEngines::cuddle:
        return sub_E_Cuddle(mv, curBoard, color);
        case enEngines::enPassant:
        return sub_EnPassant(mv, curBoard, color);
        case enEngines::promote:
        return sub_Promote(mv, curBoard, color);
        case enEngines::badSchoolKid:
        return sub_E_BadSchoolKid(mv, curBoard, color);
    }
    return 0.0f;
}

float E_Recursive::getEvaluatorResult(enEvaluators engine){
    switch (engine) {
        case enEvaluators::push:
        return sub_Ev_Push(curBoard, color);
        case enEvaluators::check:
        return sub_Ev_Check(curBoard, color);
        case enEvaluators::material:
        return sub_Ev_Material(curBoard, color);
    }
    return 0.0f;
}
