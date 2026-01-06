#include "chessVector.h"
#include <algorithm>
#include <cstdlib>

chessVector::chessVector(int _x, int _y) : x(_x), y(_y) {

}

chessVector chessVector::getTo(chessVector second) const {
    return getTo(second.x, second.y);
}

chessVector chessVector::getTo(int _x, int _y) const {
    return chessVector(_x - x, _y - y);
}

chessVector chessVector::getDistance(chessVector second) const {
    return getDistance(second.x, second.y);
}

chessVector chessVector::getDistance(int _x, int _y) const {
    return chessVector(std::abs(_x - x), std::abs(_y - y));
}

chessVector chessVector::normToInt() const {
    return chessVector(std::clamp(x, -1, 1), std::clamp(y, -1, 1));
}

chessVector chessVector::scaledown(int amount ) const {
    return chessVector(x / amount, y / amount);
}

int chessVector::manhattanDist() const {
    return std::abs(x) + std::abs(y);
}

int chessVector::matinnatianDist() const {
    return std::max(std::abs(x), std::abs(y));
}




//Operators
// 


chessVector chessVector::operator *(int amount) const {
    return chessVector(x * amount, y * amount);
}

chessVector chessVector::scale(int amount ) const {
    return chessVector(x * amount, y * amount);
}

chessVector chessVector::operator +(chessVector second) const {
    return chessVector(second.x + x, second.y + y);
}

chessVector chessVector::add(chessVector second) const {
    return chessVector(second.x + x, second.y + y);
}

chessVector chessVector::add(int _x, int _y) const {
    return chessVector(_x + x, _y + y);
}

chessVector chessVector::operator -(chessVector second) const {
    return chessVector(x - second.x, y - second.y);
}

chessVector chessVector::sub(chessVector second) const {
    return chessVector(x - second.x, y - second.y);
}

chessVector chessVector::sub(int _x, int _y) const {
    return chessVector(x - _x, y - _y);
}

bool chessVector::operator ==(chessVector second) const {
    return x == second.x && y == second.y;
}

bool chessVector::equal(chessVector second ) const {
    return x == second.x && y == second.y;
}

bool chessVector::equal(int _x, int _y) const {
    return x == _x && y == _y;
}
