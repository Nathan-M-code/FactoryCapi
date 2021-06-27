#include "Entity.h"
#include "../Map/Map.h"

using namespace sf;
using namespace std;

Entity::Entity(){

}
Entity::Entity(Map *map){
    m_map = map;
}
Entity::~Entity(){

}

void Entity::setAlpha(unsigned value){
    m_sprite.setColor(sf::Color(255,255,255,value));
}


void Entity::draw(sf::RenderWindow &window){
    window.draw(m_sprite);
}
