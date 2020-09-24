#ifndef MAP_H
#define MAP_H

#include "../Entity/Building/Chest1.h"
#include "../Entity/Building/AssemblingMachine1.h"
#include "../Entity/TransportSystem/Belt1.h"
#include "TerrainType.h"

class Map{
public:
    Map();
    ~Map();

    sf::Vector2i getSize() const;

    sf::Vector2i getCooTileTerrainByCooMap(const sf::Vector2f &cooMap);
    sf::Vector2i getCooTileBuildingByCooMap(const sf::Vector2f &cooMap);

    bool canBuildOnIt(const sf::Vector2i &cooTile);

    void addBuilding(BuildingEnum buildingEnum, const sf::Vector2f &cooMap);
    void addTransportSystem(TransportSystemEnum transportSystemEnum, const sf::Vector2f &cooMap);

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const bool pressed, const sf::Vector2f &cooMap);
    void mouseMoveHandler(const sf::Vector2f &cooMap);

    void calculateDirection(sf::Vector2i cooTile, Direction &from, Direction &to);
    void updateDirectionCloseTransport(sf::Vector2i cooTile);

    void update();

    void draw(sf::RenderWindow &window);

    static int s_sizeTileTerrain;
    static float s_sizeTileBuildings;

private:
    sf::Clock m_clock;
    sf::Time m_actualTime;
    sf::Time m_previousTime;
    sf::Time m_elapsedTime;
    sf::Time m_lagTime;

    sf::Vector2i m_nbTile;
    sf::Vector2i m_size;
    unsigned m_nbEntityPerSide;

    TerrainType *m_terrainDirt;
    TerrainType *m_terrainWater;
    std::vector<std::vector<TerrainType *>> m_listTerrainType;


    sf::RenderTexture m_renderTextureMap;
    sf::Sprite m_spriteMap;
    void updateAllRenderTextureMap();


    std::vector<Entity *> m_listEntity;

    std::vector<std::vector<Building *>> m_listBuildingToBuild;
    std::vector<std::vector<Building *>> m_listBuildingToRender;

    std::vector<std::vector<TransportSystem *>> m_listTransportSystemToBuild; //to quickly access to tile
    std::vector<std::vector<TransportSystem *>> m_listTransportSystemToRender;
};


#endif //MAP_H
