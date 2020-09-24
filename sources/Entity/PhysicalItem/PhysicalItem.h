#ifndef PHYSICALITEM_H
#define PHYSICALITEM_H

#include "../Entity.h"
#include "../../Item/ItemType.h"

class PhysicalItem: public Entity{

public:
    PhysicalItem(ItemType * itemType);
    virtual ~PhysicalItem();

    virtual void update();
private:
    ItemType * m_itemType;
};

#endif //PHYSICALITEM_H
