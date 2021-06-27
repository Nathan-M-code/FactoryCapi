#include "QualityDataItem.h"

QualityDataItem::QualityDataItem(QualityEnum quality):
DataItem(DATA_QUALITY)
{
    m_quality = quality;
}

QualityEnum QualityDataItem::getQuality() const {
    return m_quality;
}
