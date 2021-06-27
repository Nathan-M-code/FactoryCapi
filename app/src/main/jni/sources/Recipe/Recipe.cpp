//
// Created by natha on 26/10/2020.
//

#include "Recipe.h"
#include "../DataLoader.h"

using namespace std;

Recipe::Recipe(std::vector<ItemType*> listIngredientType,
               std::vector<unsigned> listIngredientNumber,
               std::vector<std::vector<DataItem*>> listDataItemIngredient,
               std::vector<ItemType*> listOutputType,
               std::vector<unsigned> listOutputNumber,
               std::vector<std::vector<DataItem*>> listDataItemOutput,
               float timeSec,
               bool producesElectricity,
               unsigned amountElectricityProduced
               )
{
    if(listIngredientType.size() != listIngredientNumber.size() || listOutputType.size() != listOutputNumber.size()){
        cout << "WARNING: cant create Recipe: listIngredientType.size != listIngredientNumber.size or istOutputType.size() != listOutputNumber.size()" << endl;
        return;
    }

    for(int i=0;i<listIngredientType.size();i++){
        m_listIngredient.push_back(new ItemStack(new ItemTypeWithData(listIngredientType[i], listDataItemIngredient[i]), listIngredientNumber[i]));
    }

    for(int i=0;i<listOutputType.size();i++){
        m_listResult.push_back(new ItemStack(new ItemTypeWithData(listOutputType[i], listDataItemOutput[i]), listOutputNumber[i]));
    }

    m_timeProduction = timeSec;

    m_producesElectricity = producesElectricity;
    m_amountElectricityProduced = amountElectricityProduced;

    m_spriteArrow.setTexture(*DataLoader::getTextureArrowUI());
    m_spriteArrow.setScale(100.0/m_spriteArrow.getLocalBounds().width,50.0/m_spriteArrow.getLocalBounds().height);
    initRectStyle(m_bgInfo);
    initTextStyle(m_textInfoIngre, DataLoader::getFont(), 22);
    initTextStyle(m_textInfoRes, DataLoader::getFont(), 22);
    initTextStyle(m_textTime, DataLoader::getFont(), 24);

    string strIngre = m_listIngredient[0]->getString();
    for (int i = 1; i < m_listIngredient.size(); ++i) {
        strIngre += "\n-------------\n"+m_listIngredient[i]->getString();
    }
    m_textInfoIngre.setString(strIngre);

    string strRes = "";
    if(m_listResult.size()) {
        strRes += m_listResult[0]->getString();
        for (int i = 1; i < m_listResult.size(); ++i) {
            strRes += "\n-------------\n" + m_listResult[i]->getString();
        }
    }
    if(m_producesElectricity){
        strRes += "\n-------------\n"+to_string(m_amountElectricityProduced)+" kW";
    }
    //cout << "strRes: " << strRes << endl;
    m_textInfoRes.setString(strRes);

    m_textTime.setString(to_string(m_timeProduction));

    m_bgInfo.setSize({80 + m_textInfoIngre.getGlobalBounds().width + m_spriteArrow.getGlobalBounds().width + m_textInfoRes.getGlobalBounds().width,
                          max(m_textInfoIngre.getGlobalBounds().height,
                                            max(m_textInfoRes.getGlobalBounds().height,
                                                m_spriteArrow.getGlobalBounds().height*2+m_textTime.getGlobalBounds().height))
                            +20});

    setPosUI({0,500});
}
Recipe::~Recipe(){

}

unsigned Recipe::getNbIngreForType(ItemType *it){
    for(int i=0;i<m_listIngredient.size();i++){
        if(m_listIngredient[i]->getItemTypeWithData()->getItemType() == it){
            return m_listIngredient[i]->getNbItem();
        }
    }
    return 0;
}

std::vector<ItemStack*> Recipe::getListResult() const{
    return m_listResult;
}

std::vector<ItemStack*> Recipe::getListIngre() const{
    return m_listIngredient;
}

std::vector<ItemType*> Recipe::getListIngreType() const{
    std::vector<ItemType*> ret;
    for(int i=0;i<m_listIngredient.size();i++){
        ret.push_back(m_listIngredient[i]->getItemTypeWithData()->getItemType());
    }
    return ret;
}

bool Recipe::hasIngreType(ItemType* it) const{
    for(int i=0;i<m_listIngredient.size();i++){
        if(m_listIngredient[i]->getItemTypeWithData()->getItemType() == it){
            return true;
        }
    }
    return false;
}

bool Recipe::hasIngreItemTypeWithData(ItemTypeWithData* itemTypeWithData) const{
    for(int i=0;i<m_listIngredient.size();i++){
        if(m_listIngredient[i]->getItemTypeWithData()->sameAs(itemTypeWithData)){
            return true;
        }
    }
    return false;
}

float Recipe::getProductionTime() {
    return m_timeProduction;
}

//-------------------------------------------------------------------------

void Recipe::setPosUI(const sf::Vector2i &posScreen){
    m_bgInfo.setPosition(posScreen.x, posScreen.y);
    m_textInfoIngre.setPosition(m_bgInfo.getPosition().x+10,m_bgInfo.getPosition().y+m_bgInfo.getSize().y/2-m_textInfoIngre.getGlobalBounds().height/2);
    m_spriteArrow.setPosition(m_textInfoIngre.getPosition().x+m_textInfoIngre.getGlobalBounds().width+30,m_bgInfo.getPosition().y+m_bgInfo.getSize().y/2-m_spriteArrow.getGlobalBounds().height/2);
    m_textInfoRes.setPosition(m_spriteArrow.getPosition().x+m_spriteArrow.getGlobalBounds().width+30,
                              m_bgInfo.getPosition().y+m_bgInfo.getSize().y/2-m_textInfoRes.getGlobalBounds().height/2);

    m_textTime.setPosition(m_spriteArrow.getPosition().x+m_spriteArrow.getGlobalBounds().width/2-m_textTime.getGlobalBounds().width/2, m_spriteArrow.getPosition().y+m_spriteArrow.getGlobalBounds().height+5);
}

void Recipe::drawScreenPosUI(sf::RenderWindow &window){
    window.draw(m_bgInfo);
    window.draw(m_spriteArrow);
    window.draw(m_textInfoIngre);
    window.draw(m_textInfoRes);
    window.draw(m_textTime);
}

//-------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, Recipe &recipe){
    out << "recipe returns: " << endl;
    std::vector<ItemStack*> listResult = recipe.getListResult();
    for(int i=0;i<listResult.size();i++){
        out << listResult[i]->getNbItem() << " " << listResult[i]->getItemTypeWithData()->getItemType() << endl;
    }
    return out;
}