#include "general.h"
#include "board.h"
#include "chessVector.h"
#include "subGeneral.h"


E_General::E_General(Board& board, RenderBoard& renderer) {
    curBoard = &board;
}

void E_General::generalSet(enEngines engine, float priority){
    priorities.push_back({engine, priority});
}

void E_General::generalReset(){
    priorities.clear();
}

void E_General::startBot(enPlayers _color) {
    color = _color;
}

bool E_General::isReady() {
    return true;
}

void E_General::startTurn(Move LastTurn){

}

Move E_General::calculate() {
    float score;
    moves.clear();
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (curBoard->get({x, y}) != '.' && isWhite(curBoard->get({x, y})) == color) {
                for (Move mv : curBoard->getLegalMoves({x, y})) {
                    score = 0.0f;
                    for (prioEngine engine : priorities) {
                        score += getEngineResult(mv, engine.engine) * engine.priority;
                    }
                    moves.push_back({mv, score});
                }
            }
        }
    }
    selected = moves.front();
    for (scoredMove mv : moves) {
        if (mv.score > selected.score) {
            selected = mv;
        }
    }
    //std::cout << moveToString(selected) << std::endl;

    return selected.move;
}

Move E_General::fallback(std::vector<Move> &moves){
    return {{-1, -1}, {-1, -1}};
}

unsigned char E_General::getPromotion() {
    return 'q';
}


float E_General::getEngineResult(Move mv, enEngines engine){
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
