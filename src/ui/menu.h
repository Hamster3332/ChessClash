//
// Created by malicopter on 11.02.26.
//

#ifndef CHESSCLASH_MENU_H
#define CHESSCLASH_MENU_H
#include <textureManager.h>
#include "windowInterface.cpp"

class MainMenu : public WindowInterface {
public:
    explicit MainMenu(sf::RenderWindow& w, TextureManager &textureManager);
    void initialize(const sf::Vector2f &pos, const sf::Vector2f &size) override;
    void draw(const double timeSinceStart) override;
    void onMouseMove(const sf::Vector2i &mousePos) override;
    void onClick(const sf::Vector2i &clickPos, bool isClicking) override;

    const sf::Vector2f startGamePos = {0.25f, 0.25f};
    const sf::Vector2f startGameSize = {0.5f, 0.5f};

    bool enableGame = false;
    bool showGame = false;

    sf::RenderWindow &window;
    TextureManager &textureManager;

    sf::Vector2f windowPos = {};
    sf::Vector2f windowSize = {};
};

#endif //CHESSCLASH_MENU_H

