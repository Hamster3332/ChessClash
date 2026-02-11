#ifndef WINDOW_INTERFACE_H
#define WINDOW_INTERFACE_H
#include <SFML/Graphics.hpp>


class WindowInterface {
public:
    virtual ~WindowInterface() = default;

    virtual void initialize(const sf::Vector2f &pos,const sf::Vector2f &size){}
    virtual void onClick(const sf::Vector2i &pos, bool isClicking){}
    virtual void onMouseMove(const sf::Vector2i &pos){}
    virtual void draw(double timeSinceStart){}

    bool active = true;
    bool drawing = true;
};
#endif
