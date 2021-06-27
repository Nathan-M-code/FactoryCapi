//
// Created by natha on 04/11/2020.
//

#ifndef DATAITEM_H
#define DATAITEM_H

#include "../../utile.h"

class DataItem {
public:
    DataItem(DataItemLabel dataItemLabel);

    virtual DataItemLabel getDataItemLabel() const;

private:
    DataItemLabel m_dataItemLabel;
};


#endif //DATAITEM_H
