#ifndef OPTIONMANAGER_H_INCLUDED
#define OPTIONMANAGER_H_INCLUDED

#include <fstream>
#include <string>
#include <iostream>
#include "utile.h"

class OptionManager{
public:
    static bool loadOption(AAssetManager* assetManager);
    static bool saveOption();

    static float getSpeedCam();
    static void setSpeedCam(float value);

    static float getMusicVolume();
    static void setMusicVolume(float value);

    static float getFxVolume();
    static void setFxVolume(float value);

    static sf::Vector2i getScreenSize();

private:
    static float speedCam;
    static float musicVolume;
    static float fxVolume;

    static sf::Vector2i screenSize;
};

#endif // OPTIONMANAGER_H_INCLUDED
