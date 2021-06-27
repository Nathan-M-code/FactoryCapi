//
// Created by natha on 21/10/2020.
//

#ifndef TILEMAPTERRAIN_H
#define TILEMAPTERRAIN_H

#include "TerrainType.h"

class TileMapTerrain : public sf::Drawable, public sf::Transformable{
public:

    TileMapTerrain(sf::Vector2i nbTile, int sizeTileTerrain);
    ~TileMapTerrain();

    TerrainType* getTerrainType(unsigned row, unsigned col) const;

    bool load();

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


    sf::Vector2i m_nbTile;
    int m_sizeTileTerrain;

    sf::VertexArray m_vertices;
    sf::Texture* m_tileset;

    TerrainType *m_terrainDirt;
    TerrainType *m_terrainGrass;
    TerrainType *m_terrainWater;
    TerrainType *m_terrainCoal;
    TerrainType *m_terrainWood;
    std::vector<std::vector<TerrainType *>> m_listTerrainType;
};


#endif //TILEMAPTERRAIN_H
