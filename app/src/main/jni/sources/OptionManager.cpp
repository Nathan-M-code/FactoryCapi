#include "OptionManager.h"

using namespace std;

float OptionManager::speedCam = 15.0;
float OptionManager::musicVolume = 1.0;
float OptionManager::fxVolume = 1.0;
sf::Vector2i OptionManager::screenSize(0,0);

bool OptionManager::loadOption(AAssetManager* assetManager){
    screenSize = sf::Vector2i(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

    vector<string> lines = Utile::getLinesFromFile("data/save/option.txt");

    if(lines.empty())
        return false;

    for(auto &line : lines){
        size_t current = 0;
        current = line.find_first_of(":");
        string option = line.substr(0,current);
        string value = line.substr(current+1,line.size());
        //cout << option << " value: " <<  value << endl;
        if(option == "speedcam"){
            speedCam = stof(value);
        }else if(option == "musicvolume"){
            musicVolume = stof(value);
        }else if(option == "fxvolume"){
            fxVolume = stof(value);
        }
    }
   return true;
}

bool OptionManager::saveOption(){
    ofstream file("data/save/option.txt");
    if(file){
        file << "speedcam:" << getSpeedCam() << endl;
        file << "musicvolume:" << getMusicVolume() << endl;
        file << "fxvolume:" << getFxVolume() << endl;
        return true;
    }else{
        return false;
    }
}

sf::Vector2i OptionManager::getScreenSize(){
    return screenSize;
}


float OptionManager::getSpeedCam(){
    return speedCam;
}

void OptionManager::setSpeedCam(float value){
    speedCam = value;
}

float OptionManager::getMusicVolume(){
    return musicVolume;
}

void OptionManager::setMusicVolume(float value){
    musicVolume = value;
}

float OptionManager::getFxVolume(){
    return fxVolume;
}

void OptionManager::setFxVolume(float value){
    fxVolume = value;
}
