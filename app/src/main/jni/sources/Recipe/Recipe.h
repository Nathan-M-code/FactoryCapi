//
// Created by natha on 26/10/2020.
//

#ifndef RECIPEMANAGER_H
#define RECIPEMANAGER_H

#include "../utile.h"
#include "../Item/ItemStack.h"


class Recipe{
public:
    Recipe(std::vector<ItemType*> listIngredientType,
           std::vector<unsigned> listIngredientNumber,
           std::vector<std::vector<DataItem*>> listDataItemIngredient,
           std::vector<ItemType*> listOutputType,
           std::vector<unsigned> listOutputNumber,
           std::vector<std::vector<DataItem*>> listDataItemOutput,
           float timeSec,
           bool producesElectricity,
           unsigned amountElectricityProduced);
    ~Recipe();

    inline unsigned getNbRes() const{
        return m_listResult.size();
    }

    inline unsigned getNbIngre() const{
        return m_listIngredient.size();
    }

    unsigned getNbIngreForType(ItemType *it);

    bool hasIngreType(ItemType* it) const;
    bool hasIngreItemTypeWithData(ItemTypeWithData* itemTypeWithData) const;

    std::vector<ItemStack*> getListResult() const;
    std::vector<ItemStack*> getListIngre() const;
    std::vector<ItemType*> getListIngreType() const;

    inline bool producesElectricity() const{
        return m_producesElectricity;
    }

    inline unsigned getAmountElectricity() const{
        return m_amountElectricityProduced;
    }

    float getProductionTime();

    void setPosUI(const sf::Vector2i &posScreen);

    void drawScreenPosUI(sf::RenderWindow &window);

private:
    float m_timeProduction;
    std::vector<ItemStack*> m_listIngredient;
    std::vector<ItemStack*> m_listResult;

    bool m_producesElectricity;
    unsigned m_amountElectricityProduced;

    sf::RectangleShape m_bgInfo;
    sf::Sprite m_spriteArrow;
    sf::Text m_textInfoIngre;
    sf::Text m_textInfoRes;
    sf::Text m_textTime;
};

std::ostream& operator<<(std::ostream& out, Recipe &recipe);


#endif //RECIPEMANAGER_H
