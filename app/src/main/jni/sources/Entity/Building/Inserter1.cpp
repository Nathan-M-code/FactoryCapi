#include "Inserter1.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;
using namespace rapidjson;

Inserter1::Inserter1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality, Direction from, Direction to):
OrientedBuilding(map, clock, posTile, DataLoader::getBuildingType(4), from, to),
Building(map,clock,posTile,DataLoader::getBuildingType(4))
{
    setQuality(quality);

    setDirection(from, to);

    auto &value = *m_buildingType->getValueJSON();
    m_itemPerSec = value["particularity"]["itemPerSec"].GetDouble();
}
Inserter1::~Inserter1()
{

}

void Inserter1::setDirection(Direction from, Direction to){
    m_from = from;
    m_to = to;

    if(m_from == NORD && m_to == SOUTH){
        setNumTexture(0);
    }else if(m_from == SOUTH && m_to == NORD){
        setNumTexture(1);
    }else if(m_from == EAST && m_to == WEST){
        setNumTexture(2);
    }else if(m_from == WEST && m_to == EAST){
        setNumTexture(3);
    }

    m_sizeOneSheet = {(float)m_sprite.getTexture()->getSize().x/m_buildingType->getTileSizeSheet()[m_numTexture].x, (float)m_sprite.getTexture()->getSize().y/m_buildingType->getTileSizeSheet()[m_numTexture].y};
    m_sprite.setScale(1,1);
    m_sprite.setTextureRect(IntRect(m_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,m_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
    m_sprite.setScale(m_buildingType->getSizeSprite()[m_numTexture].x/m_sprite.getGlobalBounds().width,m_buildingType->getSizeSprite()[m_numTexture].y/m_sprite.getGlobalBounds().height);
}

void Inserter1::update(){
    Building::update();
}

void Inserter1::draw(sf::RenderWindow &window){
    Building::draw(window);
}


