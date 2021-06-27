#ifndef WIP_H
#define WIP_H

#include "Building.h"

class Robot;

class WorkInProgress: public Building{

public:
    WorkInProgress(Map *map, sf::Clock *clock, WIPStatus wipStatus, Building *buildingToCopy);

    void allInit(sf::Clock *clock, WIPStatus wipStatus, sf::Vector2i posTile, BuildingType* buildingType);
    virtual ~WorkInProgress();

    inline QualityEnum getQuality() const{
        return m_quality;
    }

    inline WIPStatus getWipStatus() const{
        return m_wipStatus;
    }

    inline void getDirection(Direction &from, Direction &to){
        from = m_from;
        to = m_to;
    }

    void setPos(sf::Vector2i posTile);

    inline void setRobot(Robot *robot){
        m_robot = robot;
    }

    sf::Vector2f getSize() const;

    virtual void update();

    void draw(sf::RenderWindow &window);

private:
    WIPStatus m_wipStatus;

    sf::Vector2i m_posTile;

    QualityEnum m_quality;
    Direction m_from;
    Direction m_to;

    sf::Text m_text;
    sf::Vector2f m_midPos;

    sf::Time m_finishTime;
    sf::Time m_timeLeft;

    sf::Sprite m_spriteConstructionUI;
    std::vector<sf::Sprite> m_listSpriteScaffolding;

    Robot* m_robot;
};

#endif //WIP_H
