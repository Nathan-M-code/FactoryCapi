#ifndef ORIENTEDBUILDING_H
#define ORIENTEDBUILDING_H

#include "Building.h"

class Map;

class OrientedBuilding: virtual public Building{

public:
    OrientedBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, Direction from, Direction to);
    virtual ~OrientedBuilding();

    virtual void setPos(sf::Vector2i posTile) override;

    inline void getDirection(Direction &from, Direction &to){
        from = m_from;
        to = m_to;
    }

    virtual void setDirection(Direction from, Direction to);

    virtual void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen) override;

    virtual void draw(sf::RenderWindow &window) override;

protected:
    Direction m_from;
    Direction m_to;
    unsigned m_indexSpriteOffsetY;
};

#endif //ORIENTEDBUILDING_H


