//
// Created by nathan on 27/10/2020.
//

#include "RecipeManager.h"

using namespace std;

std::vector<Recipe*> RecipeManager::sm_listRecipe = {};
std::vector<ItemTypeWithData*> RecipeManager::sm_saveItemTypeWithData = {};
std::vector<unsigned> RecipeManager::sm_listMaxOutput = {0,0,0,0,0};

void RecipeManager::addRecipe(std::vector<ItemType*> listIngredientType,
                              std::vector<unsigned> listIngredientNumber,
                              std::vector<std::vector<DataItem*>> listDataItemIngredient,
                              std::vector<ItemType*> listOutputType,
                              std::vector<unsigned> listOutputNumber,
                              std::vector<std::vector<DataItem*>> listDataItemOutput,
                              float timeSec,
                              bool producesElectricity,
                              unsigned amountElectricityProduced){
    //cout << "RecipeManager::addRecipe, ingredient size: " << listIngredientType.size() << endl;
    if(listIngredientType.size() == 0){
        cout << "ERROR RecipeManager::addRecipe, listIngredientType.size() == 0" << endl;
        return;
    }

    sm_listRecipe.push_back(new Recipe(listIngredientType,
                                       listIngredientNumber,
                                       listDataItemIngredient,
                                       listOutputType,
                                       listOutputNumber,
                                       listDataItemOutput,
                                       timeSec,
                                       producesElectricity,
                                       amountElectricityProduced));

    /*cout << "listIngredientType.size: " << listIngredientType.size() << endl;
    cout << "save nb: " << sm_listMaxOutput[listIngredientType.size()-1] << endl;
    cout << "producesElectricity: " << boolToString(producesElectricity) << endl;
    cout << "output size: " << listOutputType.size()+(producesElectricity?1:0) << endl;*/
    if(listOutputType.size()+(producesElectricity?1:0) > sm_listMaxOutput[listIngredientType.size()-1]){
        sm_listMaxOutput[listIngredientType.size()-1] = listOutputType.size()+(producesElectricity?1:0);
    }
}
Recipe* RecipeManager::getRecipe(ItemTypeWithData *itemTypeWithData1, ItemTypeWithData *itemTypeWithData2, ItemTypeWithData *itemTypeWithData3){
    if(itemTypeWithData3){
        for(unsigned i = 0;i<sm_listRecipe.size();i++){
            if(sm_listRecipe[i]->getNbIngre() == 3){
                if(sm_listRecipe[i]->hasIngreItemTypeWithData(itemTypeWithData1) && sm_listRecipe[i]->hasIngreItemTypeWithData(itemTypeWithData2) && sm_listRecipe[i]->hasIngreItemTypeWithData(itemTypeWithData3)){
                    return sm_listRecipe[i];
                }
            }
        }
    }else if(itemTypeWithData2){
        for(unsigned i = 0;i<sm_listRecipe.size();i++){
            if(sm_listRecipe[i]->getNbIngre() == 2){
                if(sm_listRecipe[i]->hasIngreItemTypeWithData(itemTypeWithData1) && sm_listRecipe[i]->hasIngreItemTypeWithData(itemTypeWithData2)){
                    return sm_listRecipe[i];
                }
            }
        }
    }else if(itemTypeWithData1){
        for(unsigned i = 0;i<sm_listRecipe.size();i++){
            if(sm_listRecipe[i]->getNbIngre() == 1){
                if(sm_listRecipe[i]->hasIngreItemTypeWithData(itemTypeWithData1)){
                    return sm_listRecipe[i];
                }
            }
        }
    }else{
        cout << "cant find recipe in RecipeManager cause only nullptr was passed in arguments" << endl;
    }
    return nullptr;
}

unsigned RecipeManager::getMaxOuput(unsigned nbIngre){
    if(nbIngre-1 >= sm_listMaxOutput.size() || nbIngre-1 < 0){
        cout << "WARNING RecipeManager::getMaxOuput: nbIngre passed not managed" << endl;
        return 0;
    }
    return sm_listMaxOutput[nbIngre-1];
}

void RecipeManager::addItemTypeWithDataForLate(ItemTypeWithData *itemTypeWithData){
    sm_saveItemTypeWithData.push_back(itemTypeWithData);
}

Recipe* RecipeManager::getRecipeLate(){
    if(sm_saveItemTypeWithData.size()==3){
        return getRecipe(sm_saveItemTypeWithData[0], sm_saveItemTypeWithData[1], sm_saveItemTypeWithData[2]);
    }else if(sm_saveItemTypeWithData.size()==2){
        return getRecipe(sm_saveItemTypeWithData[0], sm_saveItemTypeWithData[1]);
    }else if(sm_saveItemTypeWithData.size()==1){
        return getRecipe(sm_saveItemTypeWithData[0]);
    }else if(sm_saveItemTypeWithData.size()==0){
        cout << "WARNING recipe with >3 ingredient are note handled yet" << endl;
    }else{
        cout << "WARNING getRecipeLate recipe 0 ingredient return nullptr. Did you forget to addItemType" << endl;
    }
    return nullptr;
}
void RecipeManager::clear(){
    sm_saveItemTypeWithData.clear();
}
