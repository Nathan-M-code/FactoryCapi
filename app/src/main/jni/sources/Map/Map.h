#ifndef MAP_H
#define MAP_H

#include "../Entity/Building/AssemblingMachine1.h"
#include "../Entity/Building/Chest1.h"
#include "../Entity/Building/Inserter1.h"
#include "../Entity/Building/Belt1.h"
#include "../Entity/Building/Drill1.h"
#include "../Entity/Building/WorkInProgress.h"
#include "../Entity/Building/Pump.h"
#include "../Entity/Building/Boiler.h"
#include "../Entity/Building/ElectricalBuilding/Pole1.h"

#include "../Entity/PhysicalItem/PhysicalItem.h"

#include "../Entity/Robot/Robot.h"

#include "../BuildMode.h"
#include "../Broadcast.h"

#include "TileMapTerrain.h"
#include "../Electricity/ElectricalCircuit.h"

class Map{
public:
    Map(sf::RenderWindow* renderWindow);
    ~Map();

    inline sf::Clock *getClock(){
        return &m_clock;
    }

    sf::Vector2i getSize() const;

    sf::Vector2i getCooTileTerrainByCooMap(const sf::Vector2f &cooMap);
    sf::Vector2i getCooTileBuildingByCooMap(const sf::Vector2f &cooMap);
    sf::Vector2i getCooTileTerrainByCooTileBuilding(const sf::Vector2i &cooTileBuilding);

    bool canBuildOnIt(const sf::Vector2i &cooTile);

    void createAndPlaceWorkInProgressFromBuildMode();
    void createAndPlaceWorkInProgressDeconstruct(Building* buildingToDeconstruct);
    void finishWIP(const sf::Vector2i &posTile, WorkInProgress *wip);

    void createAndPlaceBuilding(unsigned numBuildingType, const sf::Vector2i &posTile, QualityEnum quality, Direction from, Direction to);
    bool canBuildHere(const sf::Vector2i &posTileBuilding, const sf::Vector2i &sizeBuilding);
    bool addBuilding(Building *b, const sf::Vector2i &posTileBuilding); //return false if it cant add the building

    bool deconstructionBuilding(Building *b); //check if can be deconstruct and place WIP
    void deleteFromListBuilding(Building *b); //remove building from lists

    void findIOProcessBuilding(ProcessBuilding *pb,const sf::Vector2i &posTileBuilding);

    void computeElectricalCircuit(ElectricalBuilding *eb);
    void findElectricalBuilding(Building *building);

    /*void createAndPlaceTransportSystem(unsigned numTransportType, const sf::Vector2i &posTile, Direction from, Direction to);
    void adaptDirection(const sf::Vector2i &posTile, const Direction currentFrom, const Direction currentTo, Direction &returnFrom, Direction &returnTo);
    void nearAdaptDirection(const sf::Vector2i &posTile, const Direction currentFrom, const Direction currentTo);*/

    void createRobot(const sf::Vector2f &cooMap, WorkInProgress *wip);
    void destroyRobot(Robot *robot);

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent);
    void keyPressedEventHandler(sf::Event::KeyEvent &keyEvent);
    void keyReleasedEventHandler(sf::Event::KeyEvent &keyEvent);
    void mouseMoveHandler();
    void wheelEventHandler(sf::Event::MouseWheelEvent &mouseWheelEvent);

    void selectBuildingOrTransport(const sf::Vector2i &posTile);

    void update();

    void draw(sf::RenderWindow &window);
    void drawWorldPosUI(sf::RenderWindow &window);
    void drawScreenPosUI(sf::RenderWindow &window);

    static int s_sizeTileTerrain;
    static float s_sizeTileBuildings;

private:
    sf::RenderWindow* m_renderWindow;

    sf::View m_view;
    float m_speedView;
    sf::Vector2i m_cooScreen;
    sf::Vector2f m_cooMap;
    sf::Vector2i m_posTileBuilding;

    sf::Text m_textCamera;

    sf::Clock m_clock;
    sf::Time m_actualTime;
    sf::Time m_previousTime;
    sf::Time m_elapsedTime;
    sf::Time m_lagTime;

    BuildMode m_buildMode;

    sf::Vector2i m_nbTile;
    sf::Vector2i m_size;
    int m_nbEntityPerSide;

    TileMapTerrain* m_tileMap;

    std::vector<Entity *> m_listEntity;

    std::vector<std::vector<ProcessBuilding *>> m_listProcessBuildingToBuild;

    std::vector<TransportSystem *> m_listTransportSystemToRender; //list useful to call drawItem

    std::vector<std::vector<Building *>> m_listBuildingToBuild; //to quickly access to tile[row][col]
    std::vector<std::vector<Building *>> m_listBuildingToRender;

    std::vector<Robot *> m_listRobotToRender;

    std::vector<ElectricalBuilding *> m_listElectricalBuilding;
    std::vector<ElectricalCircuit *> m_listElectricalCircuit;
    ElectricalCircuit * m_electricalCircuitSelected;

    Building *m_buildingSelected;
    sf::Sprite m_listSpriteSelected[4];


    //------------------------------------
    void addUniqueInVector(std::vector<ProcessBuilding*> &vec, ProcessBuilding* ptr);
    void addUniqueInVector(std::vector<ElectricalCircuit*> &vec, ElectricalCircuit* ec);
    void removeFromListEC(const std::vector<ElectricalCircuit*> &listECInRange);

public:
    void checkViewPos();
    void moveView();
    void zoomView(int delta);

    void updatePosVariables();
};


#endif //MAP_H
