#ifndef CHEST1_H
#define CHEST1_H

#include "Building.h"

class Chest1: public Building{

public:
    Chest1(sf::Clock *clock, sf::Vector2i posTile);
    virtual ~Chest1();

    virtual void update();

private:

};

#endif //CHEST1_H

