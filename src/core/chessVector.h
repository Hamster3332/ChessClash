#ifndef CHESS_VECTOR_H
#define CHESS_VECTOR_H


class chessVector {
public:
    int x = 0;
    int y = 0;
    chessVector(int _x, int _y);

    chessVector getTo(chessVector second ) const;
    chessVector getTo(int _x, int _y) const;

    chessVector getDistance(chessVector second ) const;
    chessVector getDistance(int _x, int _y) const;

    chessVector normToInt() const;
    chessVector operator *(int amount) const;
    chessVector operator +(chessVector second) const;
    chessVector operator -(chessVector second) const;
    bool operator ==(chessVector second) const;
    chessVector scale(int amount ) const;
    chessVector scaledown(int amount ) const;

    chessVector add(chessVector second ) const;
    chessVector add(int _x, int _y) const;

    chessVector sub(chessVector second ) const;
    chessVector sub(int _x, int _y) const;

    int manhattanDist() const;
    int matinnatianDist() const;

    bool equal(chessVector second ) const;
    bool equal(int _x, int _y) const;

private:

};






#endif
