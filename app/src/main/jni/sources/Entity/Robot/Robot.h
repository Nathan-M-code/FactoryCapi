#ifndef ROBOT_H
#define ROBOT_H

#include "../Entity.h"
#include "RobotType.h"
#include "../Building/WorkInProgress.h"

class Map;

class Robot: public Entity{

public:
    Robot(Map *map, sf::Clock* clock, const sf::Vector2f &originCooMap, WorkInProgress *wip);
    virtual ~Robot();

    void constructWIP(WorkInProgress* wip);
    void finishWork();

    void moveTo(const sf::Vector2f &posObj);

    virtual void update();
private:
    sf::Texture *m_texture;
    sf::Vector2i m_sizeSprite;

    sf::Clock* m_clock;
    sf::Time m_previousTime;
    sf::Time m_actualTime;
    sf::Time m_elapsedTime;
    sf::Time m_msEachSheet; //change m_indexSpriteSheet every m_msEachSheet
    bool m_up;

    sf::Time m_timeLeftBeforeMove;

    unsigned m_indexSpriteSheet;
    unsigned m_indexBoucing;
    int m_indexSpriteSheetX;

    sf::Vector2f m_sizeOneSheet;

    bool m_isMoving;
    bool m_lastMove;
    sf::Vector2f m_originCooMap;
    sf::Vector2f m_posObj;
    float m_speed;

    WorkInProgress* m_wip;
};

#endif //PHYSICALITEM_H

