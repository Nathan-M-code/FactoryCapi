//
// Created by natha on 31/10/2020.
//

#ifndef RECIPEPROCESSING_H
#define RECIPEPROCESSING_H

#include "../utile.h"
#include "Recipe.h"
#include "../Item/DrawableItemStack.h"
#include "../Electricity/Power.h"


class RecipeProcessing {

public:
    RecipeProcessing(sf::Clock* clock, bool canProduceElectricity);
    virtual ~RecipeProcessing();

    void initLate(unsigned nbInputIngre, float divTime);

    virtual void update();

    virtual void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen);
    virtual void mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick);

    virtual void drawScreenPosUI(sf::RenderWindow &window);

protected:
    void clickFlush();
    bool mouseOnFlushButton(const sf::Vector2i &cooScreen);

    bool hasAnOutput();

    ItemStack *getFirstItemStackIngredient();
    ItemStack *getFirstItemStackResult();

    unsigned getNbIngredient();

    bool canAcceptItem(ItemStack *is);
    void addIngredient(ItemStack *is);
    void removeItemStackIngredient(ItemStack *itemStack);
    void removeItemStackOutput(ItemStack *itemStack);

    bool isAnIngredient(ItemStack *is);
    bool isAnOutput(ItemStack *is);

    virtual void startProducing();
    virtual void stopProducing();

    void setOutput();



protected:
    sf::Clock* m_clock;

    sf::Time m_msItemProduct;

    sf::Time m_previousTimeProd;
    sf::Time m_actualTimeProd;
    sf::Time m_elapsedTimeProd;

    bool m_canProduceElectricity;
    bool m_isProducingElectricity;
    Power m_powerProduced;

    float m_divTime;

    bool m_producing;

    unsigned m_nbInputIngre;
    unsigned m_nbOutputIngre;

    Recipe *m_targetRecipe;
    Recipe *m_targetRecipeSave;

    sf::Sprite m_flush;
    sf::RectangleShape m_indicFlush;
    bool m_flushing;

    std::vector<DrawableItemStack*> m_listItemIngredient;
    std::vector<DrawableItemStack*> m_listOutput;

    sf::RectangleShape m_bgInfo;
    std::vector<sf::RectangleShape> m_listBgInput;

    std::vector<sf::RectangleShape> m_listBgSprite;
    std::vector<sf::Sprite> m_listSpriteItemProduced;

    sf::RectangleShape m_bgOutput;
    sf::RectangleShape m_bgBarre;
    sf::RectangleShape m_barre;

    sf::Text m_text;

    sf::Vector2i m_savePosScreen;

    bool mouseIsOnBarre();
};


#endif //RECIPEPROCESSING_H
