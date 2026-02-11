//
// Created by malicopter on 11.02.26.
//

#include "menu.h"

#include <iostream>


MainMenu::MainMenu(sf::RenderWindow& w, TextureManager &Textures) : window(w), textureManager(Textures) {}

void MainMenu::initialize(const sf::Vector2f &pos, const sf::Vector2f &size) {
    windowPos = pos;
    windowSize = size;
}

void MainMenu::draw(const double timeSinceStart) {
    sf::Texture &texture = textureManager.get_ui_asset("start_button");
    sf::Vector2u textureSize = texture.getSize();
    static sf::Sprite sprite(texture);
    sprite.setTexture(texture);
    sprite.setPosition({windowPos.x + startGamePos.x * windowSize.x,
                        windowPos.y + startGamePos.y * windowSize.y});
    sprite.setScale({(startGameSize.x * windowSize.x) / static_cast<float>(textureSize.x),
                            (startGameSize.y * windowSize.y) / static_cast<float>(textureSize.y)});
    window.draw(sprite);
}

void MainMenu::onMouseMove(const sf::Vector2i &mousePos) {

}

void MainMenu::onClick(const sf::Vector2i &clickPos, bool isClicking) {
    enableGame = true;
    showGame = true;
    drawing = false;
    active = false;
}