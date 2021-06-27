#ifndef INSERTER1_H
#define INSERTER1_H

#include "OrientedBuilding.h"

class Map;

class Inserter1: public OrientedBuilding{

public:
    Inserter1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality, Direction from=NORD, Direction to=SOUTH);
    virtual ~Inserter1();

    void setDirection(Direction from, Direction to) override;

    void update() override;

    void draw(sf::RenderWindow &window) override;

private:
    float m_itemPerSec;
};

#endif //INSERTER1_H

