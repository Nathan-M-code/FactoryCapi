#ifndef MUSICMANAGER_H_INCLUDED
#define MUSICMANAGER_H_INCLUDED

#include <SFML/Audio.hpp>
#include "OptionManager.h"

class MusicManager{
public:
    enum MusicType{day1,day2,day3};


    MusicManager();
    ~MusicManager();
    void loadPaths();

    void switchMusic(MusicType musicType);
private:
    sf::Music _music;
    std::vector<std::string> _listPathMusic;
};

#endif // MUSICMANAGER_H_INCLUDED
