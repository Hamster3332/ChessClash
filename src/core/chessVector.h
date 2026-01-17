#ifndef CHESS_VECTOR_H
#define CHESS_VECTOR_H
#include <string>


class ChessVector {
public:
    int x = 0;
    int y = 0;
    ChessVector(int _x, int _y);

    ChessVector getTo(ChessVector second ) const;
    ChessVector getTo(int _x, int _y) const;

    ChessVector getDistance(ChessVector second ) const;
    ChessVector getDistance(int _x, int _y) const;

    ChessVector normToInt() const;
    ChessVector operator *(int amount) const;
    ChessVector operator +(ChessVector second) const;
    ChessVector operator -(ChessVector second) const;
    bool operator ==(ChessVector second) const;
    bool operator !=(ChessVector second) const;
    ChessVector scale(int amount ) const;
    ChessVector scaledown(int amount ) const;

    ChessVector add(ChessVector second ) const;
    ChessVector add(int _x, int _y) const;

    ChessVector sub(ChessVector second ) const;
    ChessVector sub(int _x, int _y) const;

    int manhattanDist() const;
    int matinnatianDist() const;
    float absoluteDist() const;

    bool equal(ChessVector second ) const;
    bool equal(int _x, int _y) const;

    bool notEqual(ChessVector second ) const;
    bool notEqual(int _x, int _y) const;
private:

};

struct Move {
    ChessVector from;
    ChessVector to;
};

struct scoredMove {
    Move move;
    float score;
};



std::string moveToString(Move move);
std::string moveToString(scoredMove move);

#endif
