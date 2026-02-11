#include "soundManager.h"
#include "board.h"
#include <cctype>
#include <iostream>

void SoundManager::load() {
    const std::string sound_path = "assets/sounds/";
    if (!soundBuffers[enSounds::capture].loadFromFile(sound_path + "capture.mp3"))
        std::cerr << "Failed to load capture.mp4.\n";
    if (!soundBuffers[enSounds::move].loadFromFile(sound_path + "move.mp3"))
        std::cerr << "Failed to load move.mp3.\n";
    if (!soundBuffers[enSounds::castling].loadFromFile(sound_path + "castling.mp3"))
        std::cerr << "Failed to load castling.mp3.\n";
    if (!soundBuffers[enSounds::check].loadFromFile(sound_path + "check.mp3"))
        std::cerr << "Failed to load check.mp3.\n";
    if (!soundBuffers[enSounds::checkmate].loadFromFile(sound_path + "checkmate.mp3"))
        std::cerr << "Failed to load checkmate.mp3.\n";
    if (!soundBuffers[enSounds::draw].loadFromFile(sound_path + "draw.mp3"))
        std::cerr << "Failed to load draw.mp3.\n";
}


sf::SoundBuffer& SoundManager::get(enSounds sound) {
    return soundBuffers.at(sound);
}

SoundPlayer::SoundPlayer(SoundManager& sounds) : sound(sounds.soundBuffers[enSounds::castling]) {}


void SoundPlayer::getToMove(Move lastMove, Board& board, SoundManager& sounds) {

    if (std::tolower(board.get(lastMove.from)) == 'k' && lastMove.from.getDistance(lastMove.to).x == 2) {
        //sound.stop();
        sound.setBuffer(sounds.get(enSounds::castling));
    }
    else if (board.get(lastMove.to) == '.') {
        //sound.stop();
        sound.setBuffer(sounds.get(enSounds::move));
    }
    else if (board.get(lastMove.to) != '.') {
        //sound.stop();
        sound.setBuffer(sounds.get(enSounds::capture));
    }
}

void SoundPlayer::getAfterMove(Board& board, SoundManager& sounds) {
    if (board.gameState == GameState::DRAW) {
        //sound.stop();
        sound.setBuffer(sounds.get(enSounds::draw));
    }
    else if (board.gameState != GameState::ONGOING) {
        //sound.stop();
        sound.setBuffer(sounds.get(enSounds::checkmate));
    }
    else if (board.isAttackedByOpponent(board.KPos[board.activePlayer], board.activePlayer)) {
        //sound.stop();
        sound.setBuffer(sounds.get(enSounds::check));
    }
}

void SoundPlayer::play() {
    sound.setVolume(40);
    sound.play();
}
