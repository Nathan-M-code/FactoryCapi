#ifndef BELT1_H
#define BELT1_H

#include "TransportSystem.h"

class Belt1: public TransportSystem{

public:
    Belt1(sf::Clock *clock, sf::Vector2i posTile, Direction from, Direction to);
    virtual ~Belt1();

    virtual void update();

    static void updateIndexSprite(const sf::Time& elapsedTime);

private:
    static unsigned s_indexSpriteSheet;
    static sf::Time s_msEachSheet;
    static sf::Time s_lag;
    static unsigned s_nb;
};

#endif //BELT1_H

