#ifndef PROCESSBUILDING_H
#define PROCESSBUILDING_H

#include "Building.h"
#include "../../Item/ItemStack.h"

class Map;

class ProcessBuilding: virtual public Building{

public:
    ProcessBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, bool hasInput, bool hasOutput);
    virtual ~ProcessBuilding();

    inline bool hasInput() const{
        return m_hasInput;
    }
    inline bool hasOutput() const{
        return m_hasOutput;
    }

    inline std::vector<ProcessBuilding*> getAllInputBuilding() const{
        return m_listInputProcessBuilding;
    }

    void addOutputProcessBuilding(ProcessBuilding* outputProcessBuilding);
    void removeOutputProcessBuilding(ProcessBuilding* outputProcessBuilding);
    void setOutputProcessBuilding(unsigned index);
    void deselectOutputProcessBuilding();

    void addInputProcessBuilding(ProcessBuilding* inputProcessBuilding);
    void removeInputProcessBuilding(ProcessBuilding* inputProcessBuilding);

    virtual bool canAcceptItem(ItemStack *is) = 0;
    virtual void transmitItem(ItemStack *is) = 0;
    virtual void receiveItem(ItemStack *is) = 0;

    void update() override;

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen) override;
    void mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick) override;

    void draw(sf::RenderWindow &window) override;
    void drawWorldPosUI(sf::RenderWindow &window) override;
    void drawScreenPosUI(sf::RenderWindow &window) override;


protected:
    bool m_hasInput;
    bool m_hasOutput;

    ItemStack * m_outputReady;

    std::vector<ProcessBuilding*> m_processBuildingOutputAvailable;
    ProcessBuilding* m_outputProcessBuilding;
    std::vector<ProcessBuilding*> m_listInputProcessBuilding;

    sf::Sprite m_spriteArrowOutput;
    sf::Sprite m_spriteArrowInput;
};

#endif //PROCESSBUILDING_H
