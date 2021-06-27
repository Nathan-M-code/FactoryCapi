#include "Robot.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

Robot::Robot(Map *map, sf::Clock* clock, const sf::Vector2f &originCooMap, WorkInProgress *wip):
Entity(map)
{
    m_clock = clock;

    m_lastMove = false;
    m_originCooMap = originCooMap;
    m_pos = originCooMap;
    m_speed = 10;
    m_isMoving = false;
    constructWIP(wip);

    m_previousTime = m_clock->getElapsedTime();
    m_msEachSheet = sf::milliseconds(1000.0/2);
    m_up = false;
    m_elapsedTime = milliseconds(0);
    //cout << "m_msEachSheet: " << m_msEachSheet.asMilliseconds() << endl;

    m_indexBoucing = 0;
    m_indexSpriteSheet = 0;

    m_texture = DataLoader::getTextureRobot();
    m_sizeSprite = {100,110};

    m_sizeOneSheet = {static_cast<float>(m_texture->getSize().x/16/1), static_cast<float>(m_texture->getSize().y/2)};
    //cout << m_sizeOneSheet.x << " " << m_sizeOneSheet.y << endl;
    m_sprite = Sprite(*m_texture,IntRect(m_indexSpriteSheetX*m_sizeOneSheet.x*1+m_indexSpriteSheet%1*m_sizeOneSheet.x,    m_indexSpriteSheet%2*m_sizeOneSheet.y,      m_sizeOneSheet.x,m_sizeOneSheet.y));
    m_sprite.setScale(m_sizeSprite.x/m_sprite.getGlobalBounds().width,m_sizeSprite.y/m_sprite.getGlobalBounds().height);

    m_sprite.setPosition(m_pos);
}
Robot::~Robot(){}

void Robot::moveTo(const sf::Vector2f &posObj){
    m_posObj = posObj;
    m_isMoving = true;
    //update m_indexSpriteSheetX
    float dir = atan2(m_posObj.y-m_pos.y, m_posObj.x-m_pos.x);
    //cout << "dir: " << dir << endl;

    m_indexSpriteSheetX = dir * 8/PI +4;
    if(m_indexSpriteSheetX < 0){
        m_indexSpriteSheetX = 16+m_indexSpriteSheetX;
    }

    m_sprite.setTextureRect(IntRect(m_indexSpriteSheetX*m_sizeOneSheet.x*1+m_indexSpriteSheet%1*m_sizeOneSheet.x,    m_indexSpriteSheet%2*m_sizeOneSheet.y,      m_sizeOneSheet.x,m_sizeOneSheet.y));
}

void Robot::constructWIP(WorkInProgress* wip){
    m_wip = wip;
    moveTo(wip->getMidPos());
    m_timeLeftBeforeMove = sf::seconds(rand_a_b(5,11));
}

void Robot::finishWork(){
    m_wip = 0;
    m_lastMove = true;
    moveTo(m_originCooMap);
}

void Robot::update(){
    m_actualTime = m_clock->getElapsedTime();
    m_elapsedTime += m_actualTime-m_previousTime;
    m_previousTime = m_actualTime;

    //update pos
    if(m_isMoving){
        float dir = atan2(m_posObj.y-m_pos.y, m_posObj.x-m_pos.x);
        sf::Vector2f move = {cos(dir)*m_speed,sin(dir)*m_speed};
        m_pos += move;
        if(m_pos.x >= m_posObj.x-m_speed && m_pos.x <= m_posObj.x+m_speed && m_pos.y >= m_posObj.y-m_speed && m_pos.y <= m_posObj.y+m_speed){
            m_isMoving = false;
            if(m_lastMove){
                //destroy robot
                m_map->destroyRobot(this);
                return;
            }
        }
        m_sprite.setPosition(m_pos);
    }else{
        m_timeLeftBeforeMove -= m_elapsedTime;
        if(m_timeLeftBeforeMove <= seconds(0)){
            m_timeLeftBeforeMove = sf::seconds(rand_a_b(5,11));
            sf::Vector2f midPos = m_wip->getMidPos();
            sf::Vector2f sizeWIP = m_wip->getSize();
            moveTo({midPos.x+rand_a_b(-sizeWIP.x/2.0, sizeWIP.x/2.0 +1),midPos.y+rand_a_b(-sizeWIP.x/2.0, sizeWIP.x/2.0 +1)});
        }
    }


    //update sprite
    if(m_up){
        m_pos.y += 1;
    }else{
        m_pos.y -= 1;
    }
    //m_pos.y += sin(x)*10;
    //update sheet
    while(m_elapsedTime >= m_msEachSheet){
        m_indexSpriteSheet+=1;
        m_up = true;
        m_elapsedTime -= m_msEachSheet;

        if(m_indexSpriteSheet == 2){
            m_indexSpriteSheet = 0;
            m_indexBoucing = 0;
            m_up = false;
        }
    }
    m_sprite.setTextureRect(IntRect(m_indexSpriteSheetX*m_sizeOneSheet.x*1+m_indexSpriteSheet%1*m_sizeOneSheet.x,    m_indexSpriteSheet%2*m_sizeOneSheet.y,      m_sizeOneSheet.x,m_sizeOneSheet.y));
    m_sprite.setPosition(m_pos);
}
