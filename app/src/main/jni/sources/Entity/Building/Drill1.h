#ifndef DRILL1_H
#define DRILL1_H

#include "ProcessBuilding.h"
#include "../../item/DrawableItemStack.h"

class Map;

class Drill1: public ProcessBuilding{

public:
    Drill1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality);
    virtual ~Drill1();

    void setListItemTypeToProduce(std::vector<ItemType*> listItemTypeToProduce);

    virtual bool canAcceptItem(ItemStack *is);
    virtual void transmitItem(ItemStack *is);
    virtual void receiveItem(ItemStack *is);

    virtual void update();

    virtual void drawScreenPosUI(sf::RenderWindow &window) override;

private:

    sf::Time m_msItemProduct;

    sf::Time m_previousTimeProd;
    sf::Time m_actualTimeProd;
    sf::Time m_elapsedTimeProd;

    bool m_canProcess;
    unsigned m_indexToProduce;
    std::vector<ItemType*> m_listItemTypeToProduce;

    sf::RectangleShape m_bg;
    sf::RectangleShape m_bgSprite;
    sf::RectangleShape m_bgOutput;
    sf::RectangleShape m_bgBarre;
    sf::RectangleShape m_barre;
    sf::Sprite m_spriteItemProduced;

    sf::Text m_text;
};

#endif //DRILL1_H



