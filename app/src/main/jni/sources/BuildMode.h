#ifndef BUILDMODE_H
#define BUILDMODE_H

#include "Entity/Building/Building.h"

class Map;

class BuildMode{

public:
    BuildMode(Map* map);
    ~BuildMode();

    bool isReady(); //return true if active, m_okForThisTile and has m_tiledEntity

    void setBuilding(BuildingType *buildingType, QualityEnum quality);

    void setBuildMode(bool act);
    inline bool isActive() const{
        return m_active;
    }

    inline Building* getBuilding() const{
        return m_building;
    }

    inline void getDirection(Direction &from, Direction &to) const{
        from = m_from;
        to = m_to;
    }

    void changeDirectionClockwise();
    void changeDirectionAntiClockwise();

    void updatePos(sf::Vector2i &posTile);

    void keyPressedEventHandler(sf::Event::KeyEvent &keyEvent);
    void wheelEventHandler(sf::Event::MouseWheelEvent &mouseWheelEvent);

    void draw(sf::RenderWindow &window);

private:
    bool m_active;
    sf::Vector2i m_posTileMouse;

    bool m_okForThisTile;

    int m_indexTiledEntity;
    Building *m_building;

    Direction m_from;
    Direction m_to;

    Map* m_map;
};

#endif //BUILDMODE_H



