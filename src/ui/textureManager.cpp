#include "textureManager.h"
#include <iostream>

void TextureManager::load() {
        if (!textures['p'].loadFromFile("assets/b_pawn.png"))
            std::cerr << "Failed to load b_pawn.png." << std::endl;
        if (!textures['P'].loadFromFile("assets/w_pawn.png"))
            std::cerr << "Failed to load w_pawn.png." << std::endl;
        if (!textures['n'].loadFromFile("assets/b_knight.png"))
            std::cerr << "Failed to load b_knight.png." << std::endl;
        if (!textures['N'].loadFromFile("assets/w_knight.png"))
            std::cerr << "Failed to load w_knight.png." << std::endl;
        if (!textures['b'].loadFromFile("assets/b_bishop.png"))
            std::cerr << "Failed to load b_bishop.png." << std::endl;
        if (!textures['B'].loadFromFile("assets/w_bishop.png"))
            std::cerr << "Failed to load w_bishop.png." << std::endl;
        if (!textures['r'].loadFromFile("assets/b_rook.png"))
            std::cerr << "Failed to load b_rook.png." << std::endl;
        if (!textures['R'].loadFromFile("assets/w_rook.png"))
            std::cerr << "Failed to load w_rook.png." << std::endl;
        if (!textures['q'].loadFromFile("assets/b_queen.png"))
            std::cerr << "Failed to load b_queen.png." << std::endl;
        if (!textures['Q'].loadFromFile("assets/w_queen.png"))
            std::cerr << "Failed to load w_queen.png." << std::endl;
        if (!textures['k'].loadFromFile("assets/b_king.png"))
            std::cerr << "Failed to load b_king.png." << std::endl;
        if (!textures['K'].loadFromFile("assets/w_king.png"))
            std::cerr << "Failed to load w_king.png." << std::endl;
    }

sf::Texture& TextureManager::get(char piece) {
    return textures.at(piece);
}
