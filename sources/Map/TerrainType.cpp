#include "TerrainType.h"

using namespace std;
using namespace sf;

TerrainType::TerrainType(bool isCollectable, bool isBuildable,bool isLiquid, TerrainEnum terrainEnum):
    m_isCollectable(isCollectable),
    m_isBuildable(isBuildable),
    m_isLiquid(isLiquid),
    m_terrainEnum(terrainEnum)
{
    m_texture = TextureLoader::getTextureTerrain(m_terrainEnum);
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

Texture* TerrainType::getTexture() const{
    return m_texture;
}

TerrainEnum TerrainType::getTerrainEnum() const{
    return m_terrainEnum;
}
