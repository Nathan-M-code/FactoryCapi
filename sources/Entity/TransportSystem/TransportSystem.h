#ifndef TRANSPORTSYSTEM_H
#define TRANSPORTSYSTEM_H

#include "../Entity.h"
#include "TransportType.h"

class Map;

class TransportSystem: public Entity{

public:
    TransportSystem(sf::Clock* clock, sf::Vector2i posTile, TransportType *transportType, Direction fromDirection, Direction toDirection);
    virtual ~TransportSystem();

    inline void getDirection(Direction &from, Direction &to){
        from = m_fromDirection;
        to = m_toDirection;
    }

    inline void setDirection(Direction from, Direction to){
        m_fromDirection = from;
        m_toDirection = to;
        m_indexSpriteOffsetY = TransportSystem::getIndexSpriteByDirection(m_fromDirection, m_toDirection);
    }

    virtual void update();

    static void updateIndexSprite(const sf::Time& elapsedTime);

    static unsigned getIndexSpriteByDirection(Direction from, Direction to);

protected:
    sf::Clock* m_clock;

    TransportType *m_transportType;

    sf::Vector2f m_sizeOneSheet;

    Direction m_fromDirection;
    Direction m_toDirection;
    unsigned m_indexSpriteOffsetY;


};

#endif //TRANSPORTSYSTEM_H

