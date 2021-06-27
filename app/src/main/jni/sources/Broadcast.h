#ifndef BROADCAST_H_INCLUDED
#define BROADCAST_H_INCLUDED

#include "DataLoader.h"

class Broadcast{
public:
    static void init();

    static void announce(std::string str, unsigned nbSec, sf::Vector2f posScreen);
    static void error(std::string str, unsigned nbSec, sf::Vector2f posScreen);

    static void update();

    static void draw(sf::RenderWindow &window);
private:
    static sf::Clock s_clock;
    static sf::Text s_text;

    static sf::Time s_startTime;
    static sf::Time s_timeDraw;

    static bool s_drawn;

    static void startAnim(std::string str, unsigned nbSec, sf::Vector2f posScreen);
};

#endif // TEXTURELOADER_H_INCLUDED
