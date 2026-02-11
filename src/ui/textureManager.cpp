#include "textureManager.h"
#include <iostream>

void TextureManager::load() {
    const std::string textures_path = "assets/textures/";
    const std::string fail_message = "Failed to load ";
    if (!piece_textures['p'].loadFromFile(textures_path + "pieces/b_pawn.png"))
        std::cerr << fail_message << textures_path << "pieces/b_pawn.png" << std::endl;
    if (!piece_textures['P'].loadFromFile(textures_path + "pieces/w_pawn.png"))
        std::cerr << fail_message << "pieces/w_pawn.png" << std::endl;
    if (!piece_textures['n'].loadFromFile(textures_path + "pieces/b_knight.png"))
        std::cerr << fail_message << "pieces/b_knight.png" << std::endl;
    if (!piece_textures['N'].loadFromFile(textures_path + "pieces/w_knight.png"))
        std::cerr << fail_message << "pieces/w_knight.png" << std::endl;
    if (!piece_textures['b'].loadFromFile(textures_path + "pieces/b_bishop.png"))
        std::cerr << fail_message << "pieces/b_bishop.png" << std::endl;
    if (!piece_textures['B'].loadFromFile(textures_path + "pieces/w_bishop.png"))
        std::cerr << fail_message << "pieces/w_bishop.png" << std::endl;
    if (!piece_textures['r'].loadFromFile(textures_path + "pieces/b_rook.png"))
        std::cerr << fail_message << "pieces/b_rook.png" << std::endl;
    if (!piece_textures['R'].loadFromFile(textures_path + "pieces/w_rook.png"))
        std::cerr << fail_message << "pieces/w_rook.png" << std::endl;
    if (!piece_textures['q'].loadFromFile(textures_path + "pieces/b_queen.png"))
        std::cerr << fail_message << "pieces/b_queen.png" << std::endl;
    if (!piece_textures['Q'].loadFromFile(textures_path + "pieces/w_queen.png"))
        std::cerr << fail_message << "pieces/w_queen.png" << std::endl;
    if (!piece_textures['k'].loadFromFile(textures_path + "pieces/b_king.png"))
        std::cerr << fail_message << "pieces/b_king.png" << std::endl;
    if (!piece_textures['K'].loadFromFile(textures_path + "pieces/w_king.png"))
        std::cerr << fail_message << "pieces/w_king.png" << std::endl;


    if (!ui_assets["start_button"].loadFromFile(textures_path + "ui/start_button.png"))
        std::cerr << fail_message << "ui/start_button.png" << std::endl;
}

sf::Texture& TextureManager::get(char piece) {
    return piece_textures.at(piece);
}

sf::Texture& TextureManager::get_ui_asset(std::string asset_name) {
    return ui_assets.at(asset_name);
}
