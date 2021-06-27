#ifndef PHYSICALITEM_H
#define PHYSICALITEM_H

#include "../Entity.h"
#include "../../Item/ItemType.h"
#include "../../Item/ItemTypeWithData.h"

class Map;

class PhysicalItem: public Entity{

public:
    PhysicalItem(Map *map, ItemTypeWithData * itemTypeWithData, const sf::Vector2f &cooMap);
    virtual ~PhysicalItem();

    void setPos(float x, float y);
    void setPosX(float x);
    void setPosY(float y);
    void setPos(sf::Vector2f pos);
    void move(float offsetX, float offsetY);
    void move(sf::Vector2f offset);

    inline ItemTypeWithData* getItemTypeWithData() const{
        return m_itemTypeWithData;
    }

    virtual void update();

    static unsigned s_size;
private:
    ItemTypeWithData * m_itemTypeWithData;
};

#endif //PHYSICALITEM_H
