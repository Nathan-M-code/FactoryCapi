#ifndef CHEST1_H
#define CHEST1_H

#include "ProcessBuilding.h"
#include "../../Item/ItemContainer.h"

class Map;

class Chest1: public ProcessBuilding, public ItemContainer{

public:
    Chest1(Map *map, sf::Clock *clock, sf::Vector2i posTile);
    virtual ~Chest1();

    bool canAcceptItem(ItemStack *is) override;
    void transmitItem(ItemStack *is) override;
    void receiveItem(ItemStack *is) override;

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen) override;
    void mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick) override;

    void update() override;

    void drawWorldPosUI(sf::RenderWindow &window) override; //posWorld
    void drawScreenPosUI(sf::RenderWindow &window) override; //posScreen

private:
};

#endif //CHEST1_H

