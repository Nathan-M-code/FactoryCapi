#include "Building.h"
#include "Chest1.h"

#include "../../Map/Map.h"

using namespace rapidjson;
using namespace std;
using namespace sf;

Building::Building(sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType):
Entity()
{
    m_buildingType = buildingType;


    m_clock = clock;
    m_previousTime = m_clock->getElapsedTime();
    m_msEachSheet = m_buildingType->getMSEachTileSheet();
    m_elapsedTime = milliseconds(0);
    //cout << "m_msEachSheet: " << m_msEachSheet.asMilliseconds() << endl;

    m_indexSpriteSheet = 0;

    m_texture = buildingType->getTexture();
    m_sizeOneSheet = {m_texture->getSize().x/m_buildingType->getTileSizeSheet().x, m_texture->getSize().y/m_buildingType->getTileSizeSheet().y};
    //cout << m_sizeOneSheet.x << " " << m_sizeOneSheet.y << endl;
    m_sprite = Sprite(*m_texture,IntRect(m_indexSpriteSheet%m_buildingType->getTileSizeSheet().x*m_sizeOneSheet.x,m_indexSpriteSheet/m_buildingType->getTileSizeSheet().y*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));


    m_sprite.setScale(m_buildingType->getSizeSprite().x/m_sprite.getGlobalBounds().width,m_buildingType->getSizeSprite().y/m_sprite.getGlobalBounds().height);

    m_pos = sf::Vector2f();
    m_pos.x = posTile.x*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().width-buildingType->getTileSizeBox().x*Map::s_sizeTileBuildings)/2.0);
    if(m_sprite.getGlobalBounds().height > buildingType->getTileSizeBox().y*Map::s_sizeTileBuildings){
        m_pos.y = posTile.y*Map::s_sizeTileBuildings-(m_sprite.getGlobalBounds().height-buildingType->getTileSizeBox().y*Map::s_sizeTileBuildings);
    }else{
        m_pos.y = posTile.y*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().height-buildingType->getTileSizeBox().y*Map::s_sizeTileBuildings)/2.0);
    }
    m_sprite.setPosition(m_pos);
}

Building::~Building(){

}

void Building::update(){
    m_actualTime = m_clock->getElapsedTime();
    m_elapsedTime += m_actualTime-m_previousTime;
    m_previousTime = m_actualTime;

    if(m_buildingType->getNbSheet() > 1){
        while(m_elapsedTime >= m_msEachSheet){
            m_indexSpriteSheet+=1;
            m_elapsedTime -= m_msEachSheet;

            if(m_indexSpriteSheet == m_buildingType->getNbSheet()){
                m_indexSpriteSheet = 0;
            }
        }
        //cout << " index: " << m_indexSpriteSheet << " x: " << m_indexSpriteSheet%m_buildingType->getTileSizeSheet().x << " y: " << m_indexSpriteSheet/m_buildingType->getTileSizeSheet().y << endl;
        m_sprite.setTextureRect(IntRect(m_indexSpriteSheet%m_buildingType->getTileSizeSheet().x*m_sizeOneSheet.x,m_indexSpriteSheet/m_buildingType->getTileSizeSheet().x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
    }
}
