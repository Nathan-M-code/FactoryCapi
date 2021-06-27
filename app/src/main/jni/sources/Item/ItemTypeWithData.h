//
// Created by natha on 05/11/2020.
//

#ifndef ITEMTYPEWITHDATA_H
#define ITEMTYPEWITHDATA_H

#include "../utile.h"
#include "ItemType.h"

#include "DataItem/QualityDataItem.h"

class ItemTypeWithData {
public:
    ItemTypeWithData(ItemType* itemType, const std::vector<DataItem*> &listDataItem={});
    ItemTypeWithData(ItemTypeWithData* itemTypeWithData);

    inline ItemType* getItemType() const{
        return m_itemType;
    }

    inline const std::vector<DataItem*>& getListDataItem() const{
        return m_listDataItem;
    }

    bool sameAs(ItemTypeWithData *itemTypeWithData) const;

private:
    ItemType* m_itemType;
    std::vector<DataItem*> m_listDataItem;
};


#endif //ITEMTYPEWITHDATA_H
