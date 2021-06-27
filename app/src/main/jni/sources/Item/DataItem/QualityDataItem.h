#ifndef QUALITYDATAITEM_H
#define QUALITYDATAITEM_H

#include "DataItem.h"

class QualityDataItem: public DataItem{
public:
    QualityDataItem(QualityEnum quality);

    QualityEnum getQuality() const;

private:
    QualityEnum m_quality;
};


#endif //QUALITYDATAITEM_H
