//
// Created by natha on 27/10/2020.
//

#ifndef FACTORYCAPICLION_RECIPEMANAGER_H
#define FACTORYCAPICLION_RECIPEMANAGER_H

#include "../utile.h"
#include "Recipe.h"

class RecipeManager {
public:
    static void addRecipe(std::vector<ItemType*> listIngredientType,
                          std::vector<unsigned> listIngredientNumber,
                          std::vector<std::vector<DataItem*>> listDataItemIngredient,
                          std::vector<ItemType*> listOutputType,
                          std::vector<unsigned> listOutputNumber,
                          std::vector<std::vector<DataItem*>> listDataItemOutput,
                          float timeSec,
                          bool producesElectricity,
                          unsigned amountElectricityProduced);
    static Recipe* getRecipe(ItemTypeWithData *itemTypeWithData1, ItemTypeWithData *itemTypeWithData2 = nullptr, ItemTypeWithData *itemTypeWithData3 = nullptr);

    static unsigned getMaxOuput(unsigned nbIngre);

    static void addItemTypeWithDataForLate(ItemTypeWithData *itemTypeWithData);
    static Recipe* getRecipeLate();
    static void clear();

private:
    static std::vector<Recipe*> sm_listRecipe;

    static std::vector<ItemTypeWithData*> sm_saveItemTypeWithData;

    static std::vector<unsigned> sm_listMaxOutput;
};


#endif //FACTORYCAPICLION_RECIPEMANAGER_H
