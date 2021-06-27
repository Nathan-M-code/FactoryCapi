#include "PhysicalItem.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

unsigned PhysicalItem::s_size = 32;

PhysicalItem::PhysicalItem(Map *map, ItemTypeWithData * itemTypeWithData, const sf::Vector2f &cooMap):
Entity(map)
{
    m_itemTypeWithData = itemTypeWithData;

    m_sprite = Sprite(*itemTypeWithData->getItemType()->getTexture());

    m_sprite.setScale(s_size/m_sprite.getGlobalBounds().width,s_size/m_sprite.getGlobalBounds().height);

    m_pos = cooMap;
    m_sprite.setPosition(m_pos);
}

void PhysicalItem::setPos(float x, float y){
    m_pos = {x,y};
    m_sprite.setPosition(m_pos);
}

void PhysicalItem::setPosX(float x){
    m_pos.x = x;
    m_sprite.setPosition(m_pos);
}
void PhysicalItem::setPosY(float y){
    m_pos.y = y;
    m_sprite.setPosition(m_pos);
}

void PhysicalItem::setPos(sf::Vector2f pos){
    m_pos = pos;
    m_sprite.setPosition(m_pos);
}

void PhysicalItem::move(float offsetX, float offsetY){
    m_pos += {offsetX, offsetY};
    m_sprite.setPosition(m_pos);
}

void PhysicalItem::move(sf::Vector2f offset){
    m_pos += offset;
    //cout << "new pos pi: " << m_pos.x << " " << m_pos.y << endl;
    m_sprite.setPosition(m_pos);
}

PhysicalItem::~PhysicalItem(){

}

void PhysicalItem::update(){

}
