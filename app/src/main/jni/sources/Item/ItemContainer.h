#ifndef ITEMCONTAINER_H
#define ITEMCONTAINER_H

#include "ItemType.h"
#include "DrawableItemStack.h"

class ItemContainer{

public:
    ItemContainer(unsigned nbRow, sf::Vector2f posScreen, unsigned maxItemStack=10);
    ~ItemContainer();

    void setMaxItemStack(unsigned maxItemStack);

    bool canAddItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem);
    bool canAddItem(ItemStack *is);

    void addItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem={});
    void addItem(ItemStack *is);

    void removeItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem);
    void removeItem(ItemStack *is);

    bool hasItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem);
    bool hasItem(ItemStack *is);

    ItemStack* getFirstItemStack() const;

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen);
    void mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick);

    void drawInfo(sf::RenderWindow &window);

protected:
    sf::Vector2i m_saveCooScreen;
    bool m_movingItem;
    DrawableItemStack *m_saveDoubleItemStack;
    DrawableItemStack *m_saveItemStackOnHand;
    unsigned m_saveIndexClick;

    sf::Sprite m_spriteDarkHand;

    std::vector<DrawableItemStack *> m_listItemStack;
    std::vector<sf::RectangleShape> m_listBgItem;

    unsigned m_maxItemStack;

    sf::Vector2f m_posScreen;

    unsigned m_nbRow;
    unsigned m_nbCol;

    unsigned m_paddding;

    sf::Vector2f m_sizeBoxItem;

    sf::RectangleShape m_rectBackground;

    void updatePosItem();

};

#endif //ITEMCONTAINER_H


