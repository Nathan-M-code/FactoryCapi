#include "DrawableItemStack.h"

using namespace std;

DrawableItemStack::DrawableItemStack(ItemTypeWithData* itemTypeWithData, sf::Vector2f size, sf::Vector2f pos, unsigned nbItem):
ItemStack(itemTypeWithData,nbItem)
{
    init(size,pos);
}

DrawableItemStack::DrawableItemStack(ItemStack *is, sf::Vector2f size, sf::Vector2f pos):
ItemStack(is->getItemTypeWithData(),is->getNbItem())
{
    init(size,pos);
}

DrawableItemStack::DrawableItemStack(DrawableItemStack* drawablIS):
ItemStack(drawablIS->getItemTypeWithData(),drawablIS->getNbItem())
{
    init(drawablIS->getSize(),drawablIS->getPos());
}

DrawableItemStack::~DrawableItemStack(){
}

void DrawableItemStack::init(sf::Vector2f size, sf::Vector2f pos){
    m_sprite.setTexture(*m_itemTypeWithData->getItemType()->getTexture());
    m_sprite.setPosition(pos);
    m_sprite.setScale(size.x/m_sprite.getGlobalBounds().width,size.y/m_sprite.getGlobalBounds().height);

    initTextStyle(m_text, DataLoader::getFont(), 20);
    m_text.setString(to_string(m_nbItem));
    m_text.setPosition(m_sprite.getPosition().x+m_sprite.getGlobalBounds().width-m_text.getGlobalBounds().width, m_sprite.getPosition().y+m_sprite.getGlobalBounds().height-m_text.getGlobalBounds().height-5);

    initRectStyle(m_bgInfo);
    initTextStyle(m_textInfo, DataLoader::getFont(), 20);
    updateStringInfo();
    setPosUI({0,0});
}

void DrawableItemStack::setNbItem(unsigned nbItem){
    ItemStack::setNbItem(nbItem);
    m_text.setString(to_string(m_nbItem));
    m_text.setPosition(m_sprite.getPosition().x+m_sprite.getGlobalBounds().width-m_text.getGlobalBounds().width, m_sprite.getPosition().y+m_sprite.getGlobalBounds().height-m_text.getGlobalBounds().height-5);

    updateStringInfo();
}

void DrawableItemStack::setPos(sf::Vector2f pos){
    m_sprite.setPosition(pos);
    m_text.setPosition(m_sprite.getPosition().x+m_sprite.getGlobalBounds().width-m_text.getGlobalBounds().width, m_sprite.getPosition().y+m_sprite.getGlobalBounds().height-m_text.getGlobalBounds().height-5);
}

void DrawableItemStack::setPosUI(const sf::Vector2i &posScreen){
    m_bgInfo.setPosition(posScreen.x, posScreen.y);
    m_textInfo.setPosition(m_bgInfo.getPosition().x+m_bgInfo.getSize().x/2-m_textInfo.getGlobalBounds().width/2, m_bgInfo.getPosition().y+m_bgInfo.getSize().y/2-m_textInfo.getGlobalBounds().height/2);
}

bool DrawableItemStack::mouseIsOn(const sf::Vector2i &posScreen) const{
    return (posScreen.x >= m_sprite.getPosition().x
            && posScreen.x <= m_sprite.getPosition().x+m_sprite.getGlobalBounds().width
            && posScreen.y >= m_sprite.getPosition().y
            && posScreen.y <= m_sprite.getPosition().y+m_sprite.getGlobalBounds().height);
}

void DrawableItemStack::updateStringInfo(){
    m_textInfo.setString(getString());
    m_bgInfo.setSize({std::max(100.f,m_textInfo.getGlobalBounds().width+20),std::max(100.f,m_textInfo.getGlobalBounds().height+20)});
}


void DrawableItemStack::draw(sf::RenderWindow &window){
    window.draw(m_sprite);
    window.draw(m_text);
}

void DrawableItemStack::drawScreenPosUI(sf::RenderWindow &window){
    window.draw(m_bgInfo);
    window.draw(m_textInfo);
}
