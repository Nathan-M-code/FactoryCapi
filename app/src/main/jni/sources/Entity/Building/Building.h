#ifndef BUILDING_H
#define BUILDING_H

#include "BuildingType.h"
#include "../Entity.h"

class Map;
class ElectricalBuilding;

class Building: public Entity {

public:
    Building();
    Building(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType);
    Building(Map *map, sf::Clock* clock, Building *buildingToCopy);

    virtual ~Building();

    inline BuildingType* getBuildingType() const{
        return m_buildingType;
    }

    inline QualityEnum getQuality() const{
        return m_quality;
    }

    inline sf::Vector2i getPosTile() const{
        return m_posTile;
    }

    inline unsigned getNumTexture() const{
        return m_numTexture;
    }

    sf::Vector2f getMidPos() const;

    virtual void setPos(sf::Vector2i posTile);
    void setQuality(QualityEnum quality);

    void startSpriteProcessing();
    void stopSpriteProcessing();

    void setNumTexture(unsigned num);

    virtual void setElectricalBuilding(ElectricalBuilding *electricalBuilding);

    virtual bool canBeDeconstructed() const;

    virtual void update() override;

    virtual void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen);
    virtual void mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick);

    virtual void draw(sf::RenderWindow &window) override;
    virtual void drawScreenPosUI(sf::RenderWindow &window);
    virtual void drawWorldPosUI(sf::RenderWindow &window);

protected:
    void addStringTextinfo(std::string str);

protected:
    sf::Vector2i m_saveCooScreen;

    sf::Clock* m_clock;

    sf::Time m_previousTime;
    sf::Time m_actualTime;
    sf::Time m_elapsedTime;
    sf::Time m_msEachSheet; //change m_indexSpriteSheet every m_msEachSheet

    unsigned m_numTexture;

    bool m_spriteProcessing;
    sf::Vector2f m_sizeOneSheet;
    int m_indexSpriteSheet;
    bool m_isReversing;

    sf::Vector2i m_posTile;

    BuildingType *m_buildingType;

    QualityEnum m_quality;

    sf::Text m_textInfo;
    sf::RectangleShape m_bgInfo;

    ElectricalBuilding *m_electricalBuilding;
    sf::Sprite m_arrowToElecBuilding;
};

#endif //BUILDING_H
