//
// Created by natha on 05/11/2020.
//

#include "ItemTypeWithData.h"

using namespace std;

ItemTypeWithData::ItemTypeWithData(ItemType* itemType, const std::vector<DataItem*> &listDataItem){
    m_itemType = itemType;
    m_listDataItem = listDataItem;
}

ItemTypeWithData::ItemTypeWithData(ItemTypeWithData* itemTypeWithData){
    m_itemType = itemTypeWithData->getItemType();
    m_listDataItem = itemTypeWithData->getListDataItem();
}

bool ItemTypeWithData::sameAs(ItemTypeWithData *itemTypeWithData) const{
    if(m_itemType != itemTypeWithData->getItemType()){
        return false;
    }

    vector<DataItem*> otherListItemTypeWithData = itemTypeWithData->getListDataItem();
    if(m_listDataItem.empty() && otherListItemTypeWithData.empty()){
        return true;
    }

    if(m_listDataItem.size() != otherListItemTypeWithData.size()){
        return false;
    }

    for (unsigned i = 0; i < m_listDataItem.size(); ++i) {
        bool trouve = false;
        unsigned index = 0;
        while(!trouve && index<otherListItemTypeWithData.size()){
            if(m_listDataItem[i]->getDataItemLabel() == otherListItemTypeWithData[index]->getDataItemLabel()){
                if(m_listDataItem[i]->getDataItemLabel() == DATA_QUALITY){
                    auto* qualityDataItem = dynamic_cast<QualityDataItem*>(m_listDataItem[i]);
                    auto* otherQualityDataItem = dynamic_cast<QualityDataItem*>(otherListItemTypeWithData[index]);
                    if(qualityDataItem->getQuality() == otherQualityDataItem->getQuality()){
                        trouve = true;
                    }
                }else{
                    cout << "ItemTypeWithData::sameAs DataItem not managed !!!!!" << endl;
                }
            }
            index++;
        }
        if(!trouve){
            return false;
        }
    }

    return true;
}