#include "ItemStack.h"

using namespace std;

ItemStack::ItemStack(ItemTypeWithData *itemTypeWithData, unsigned nbItem){
    m_itemTypeWithData = itemTypeWithData;

    m_nbItem = nbItem;
}
ItemStack::ItemStack(ItemStack *is){
    m_itemTypeWithData = is->getItemTypeWithData();
    m_nbItem = is->getNbItem();
}
ItemStack::~ItemStack(){

}

void ItemStack::setNbItem(unsigned nbItem){
    m_nbItem = nbItem;
}

std::string ItemStack::getString() const{
    string ret = "";
    ret += to_string(m_nbItem)+" "+m_itemTypeWithData->getItemType()->getItemName();

    vector<DataItem*> listDataItem = m_itemTypeWithData->getListDataItem();
    if(listDataItem.size() > 0)
        ret+=" :\n";

    for (int i = 0; i < listDataItem.size(); ++i) {
        if(listDataItem[i]->getDataItemLabel() == DATA_QUALITY){
            QualityDataItem* qualityDataItem = dynamic_cast<QualityDataItem*>(listDataItem[i]);
            ret += "  - "+qualityToString(qualityDataItem->getQuality());
        }
    }
    return ret;
}

bool ItemStack::isStackableWith(ItemStack *is) const{
    return (m_itemTypeWithData->sameAs(is->getItemTypeWithData()));
}

void ItemStack::draw(sf::RenderWindow &window){}
void ItemStack::drawScreenPosUI(sf::RenderWindow &window) {}

