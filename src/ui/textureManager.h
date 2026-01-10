#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <SFML/Graphics/Texture.hpp>
#include <map>

class TextureManager {
public:
    std::map<char, sf::Texture> textures;
    void load();
    sf::Texture& get(char piece);
};


#endif