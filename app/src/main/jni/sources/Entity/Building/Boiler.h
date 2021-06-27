#ifndef BOILER_H
#define BOILER_H

#include "OrientedProcessBuilding.h"
#include "../../Recipe/RecipeProcessing.h"

class Map;

class Boiler : public OrientedProcessBuilding, public RecipeProcessing{

public:
    Boiler(Map *map, sf::Clock *clock, sf::Vector2i posTile, Direction from, Direction to, QualityEnum quality);
    virtual ~Boiler();

    void setDirection(Direction from, Direction to) override;

    void setElectricalBuilding(ElectricalBuilding *electricalBuilding) override;

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


#endif //BOILER_H
