#ifndef SOUNDLOADER_H_INCLUDED
#define SOUNDLOADER_H_INCLUDED

#include <SFML/Audio.hpp>
#include "utile.h"

class SoundLoader{
public:
    static void init();
    static void ajouterSoundBuffer(std::string path);

    static void freeRAM();

private:
    static std::vector<sf::SoundBuffer*> m_listSoundBuffer;
};


#endif // SOUNDLOADER_H_INCLUDED
