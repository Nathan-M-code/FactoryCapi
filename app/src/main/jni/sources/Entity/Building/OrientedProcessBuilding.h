#ifndef ORIENTEDPROCESSBUILDING_H
#define ORIENTEDPROCESSBUILDING_H

#include "ProcessBuilding.h"
#include "OrientedBuilding.h"

class Map;

class OrientedProcessBuilding: public OrientedBuilding, public ProcessBuilding{

public:
    OrientedProcessBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, Direction from, Direction to, bool hasInput, bool hasOutput);
    virtual ~OrientedProcessBuilding();


    void update() override;

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen) override;

    void draw(sf::RenderWindow &window) override;

protected:

};

#endif //ORIENTEDPROCESSBUILDING_H
