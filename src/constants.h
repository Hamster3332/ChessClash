#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SFML/Graphics.hpp>

// Constants
inline constexpr sf::Vector2u WINDOW_SIZE = {1200u, 1200u};
inline constexpr sf::Color darkSquareC = sf::Color(78, 64, 42);
inline constexpr sf::Color lightSquareC = sf::Color(204, 173, 124);
inline constexpr sf::Color selectedSquareC = sf::Color(40, 205, 40, 100);
inline constexpr sf::Color possibleMoveC = sf::Color(70, 170, 70, 200);
inline constexpr float boardSize = (float) WINDOW_SIZE.y  * 0.9;
inline unsigned char standardBoard[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};
inline unsigned char checkmateBoard[8][8] = {
    {'.', '.', '.', '.', 'k', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};
inline unsigned char castlingBoard[8][8] = {
    {'r', '.', '.', '.', 'k', '.', '.', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'.', '.', '.', '.', '.', '.', '.', '.'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', '.', '.', '.', 'K', '.', '.', 'R'}
};

inline unsigned char allPieces[13] = {'.', 'k','K','p','P','q','Q','b','B','n','N','r','R'};
inline constexpr int pieceTypeNum = 13;

constexpr int knightDirections[8][2] = {{2, 1}, {1, 2}, {-2, 1}, {-1, 2}, {2, -1}, {1, -2}, {-2, -1}, {-1, -2}};
constexpr int rookDirections[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
constexpr int BishopDirections[4][2] = {{1,1},{-1,-1},{1,-1},{-1,1}};
constexpr int PawnDirections[2][2] = {{1,1},{-1,1}};

#endif
