#include "SoundLoader.h"

using namespace std;

std::vector<sf::SoundBuffer*> SoundLoader::m_listSoundBuffer = {};

void SoundLoader::init(){
    //TextureLoader::ajouterSoundBuffer("data/sound/");
}

void SoundLoader::ajouterSoundBuffer(std::string path){
    sf::SoundBuffer *t = new sf::SoundBuffer;
    t->loadFromFile(path);
    m_listSoundBuffer.push_back(t);
}

void SoundLoader::freeRAM(){
    for(unsigned int i=0;i<m_listSoundBuffer.size();i++){
        delete m_listSoundBuffer[i];
        m_listSoundBuffer[i] = 0;
    }
}
