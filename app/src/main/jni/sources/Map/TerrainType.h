#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H

#include "../DataLoader.h"

class TerrainType
{
public:

    TerrainType(bool isCollectable, bool isBuildable, bool isLiquid, TerrainEnum terrainEnum);

    bool isCollectable() const;
    bool isBuildable() const;
    bool isLiquid() const;

    inline ItemType* getItemTypeCollected() const{
        return m_itemTypeCollected;
    }

    TerrainEnum getTerrainEnum() const;

private:
    bool m_isCollectable; //can we extract it?
    bool m_isBuildable; //can we build on it?
    bool m_isLiquid; //can we take it as liquid?

    ItemType* m_itemTypeCollected;

    TerrainEnum m_terrainEnum;
};

#endif // TERRAINTYPE_H


