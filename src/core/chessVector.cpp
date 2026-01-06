#include "chessVector.h"
#include <algorithm>
#include <cstdlib>

ChessVector::ChessVector(int _x, int _y) : x(_x), y(_y) {

}

ChessVector ChessVector::getTo(ChessVector second) const {
    return getTo(second.x, second.y);
}

ChessVector ChessVector::getTo(int _x, int _y) const {
    return ChessVector(_x - x, _y - y);
}

ChessVector ChessVector::getDistance(ChessVector second) const {
    return getDistance(second.x, second.y);
}

ChessVector ChessVector::getDistance(int _x, int _y) const {
    return ChessVector(std::abs(_x - x), std::abs(_y - y));
}

ChessVector ChessVector::normToInt() const {
    return ChessVector(std::clamp(x, -1, 1), std::clamp(y, -1, 1));
}

ChessVector ChessVector::scaledown(int amount ) const {
    return ChessVector(x / amount, y / amount);
}

int ChessVector::manhattanDist() const {
    return std::abs(x) + std::abs(y);
}

int ChessVector::matinnatianDist() const {
    return std::max(std::abs(x), std::abs(y));
}




//Operators
//


ChessVector ChessVector::operator *(int amount) const {
    return ChessVector(x * amount, y * amount);
}

ChessVector ChessVector::scale(int amount ) const {
    return ChessVector(x * amount, y * amount);
}

ChessVector ChessVector::operator +(ChessVector second) const {
    return ChessVector(second.x + x, second.y + y);
}

ChessVector ChessVector::add(ChessVector second) const {
    return ChessVector(second.x + x, second.y + y);
}

ChessVector ChessVector::add(int _x, int _y) const {
    return ChessVector(_x + x, _y + y);
}

ChessVector ChessVector::operator -(ChessVector second) const {
    return ChessVector(x - second.x, y - second.y);
}

ChessVector ChessVector::sub(ChessVector second) const {
    return ChessVector(x - second.x, y - second.y);
}

ChessVector ChessVector::sub(int _x, int _y) const {
    return ChessVector(x - _x, y - _y);
}

bool ChessVector::operator ==(ChessVector second) const {
    return x == second.x && y == second.y;
}

bool ChessVector::equal(ChessVector second ) const {
    return x == second.x && y == second.y;
}

bool ChessVector::equal(int _x, int _y) const {
    return x == _x && y == _y;
}


bool ChessVector::operator !=(ChessVector second) const {
    return x != second.x || y != second.y;
}

bool ChessVector::notEqual(ChessVector second ) const {
    return x != second.x || y != second.y;
}

bool ChessVector::notEqual(int _x, int _y) const {
    return x != _x || y != _y;
}
