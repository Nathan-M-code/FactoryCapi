#include "MusicManager.h"

MusicManager::MusicManager(){
    _music.setLoop(true);
}
MusicManager::~MusicManager(){}

void MusicManager::loadPaths(){
    _listPathMusic.push_back("data/music/Day 1.ogg");
    _listPathMusic.push_back("data/music/Day 2.ogg");
    _listPathMusic.push_back("data/music/Day 3.ogg");
    _listPathMusic.push_back("data/music/Day 4.ogg");
    _listPathMusic.push_back("data/music/Day 5.ogg");
    _listPathMusic.push_back("data/music/Day 6.ogg");
    _listPathMusic.push_back("data/music/Day 7.ogg");
    _listPathMusic.push_back("data/music/Day 8.ogg");
    _listPathMusic.push_back("data/music/Day 9.ogg");
    _listPathMusic.push_back("data/music/Day 10.ogg");
    _listPathMusic.push_back("data/music/Day 11.ogg");
    _listPathMusic.push_back("data/music/Night 1.ogg");
    _listPathMusic.push_back("data/music/Night 2.ogg");
    _listPathMusic.push_back("data/music/Night 3.ogg");
    _listPathMusic.push_back("data/music/Night 4.ogg");
    _listPathMusic.push_back("data/music/Night 5.ogg");
    _listPathMusic.push_back("data/music/Intro.ogg");
}

void MusicManager::switchMusic(MusicType musicType){
    if(musicType == day1)
        _music.openFromFile(_listPathMusic[0]);
    else if(musicType == day2)
        _music.openFromFile(_listPathMusic[1]);
    else if(musicType == day3)
        _music.openFromFile(_listPathMusic[2]);
    else if(musicType == day4)
        _music.openFromFile(_listPathMusic[3]);
    else if(musicType == day5)
        _music.openFromFile(_listPathMusic[4]);
    else if(musicType == day6)
        _music.openFromFile(_listPathMusic[5]);

    _music.setVolume(OptionManager::getMusicVolume()*100);
    _music.play();
}
