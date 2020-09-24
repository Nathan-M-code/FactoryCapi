#include "Map.h"

using namespace std;
using namespace sf;

int Map::s_sizeTileTerrain = 256;
float Map::s_sizeTileBuildings = 0;

Map::Map(){
    m_clock.restart();
    m_actualTime = m_clock.getElapsedTime();
    m_previousTime = m_clock.getElapsedTime();
    m_lagTime = sf::milliseconds(0);

    m_nbTile = {20,20};
    m_size = m_nbTile*s_sizeTileTerrain;
    m_nbEntityPerSide = 4;
    s_sizeTileBuildings = s_sizeTileTerrain/(float)m_nbEntityPerSide;


    m_terrainDirt = new TerrainType(false,true,false,dirt);
    m_terrainWater = new TerrainType(false,false,true,water);

    //init lists
    //Terrain
    for(short row=0;row<m_nbTile.y;row++){
        m_listTerrainType.push_back({});
        for(short column=0;column<m_nbTile.x;column++){
            m_listTerrainType[m_listTerrainType.size()-1].push_back(m_terrainDirt);
        }
    }

    m_listTerrainType[5][5] = m_terrainWater;

    //Building
    for(short row=0;row<m_nbTile.y*m_nbEntityPerSide;row++){
        m_listBuildingToBuild.push_back({});
        for(short column=0;column<m_nbTile.x*m_nbEntityPerSide;column++){
            m_listBuildingToBuild[m_listBuildingToBuild.size()-1].push_back(nullptr);
        }
    }

    for(short row=0;row<m_nbTile.y*m_nbEntityPerSide;row++){
        m_listBuildingToRender.push_back({});
    }

    //TransportSystem
    for(short row=0;row<m_nbTile.y*m_nbEntityPerSide;row++){
        m_listTransportSystemToBuild.push_back({});
        for(short column=0;column<m_nbTile.x*m_nbEntityPerSide;column++){
            m_listTransportSystemToBuild[m_listTransportSystemToBuild.size()-1].push_back(nullptr);
        }
    }

    for(short row=0;row<m_nbTile.y*m_nbEntityPerSide;row++){
        m_listTransportSystemToRender.push_back({});
    }

    //init sprite
    m_spriteMap.setPosition(0,0);
    //init
    updateAllRenderTextureMap();
}
Map::~Map(){}

sf::Vector2i Map::getSize() const{
    return m_size;
}

sf::Vector2i Map::getCooTileTerrainByCooMap(const sf::Vector2f &cooMap){
    int row = (m_size.y - cooMap.y)/s_sizeTileTerrain;
    int col = cooMap.x/s_sizeTileTerrain;
    return sf::Vector2i(col,row);
}

sf::Vector2i Map::getCooTileBuildingByCooMap(const sf::Vector2f &cooMap){
    int row = cooMap.y/s_sizeTileBuildings;
    int col = cooMap.x/s_sizeTileBuildings;
    return sf::Vector2i(col,row);
}

bool Map::canBuildOnIt(const sf::Vector2i &cooTile){
    if(cooTile.y < 0 || cooTile.y >= m_nbTile.y*m_nbEntityPerSide || cooTile.x < 0 || cooTile.x >= m_nbTile.x*m_nbEntityPerSide){
        return false;
    }
    return !(m_listBuildingToBuild[cooTile.y][cooTile.x] || m_listTransportSystemToBuild[cooTile.y][cooTile.x]);
}

void Map::updateAllRenderTextureMap(){
    RectangleShape carre(Vector2f(s_sizeTileTerrain,s_sizeTileTerrain));
    m_renderTextureMap.create(m_size.x, m_size.y);


    for(short row=0;row<m_listTerrainType.size();row++){
        for(short column=0;column<m_listTerrainType[row].size();column++){

            carre.setPosition(column*s_sizeTileTerrain, row*s_sizeTileTerrain);
            carre.setTexture(m_listTerrainType[row][column]->getTexture());

            m_renderTextureMap.draw(carre);
        }
    }
    m_spriteMap.setTexture(m_renderTextureMap.getTexture());
}


void Map::addBuilding(BuildingEnum buildingEnum, const sf::Vector2f &cooMap){

    Building *b = nullptr;

    switch (buildingEnum){
        case CHEST1:
            b = new Chest1(&m_clock, getCooTileBuildingByCooMap(cooMap));
            break;
        case ASSEMBLINGMACHINE1:
            b = new AssemblingMachine1(&m_clock, getCooTileBuildingByCooMap(cooMap));
            break;
        default:
            cout << "the buildingEnum in addBuilding is not managed" << endl;
            b = nullptr;
            return;
    }

    sf::Vector2i posTileBuilding = getCooTileBuildingByCooMap(cooMap);
    sf::Vector2i sizeTile = b->getBuildingType()->getTileSizeBox();

    //test valid
    for(unsigned int row=posTileBuilding.y;row<posTileBuilding.y+sizeTile.y;row++){
        for(unsigned int col=posTileBuilding.x;col<posTileBuilding.x+sizeTile.x;col++){

            if(!canBuildOnIt(sf::Vector2i(col,row))){
                cout << "impossible de mettre un building ici" << endl;
                return;
            }

        }
    }

    //push back
    m_listEntity.push_back(b);
    //cout << "ajout d'un building dans m_listEntity" << endl;
    m_listBuildingToRender[posTileBuilding.y].push_back(b);
    //cout << "ajout d'un building dans m_listBuildingToRender" << endl;
    for(unsigned int row=posTileBuilding.y;row<posTileBuilding.y+sizeTile.y;row++){
        for(unsigned int col=posTileBuilding.x;col<posTileBuilding.x+sizeTile.x;col++){
            m_listBuildingToBuild[row][col] = b;
            //cout << "ajout d'un building dans m_listBuildingToBuild" << endl;
        }
    }

}

void Map::addTransportSystem(TransportSystemEnum transportSystemEnum, const sf::Vector2f &cooMap){

    sf::Vector2i posTileBuilding = getCooTileBuildingByCooMap(cooMap);

    TransportSystem *ts = nullptr;
    Direction from;
    Direction to;
    calculateDirection(posTileBuilding, from,to);

    switch (transportSystemEnum){
        case BELT1:
            ts = new Belt1(&m_clock, getCooTileBuildingByCooMap(cooMap), from, to);
            break;
        default:
            cout << "the transportSystemEnum in addTransportSystem is not managed" << endl;
            ts = nullptr;
            return;
    }

    //test valid
    if(!canBuildOnIt(posTileBuilding)){
        cout << "impossible de mettre un transportSystem ici" << endl;
        return;
    }

    //push back
    m_listEntity.push_back(ts);

    m_listTransportSystemToRender[posTileBuilding.y].push_back(ts);
    m_listTransportSystemToBuild[posTileBuilding.y][posTileBuilding.x] = ts;

    updateDirectionCloseTransport(posTileBuilding);

}

void Map::clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent,const bool pressed, const sf::Vector2f &cooMap){
    if(mouseButtonEvent.button == sf::Mouse::Button::Left){
        if(pressed){
            //addBuilding(ASSEMBLINGMACHINE1, cooMap);
            addTransportSystem(BELT1, cooMap);
        }
    }else if(mouseButtonEvent.button == sf::Mouse::Button::Right){
        if(pressed){
            addBuilding(ASSEMBLINGMACHINE1, cooMap);
            //addTransportSystem(BELT1, cooMap);
        }
    }
}

void Map::mouseMoveHandler(const sf::Vector2f &cooMap){

}

void Map::calculateDirection(sf::Vector2i cooTile, Direction &from, Direction &to){
    from = SOUTH;
    to = NORD;

    Direction otherFrom;
    Direction otherTo;

    if(cooTile.y+1 < m_listTransportSystemToBuild.size() && m_listTransportSystemToBuild[cooTile.y+1][cooTile.x]){
        if(cooTile.x-1 >= 0 && m_listTransportSystemToBuild[cooTile.y][cooTile.x-1]){
            m_listTransportSystemToBuild[cooTile.y+1][cooTile.x]->getDirection(otherFrom, otherTo);
            if(otherTo == SOUTH){
                from = WEST;
                to = SOUTH;
            }else if(otherTo == NORD){
                from = SOUTH;
                to = WEST;
            }
        }
    }
}

void Map::updateDirectionCloseTransport(sf::Vector2i cooTile){
    Direction newFromDir = NORD;
    Direction newToDir = SOUTH;
    if(cooTile.y+1 < m_listTransportSystemToBuild.size() && m_listTransportSystemToBuild[cooTile.y+1][cooTile.x]){
        calculateDirection(sf::Vector2i(cooTile.x, cooTile.y+1), newFromDir, newToDir);
        m_listTransportSystemToBuild[cooTile.y+1][cooTile.x]->setDirection(newFromDir, newToDir);
    }
    if(cooTile.y-1 >= 0 && m_listTransportSystemToBuild[cooTile.y-1][cooTile.x]){
        calculateDirection(sf::Vector2i(cooTile.x, cooTile.y-1), newFromDir, newToDir);
        m_listTransportSystemToBuild[cooTile.y-1][cooTile.x]->setDirection(newFromDir, newToDir);
    }
    if(cooTile.x+1 < m_listTransportSystemToBuild[cooTile.y].size() && m_listTransportSystemToBuild[cooTile.y][cooTile.x+1]){
        calculateDirection(sf::Vector2i(cooTile.x+1, cooTile.y), newFromDir, newToDir);
        m_listTransportSystemToBuild[cooTile.y][cooTile.x+1]->setDirection(newFromDir, newToDir);
    }
    if(cooTile.x-1 >= 0 && m_listTransportSystemToBuild[cooTile.y][cooTile.x-1]){
        calculateDirection(sf::Vector2i(cooTile.x-1, cooTile.y), newFromDir, newToDir);
        m_listTransportSystemToBuild[cooTile.y][cooTile.x-1]->setDirection(newFromDir, newToDir);
    }
}

void Map::update(){
    m_actualTime = m_clock.getElapsedTime();
    m_elapsedTime = m_actualTime-m_previousTime;
    m_previousTime = m_actualTime;
    //m_lagTime += m_elapsedTime;
    //cout << m_lagTime.asMilliseconds() << endl;

    Belt1::updateIndexSprite(m_elapsedTime);

    for(int i=0;i<m_listEntity.size();i++){
        m_listEntity[i]->update();
    }
}

void Map::draw(sf::RenderWindow &window){
    window.draw(m_spriteMap);

    for(int row=0;row<m_listBuildingToRender.size();row++){
        for(int i=0;i<m_listBuildingToRender[row].size();i++){
            m_listBuildingToRender[row][i]->draw(window);
        }
    }

    for(int row=0;row<m_listTransportSystemToRender.size();row++){
        for(int i=0;i<m_listTransportSystemToRender[row].size();i++){
            m_listTransportSystemToRender[row][i]->draw(window);
        }
    }
}

