#include "Belt1.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

unsigned Belt1::s_indexSpriteSheet = 0;
sf::Time Belt1::s_msEachSheet = sf::milliseconds(0);
sf::Time Belt1::s_lag = sf::milliseconds(0);

unsigned Belt1::s_nb = 0;


Belt1::Belt1(Map *map, sf::Clock *clock, sf::Vector2i posTile, Direction from, Direction to):
TransportSystem(map, clock, posTile, DataLoader::getBuildingType(0), from, to),
Building(map,clock,posTile,DataLoader::getBuildingType(0))
{
    s_nb++;

    s_msEachSheet = m_buildingType->getMSEachTileSheet(m_numTexture);
    //cout << "s_msEachSheet: " << s_msEachSheet.asMilliseconds() << endl;

}
Belt1::~Belt1()
{

}

void Belt1::setDirection(Direction from, Direction to){
    TransportSystem::setDirection(from,to);
    m_sprite.setTextureRect(IntRect(s_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,(m_indexSpriteOffsetY*m_sizeOneSheet.y)+s_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
}

void Belt1::updateIndexSprite(const sf::Time& elapsedTime){
    if(s_nb > 0){
        s_lag += elapsedTime;
        while(s_lag >= s_msEachSheet){
            s_indexSpriteSheet++;
            s_lag -= s_msEachSheet;
        }
        s_indexSpriteSheet = s_indexSpriteSheet%DataLoader::getBuildingType(0)->getTileSizeSheet()[0].x;
        //cout << s_indexSpriteSheet << endl;
    }
}

void Belt1::update(){
    TransportSystem::update();
    //cout << m_indexSpriteOffsetY << " " << s_indexSpriteSheet%m_buildingType->getTileSizeSheet().x*m_sizeOneSheet.x << " " << (m_indexSpriteOffsetY*m_sizeOneSheet.y)+s_indexSpriteSheet/m_buildingType->getTileSizeSheet().x*m_sizeOneSheet.y << " " << m_sizeOneSheet.x << " " << m_sizeOneSheet.y << endl;
    m_sprite.setTextureRect(IntRect(s_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,(m_indexSpriteOffsetY*m_sizeOneSheet.y)+s_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
}
