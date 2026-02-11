#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio.hpp>
#include <map>

class TextureManager {
public:
    std::map<char, sf::Texture> piece_textures;
    std::map<std::string, sf::Texture> ui_assets;
    void load();
    sf::Texture& get(char piece);
    sf::Texture& get_ui_asset(std::string asset_name);
};

#endif
