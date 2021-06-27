#include "TerrainType.h"

using namespace std;
using namespace sf;

TerrainType::TerrainType(bool isCollectable, bool isBuildable,bool isLiquid, TerrainEnum terrainEnum):
    m_isCollectable(isCollectable),
    m_isBuildable(isBuildable),
    m_isLiquid(isLiquid),
    m_terrainEnum(terrainEnum)
{
    m_itemTypeCollected = nullptr;

    if(terrainEnum == wood) {
        m_itemTypeCollected = DataLoader::getItemType(0);
    }else if(terrainEnum == coal) {
        m_itemTypeCollected = DataLoader::getItemType(1);
    }else if(terrainEnum == water) {
        m_itemTypeCollected = DataLoader::getItemType(5);
    }
}


bool TerrainType::isCollectable() const{
    return m_isCollectable;
}
bool TerrainType::isBuildable() const{
    return m_isBuildable;
}
bool TerrainType::isLiquid() const{
    return m_isLiquid;
}

TerrainEnum TerrainType::getTerrainEnum() const{
    return m_terrainEnum;
}
