#ifndef ITEM_H
#define ITEM_H

#include "ItemType.h"

class Item{

public:
    Item(ItemType *itemType);
    ~Item();
private:
    ItemType * m_itemType;

};

#endif //ITEM_H
