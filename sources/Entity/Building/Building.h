#ifndef BUILDING_H
#define BUILDING_H

#include "../Entity.h"

class Map;

class Building: public Entity{

public:
    Building(sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType);
    virtual ~Building();

    inline BuildingType* getBuildingType() const{
        return m_buildingType;
    }

    virtual void update();

protected:
    sf::Clock* m_clock;
    sf::Time m_previousTime;
    sf::Time m_actualTime;
    sf::Time m_elapsedTime;
    sf::Time m_msEachSheet; //change m_indexSpriteSheet every m_msEachSheet

    BuildingType *m_buildingType;

    int m_indexSpriteSheet;
    sf::Vector2f m_sizeOneSheet;
};

#endif //BUILDING_H
