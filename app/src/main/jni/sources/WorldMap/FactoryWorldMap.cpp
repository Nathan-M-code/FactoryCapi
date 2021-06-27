#include "FactoryWorldMap.h"

using namespace std;
using namespace sf;

FactoryWorldMap::FactoryWorldMap() {
    initRectStyle(m_rsBackGroundInfo);
    m_rsBackGroundInfo.setPosition(10,50);
    initTextStyle(m_textInfo, DataLoader::getFont(), 25);
    m_textInfo.setPosition(m_rsBackGroundInfo.getPosition().x+5, m_rsBackGroundInfo.getPosition().y+25);
}
FactoryWorldMap::~FactoryWorldMap() {}

void FactoryWorldMap::loadInfo(unsigned lvl,
                               sf::String owner,
                               const sf::Vector2f &posSprite,
                               const sf::Vector2f &sizeSprite,
                               const sf::Vector2i &posAbsolute){
    m_posAbsolute = posAbsolute;
    m_lvl = lvl;
    m_owner = owner;

    m_sprite.setTexture(*DataLoader::getTextureFactoryByLvl(m_lvl));
    m_sprite.setScale(sizeSprite.x/m_sprite.getLocalBounds().width, sizeSprite.y/m_sprite.getLocalBounds().height);
    m_sprite.setPosition(posSprite);

    updateInfo();
}
void FactoryWorldMap::updateInfo(){
    m_textInfo.setString("Pos: "+vector2iToString(m_posAbsolute)+"\n"
                             "Owner: "+m_owner+"\n"
                             "Level: "+to_string(m_lvl)+"\n"
    );
    m_rsBackGroundInfo.setSize({max(250.f,m_textInfo.getGlobalBounds().width+20),m_textInfo.getGlobalBounds().height+50});
}


void FactoryWorldMap::draw(sf::RenderWindow &window){
    window.draw(m_sprite);
}

void FactoryWorldMap::drawUIScreenPos(sf::RenderWindow &window){
    window.draw(m_rsBackGroundInfo);
    window.draw(m_textInfo);
}
