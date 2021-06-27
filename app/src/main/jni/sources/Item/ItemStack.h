#ifndef ITEMSTACK_H
#define ITEMSTACK_H

#include "DataItem/QualityDataItem.h"
#include "ItemTypeWithData.h"

class ItemStack{

public:
    ItemStack(ItemTypeWithData *itemTypeWithData, unsigned nbItem=0);
    ItemStack(ItemStack *is);
    ~ItemStack();

    inline ItemTypeWithData* getItemTypeWithData() const{
        return m_itemTypeWithData;
    }
    inline unsigned getNbItem() const{
        return m_nbItem;
    }
    std::string getString() const;
    bool isStackableWith(ItemStack *is) const;

    virtual void setNbItem(unsigned nbItem);

    virtual void drawScreenPosUI(sf::RenderWindow &window);
    virtual void draw(sf::RenderWindow &window);

protected:
    ItemTypeWithData* m_itemTypeWithData;
    unsigned m_nbItem;
};

#endif //ITEMSTACK_H

