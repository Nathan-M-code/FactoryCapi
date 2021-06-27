#ifndef FACTORYWORLDMAP_H
#define FACTORYWORLDMAP_H

#include "../DataLoader.h"

class FactoryWorldMap {
public:
    FactoryWorldMap();
    ~FactoryWorldMap();

    void loadInfo(unsigned lvl,
                  sf::String owner,
                  const sf::Vector2f &posSprite,
                  const sf::Vector2f &sizeSprite,
                  const sf::Vector2i &posAbsolute);
    void updateInfo();

    void draw(sf::RenderWindow &window);
    void drawUIScreenPos(sf::RenderWindow &window);

private:
    sf::Vector2i m_posAbsolute;
    unsigned m_lvl;
    sf::String m_owner;

    sf::Sprite m_sprite;

    sf::RectangleShape m_rsBackGroundInfo;
    sf::Text m_textInfo;

};


#endif //FACTORYWORLDMAP_H
