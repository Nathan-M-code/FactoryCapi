#include "Entity.h"
#include "../Map/Map.h"

using namespace sf;
using namespace std;

Entity::Entity(){

}
Entity::~Entity(){

}


void Entity::draw(sf::RenderWindow &window){
    window.draw(m_sprite);
}
