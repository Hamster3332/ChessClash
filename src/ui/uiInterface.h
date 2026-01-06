#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Window.hpp>

class UIElement {
    virtual void draw(sf::Window &window) = 0;
    virtual void onClick(sf::Window &window, sf::Vector2i clickPos) = 0;
};

#endif