//
// Created by natha on 04/11/2020.
//

#include "DataItem.h"

DataItem::DataItem(DataItemLabel dataItemLabel){
    m_dataItemLabel = dataItemLabel;
}

DataItemLabel DataItem::getDataItemLabel() const {
    return m_dataItemLabel;
}
