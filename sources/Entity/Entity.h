#ifndef ENTITY_H
#define ENTITY_H

#include "../utile.h"
#include "../TextureLoader.h"

class Map;

class Entity{

public:
    Entity();
    virtual ~Entity();

    virtual void update() = 0;

    void draw(sf::RenderWindow &window);
protected:
    sf::Vector2f m_pos;

    sf::Sprite m_sprite;
    sf::Texture* m_texture;

};

#endif //ENTITY_H

