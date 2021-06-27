#ifndef TRANSPORTSYSTEM_H
#define TRANSPORTSYSTEM_H

#include "../PhysicalItem/PhysicalItem.h"
#include "OrientedProcessBuilding.h"

class Map;

class TransportSystem: public OrientedProcessBuilding{

public:
    TransportSystem(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, Direction from, Direction to);
    virtual ~TransportSystem();

    virtual void setPos(sf::Vector2i posTile);
    inline void getDirection(Direction &from, Direction &to){
        from = m_from;
        to = m_to;
    }
    inline float getPosBoundary() const{
        return m_posBoundary;
    }
    PhysicalItem *getLastPhysicalItem() const;

    void updatePosBoundary();
    virtual void setDirection(Direction from, Direction to);
    static unsigned getIndexSpriteByDirection(Direction from, Direction to);

    inline void setNextTS(TransportSystem *ts){
        //std::cout << "setNextTS" << std::endl;
        m_nextTS = ts;
        //std::cout << "nextTS pos: " << ts->getPosTile().x << " " << ts->getPosTile().y << std::endl;
    }


    void transmitPhysicalItemTo(TransportSystem* ts, PhysicalItem *pi, float remains);
    void addPhysicalItemStart(PhysicalItem *pi, float remains=0.0);

    virtual bool canAcceptItem(ItemStack *is);
    virtual void transmitItem(ItemStack *is);
    virtual void receiveItem(ItemStack *is);

    virtual void update();
    virtual void draw(sf::RenderWindow &window);
    virtual void drawItem(sf::RenderWindow &window);

    static unsigned s_density;

protected:
    float m_itemPerSec; //throughput
    float m_speed; //sec per Tile
    float m_pxlPerTick;

    TransportSystem *m_nextTS;
    float m_posBoundary; //can be a x or a y pos
    float m_posSemiBoundary; //can be a x or a y pos for ts with turn

    std::vector<PhysicalItem *> m_listPhysicalItem;
};

#endif //TRANSPORTSYSTEM_H



