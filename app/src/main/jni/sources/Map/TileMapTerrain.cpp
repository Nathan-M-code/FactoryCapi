//
// Created by natha on 21/10/2020.
//

#include "TileMapTerrain.h"

using namespace std;

TileMapTerrain::TileMapTerrain(sf::Vector2i nbTile, int sizeTileTerrain){
    m_nbTile = nbTile;
    m_sizeTileTerrain = sizeTileTerrain;

    m_terrainDirt = new TerrainType(false,true,false,dirt);
    m_terrainGrass = new TerrainType(false,true,false,grass);
    m_terrainWater = new TerrainType(false,false,true,water);
    m_terrainCoal = new TerrainType(true,true,false,coal);
    m_terrainWood = new TerrainType(true,true,false,wood);

    //Terrain
    for(unsigned short row=0;row<m_nbTile.y;row++){
        m_listTerrainType.push_back({});
        for(unsigned short column=0;column<m_nbTile.x;column++){
            m_listTerrainType[m_listTerrainType.size()-1].push_back(m_terrainGrass);
        }
    }

    m_listTerrainType[5][5] = m_terrainWater;
    m_listTerrainType[0][0] = m_terrainCoal;
    m_listTerrainType[0][1] = m_terrainWood;
    m_listTerrainType[5][2] = m_terrainCoal;

    m_tileset = DataLoader::getTextureTerrainTileSet();

}
TileMapTerrain::~TileMapTerrain(){}

TerrainType* TileMapTerrain::getTerrainType(unsigned row, unsigned col) const{
    return m_listTerrainType[row][col];
}

bool TileMapTerrain::load()
{

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Triangles);
    m_vertices.resize(m_nbTile.x * m_nbTile.y * 6); //6 because 2 triangles per quad

    // populate the vertex array, with one quad per tile
    for (unsigned int row = 0; row < m_listTerrainType.size(); row++)
        for (unsigned int col = 0; col < m_listTerrainType[row].size(); col++)
        {
            // get a pointer to the current tile's quad
            sf::Vertex* triangle = &m_vertices[(col + row * m_nbTile.x) * 6];

            // define 3 corners for triangle one
            triangle[0].position = sf::Vector2f(col * m_sizeTileTerrain, row * m_sizeTileTerrain);
            triangle[1].position = sf::Vector2f((col + 1) * m_sizeTileTerrain, row * m_sizeTileTerrain);
            triangle[2].position = sf::Vector2f((col + 1) * m_sizeTileTerrain, (row + 1) * m_sizeTileTerrain);
            // define 3 corners for triangle two
            triangle[3].position = sf::Vector2f(col * m_sizeTileTerrain, row * m_sizeTileTerrain);
            triangle[4].position = sf::Vector2f(col * m_sizeTileTerrain, (row + 1) * m_sizeTileTerrain);
            triangle[5].position = sf::Vector2f((col + 1) * m_sizeTileTerrain, (row + 1) * m_sizeTileTerrain);

            // define its 4 texture coordinates
            int tu = static_cast<int>(m_listTerrainType[row][col]->getTerrainEnum());
            // define 3 corners for triangle one
            triangle[0].texCoords = sf::Vector2f(tu * m_sizeTileTerrain, 0);
            triangle[1].texCoords = sf::Vector2f((tu + 1) * m_sizeTileTerrain, 0);
            triangle[2].texCoords = sf::Vector2f((tu + 1) * m_sizeTileTerrain, m_sizeTileTerrain);
            // define 3 corners for triangle two
            triangle[3].texCoords = sf::Vector2f(tu * m_sizeTileTerrain, 0);
            triangle[4].texCoords = sf::Vector2f(tu * m_sizeTileTerrain, m_sizeTileTerrain);
            triangle[5].texCoords = sf::Vector2f((tu + 1) * m_sizeTileTerrain, m_sizeTileTerrain);

        }

    return true;
}

void TileMapTerrain::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}
