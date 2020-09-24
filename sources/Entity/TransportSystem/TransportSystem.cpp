#include "TransportSystem.h"
#include "../../Map/Map.h"

using namespace sf;
using namespace std;

TransportSystem::TransportSystem(sf::Clock* clock, sf::Vector2i posTile, TransportType *transportType, Direction fromDirection, Direction toDirection):
Entity()
{
    m_fromDirection = fromDirection;
    m_toDirection = toDirection;

    m_transportType = transportType;

    m_clock = clock;


    m_texture = m_transportType->getTexture();
    m_sizeOneSheet = {m_texture->getSize().x/m_transportType->getTileSizeSheet().x, m_texture->getSize().y/20.0/m_transportType->getTileSizeSheet().y};
    //cout << m_sizeOneSheet.x << " " << m_sizeOneSheet.y << endl;
    m_indexSpriteOffsetY = TransportSystem::getIndexSpriteByDirection(m_fromDirection, m_toDirection);

    m_sprite = Sprite(*m_texture,IntRect(0%m_transportType->getTileSizeSheet().x*m_sizeOneSheet.x,(m_indexSpriteOffsetY*m_sizeOneSheet.y)+0/m_transportType->getTileSizeSheet().x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
    m_sprite.setScale(m_transportType->getSizeOneSheet().x/m_sprite.getGlobalBounds().width,m_transportType->getSizeOneSheet().y/m_sprite.getGlobalBounds().height);

    m_pos = sf::Vector2f();

    m_pos.x = posTile.x*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().width-Map::s_sizeTileBuildings)/2.0);
    m_pos.y = posTile.y*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().height-Map::s_sizeTileBuildings)/2.0);

    m_sprite.setPosition(m_pos);
}
TransportSystem::~TransportSystem(){

}


void TransportSystem::update(){

}

void TransportSystem::updateIndexSprite(const sf::Time& elapsedTime){
    Belt1::updateIndexSprite(elapsedTime);
}

unsigned TransportSystem::getIndexSpriteByDirection(Direction from, Direction to){
    if(from == WEST && to == EAST){
        return 0;
    }else if(from == EAST && to == WEST){
        return 1;
    }else if(from == SOUTH && to == NORD){
        return 2;
    }else if(from == NORD && to == SOUTH){
        return 3;
    }else if(from == EAST && to == NORD){
        return 4;
    }else if(from == NORD && to == EAST){
        return 5;
    }else if(from == WEST && to == NORD){
        return 6;
    }else if(from == NORD && to == WEST){
        return 7;
    }else if(from == SOUTH && to == EAST){
        return 8;
    }else if(from == EAST && to == SOUTH){
        return 9;
    }else if(from == SOUTH && to == WEST){
        return 10;
    }else if(from == WEST && to == SOUTH){
        return 11;
    }
}
