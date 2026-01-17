#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio/Sound.hpp>
#include <chessVector.h>
#include <board.h>
#include <map>
#include <SFML/Audio.hpp>


enum class enSounds {
    move, capture, castling, check, checkmate, draw
};

class SoundManager {
public:
    std::map<enSounds, sf::SoundBuffer> soundBuffers;
    void load();
    sf::SoundBuffer& get(enSounds sound);
};

class SoundPlayer {
public:
    SoundPlayer(SoundManager& sounds);
    void getToMove(Move lastMove, Board& board, SoundManager& sounds);
    void getAfterMove(Board& board, SoundManager& sounds);
    void play();

    sf::Sound sound;
};

#endif
