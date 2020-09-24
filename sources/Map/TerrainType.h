#ifndef TERRAINTYPE_H
#define TERRAINTYPE_H

#include "../TextureLoader.h"

class TerrainType
{
public:

    TerrainType(bool isCollectable, bool isBuildable,bool isLiquid, TerrainEnum terrainEnum);

    bool isCollectable() const;
    bool isBuildable() const;
    bool isLiquid() const;

    sf::Texture* getTexture() const;

    TerrainEnum getTerrainEnum() const;

private:
    bool m_isCollectable; //can we extract it?
    bool m_isBuildable; //can we build on it?
    bool m_isLiquid; //can we take it as liquid?
    sf::Texture *m_texture;

    TerrainEnum m_terrainEnum;

};

#endif // TERRAINTYPE_H


