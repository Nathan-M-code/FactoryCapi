#ifndef ASSEMBLINGMACHINE1_H
#define ASSEMBLINGMACHINE1_H

#include "ProcessBuilding.h"
#include "../../Recipe/RecipeProcessing.h"

class Map;

class AssemblingMachine1: public ProcessBuilding, public RecipeProcessing{

public:
    AssemblingMachine1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality);
    virtual ~AssemblingMachine1();

    bool canAcceptItem(ItemStack *is) override;
    void transmitItem(ItemStack *is) override;
    void receiveItem(ItemStack *is) override;

    virtual void update() override;

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen) override;
    void mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick) override;

    void drawScreenPosUI(sf::RenderWindow &window) override;

    void startProducing() override;
    void stopProducing() override;

private:

};

#endif //ASSEMBLINGMACHINE1_H
