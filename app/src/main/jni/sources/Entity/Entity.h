#ifndef ENTITY_H
#define ENTITY_H

#include "../utile.h"
#include "../DataLoader.h"

class Map;

class Entity{

public:
    Entity();
    Entity(Map *map);
    virtual ~Entity();

    void setAlpha(unsigned value);

    inline sf::Vector2f getPos() const{
        return m_pos;
    }

    virtual void update() = 0;

    virtual void draw(sf::RenderWindow &window);
protected:
    Map* m_map;

    sf::Vector2f m_pos;

    sf::Sprite m_sprite;
};

#endif //ENTITY_H

