#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <SFML/Graphics.hpp>

// Constants
inline constexpr sf::Vector2u WINDOW_SIZE = {2000u, 1200u};
inline constexpr sf::Color darkSquareC = sf::Color(78, 64, 42);
inline constexpr sf::Color lightSquareC = sf::Color(204, 173, 124);
inline constexpr sf::Color selectedSquareC = sf::Color(40, 205, 40, 100);
inline constexpr float boardSize = (float) WINDOW_SIZE.y  * 0.9;


#endif