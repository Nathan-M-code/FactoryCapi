#ifndef WORLDMAPCHUNK_H
#define WORLDMAPCHUNK_H

#include "FactoryWorldMap.h"
#include "../DataLoader.h"

struct MapTile{
    unsigned waterScale;
    unsigned woodScale;
    unsigned coalScale;
};

class WorldMapChunk{
public:
    WorldMapChunk(const sf::Vector2i& cooChunk);
    ~WorldMapChunk();

    static sf::String getStrInfo(MapTile *pMapTile);

    static sf::Vector2i getSize();
    static sf::Vector2f getLittleSize();
    static const int getSizeTile();
    static const sf::Vector2i &getNbTile();
    static const float getH();
    static const float getR();
    static const float getS();

    sf::Vector2i getCooChunk() const;

    sf::Vector2i getCooTileAbsoluteByCooRelative(const sf::Vector2i& posTileRelative) const;

    sf::Vector2f getPosWorldByPosTileRelative(const sf::Vector2i& posTileRelative) const;
    //static sf::Vector2f getPosWorldByPosTileAbsolute(const sf::Vector2i& posTileAbsolute);

    sf::Vector2i getCooTilePosChunk(const sf::Vector2f& mousePosChunk) const;
    sf::Vector2i getCooTileByPosWorld(sf::Vector2f& mousePosWorld, sf::Vector2i& posTileRelative, sf::Vector2i& posTileAbsolute);

    FactoryWorldMap *getFactory(const sf::Vector2i &posRelative);
    MapTile *getMapTile(const sf::Vector2i &posRelative);

    bool load();

    void draw(sf::RenderWindow& window);
    void drawWorldPosUI(sf::RenderWindow &window);

private:
    sf::Vector2i m_cooChunk;
    sf::Vector2f m_pos;

    static const sf::Vector2i s_nbTile;
    static const int s_sizeTileTerrain;
    static const int s_sizeFactory;
    static const float h;
    static const float r;
    static const float s;
    static const sf::Vector2i s_size;
    static const sf::Vector2f s_littleSize;

    sf::RectangleShape m_rectArea;

    sf::VertexArray m_vertices;
    sf::Texture* m_tileset;

    std::vector<std::vector<MapTile>> m_doubleArrayMapTile;

    std::vector<std::vector<FactoryWorldMap*>> m_doubleArrayFactory;
    std::vector<FactoryWorldMap*> m_listFactory;
};


#endif //WORLDMAPCHUNK_H
