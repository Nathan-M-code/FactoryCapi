#ifndef DRAWABLEITEMSTACK_H
#define DRAWABLEITEMSTACK_H

#include "ItemStack.h"
#include "../DataLoader.h"

class DrawableItemStack: public ItemStack{

public:
    DrawableItemStack(ItemTypeWithData* itemTypeWithData, sf::Vector2f size, sf::Vector2f pos, unsigned nbItem=1);
    DrawableItemStack(ItemStack *is, sf::Vector2f size, sf::Vector2f pos);
    DrawableItemStack(DrawableItemStack* drawablIS);
    ~DrawableItemStack();

    void init(sf::Vector2f size, sf::Vector2f pos);

    void setNbItem(unsigned nbItem) override;

    void setPos(sf::Vector2f pos);
    inline sf::Vector2f getPos() const{
        return m_sprite.getPosition();
    }
    inline sf::Vector2f getSize() const{
        return {m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height};
    }

    void setPosUI(const sf::Vector2i &posScreen);
    bool mouseIsOn(const sf::Vector2i &posScreen) const;

    void updateStringInfo();

    void draw(sf::RenderWindow &window) override;
    void drawScreenPosUI(sf::RenderWindow &window) override;

private:
    sf::Sprite m_sprite;
    sf::Text m_text;

    sf::RectangleShape m_bgInfo;
    sf::Text m_textInfo;
};

#endif //DRAWABLEITEMSTACK_H


