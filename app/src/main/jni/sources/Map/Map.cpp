#include "Map.h"
#include "../OptionManager.h"

using namespace std;
using namespace sf;

int Map::s_sizeTileTerrain = 256;
float Map::s_sizeTileBuildings = 0;

Map::Map(sf::RenderWindow* renderWindow): m_buildMode(BuildMode(this))
{
    LoggerFile::write("Map constructor");
    cout << "init map" << endl;

    m_renderWindow = renderWindow;

    Vector2i screenSize = OptionManager::getScreenSize();
    m_view = View(FloatRect(0,0,screenSize.x,screenSize.y));
    m_speedView = OptionManager::getSpeedCam();
    m_movingView = false;

    initTextStyle(m_textCamera,DataLoader::getFont(),12);

    m_clock.restart();
    m_actualTime = m_clock.getElapsedTime();
    m_previousTime = m_clock.getElapsedTime();
    m_lagTime = sf::milliseconds(0);

    m_buildingSelected = nullptr;
    m_listSpriteSelected[0] = Sprite(*DataLoader::getTextureSelectedCorner());
    m_listSpriteSelected[1] = Sprite(*DataLoader::getTextureSelectedCorner());
    m_listSpriteSelected[1].setRotation(90);
    m_listSpriteSelected[2] = Sprite(*DataLoader::getTextureSelectedCorner());
    m_listSpriteSelected[2].setRotation(180);
    m_listSpriteSelected[3] = Sprite(*DataLoader::getTextureSelectedCorner());
    m_listSpriteSelected[3].setRotation(270);

    m_nbTile = {20,20};
    m_size = m_nbTile*s_sizeTileTerrain;
    m_nbEntityPerSide = 4;
    s_sizeTileBuildings = s_sizeTileTerrain/(float)m_nbEntityPerSide;

    //init lists
    m_tileMap = new TileMapTerrain(m_nbTile, s_sizeTileTerrain);
    m_tileMap->load();

    //Building
    for(unsigned short row=0;row<m_nbTile.y*m_nbEntityPerSide;row++){
        m_listBuildingToBuild.push_back({});
        for(unsigned short column=0;column<m_nbTile.x*m_nbEntityPerSide;column++){
            m_listBuildingToBuild[m_listBuildingToBuild.size()-1].push_back(nullptr);
        }
    }

    for(unsigned short row=0;row<m_nbTile.y*m_nbEntityPerSide;row++){
        m_listBuildingToRender.push_back({});
    }

    //processBuilding
    for(unsigned short row=0;row<m_nbTile.y*m_nbEntityPerSide;row++){
        m_listProcessBuildingToBuild.push_back({});
        for(unsigned short column=0;column<m_nbTile.x*m_nbEntityPerSide;column++){
            m_listProcessBuildingToBuild[m_listProcessBuildingToBuild.size()-1].push_back(nullptr);
        }
    }

    //electricalCircuit
    m_electricalCircuitSelected = nullptr;

    LoggerFile::write("Map end constructor");
}
Map::~Map(){}

sf::Vector2i Map::getSize() const{
    return m_size;
}

sf::Vector2i Map::getCooTileTerrainByCooMap(const sf::Vector2f &cooMap){
    int row = cooMap.y/s_sizeTileTerrain;
    int col = cooMap.x/s_sizeTileTerrain;
    return sf::Vector2i(col,row);
}

sf::Vector2i Map::getCooTileBuildingByCooMap(const sf::Vector2f &cooMap){
    int row = cooMap.y/s_sizeTileBuildings;
    int col = cooMap.x/s_sizeTileBuildings;
    return sf::Vector2i(col,row);
}

sf::Vector2i Map::getCooTileTerrainByCooTileBuilding(const sf::Vector2i &cooTileBuilding){
    return sf::Vector2i(cooTileBuilding.x/m_nbEntityPerSide,cooTileBuilding.y/m_nbEntityPerSide);
}

bool Map::canBuildOnIt(const sf::Vector2i &cooTile){
    if(cooTile.y < 0 || cooTile.y >= m_nbTile.y*m_nbEntityPerSide || cooTile.x < 0 || cooTile.x >= m_nbTile.x*m_nbEntityPerSide){
        return false;
    }
    sf::Vector2i cooTileTerrain = getCooTileTerrainByCooTileBuilding(cooTile);
    if(!m_tileMap->getTerrainType(cooTileTerrain.y,cooTileTerrain.x)->isBuildable()){
        return false;
    }
    return !(m_listBuildingToBuild[cooTile.y][cooTile.x]);
}

//------------------------------------------------------------------------
void Map::checkViewPos(){
    if(m_view.getCenter().y<= m_view.getSize().y/2.0){
        m_view.setCenter(m_view.getCenter().x,m_view.getSize().y/2.0);
    }
    if(m_view.getCenter().x<= m_view.getSize().x/2.0){
        m_view.setCenter(m_view.getSize().x/2.0,m_view.getCenter().y);
    }
    if(m_size.y - m_view.getCenter().y <= m_view.getSize().y/2.0){
        m_view.setCenter(m_view.getCenter().x,m_size.y-m_view.getSize().y/2.0);
    }
    if(m_size.x - m_view.getCenter().x <= m_view.getSize().x/2.0){
        m_view.setCenter(m_size.x-m_view.getSize().x/2.0,m_view.getCenter().y);
    }
    m_view.setCenter(round(m_view.getCenter().x), round(m_view.getCenter().y));
}

//method 2:save the original pos of finger and move the view in relation to
void Map::moveViewWithFinger(){
    Vector2i cooScreen = m_listCooScreenFinger.at(0);
    Vector2f cooMap = m_renderWindow->mapPixelToCoords(cooScreen,m_saveViewStartMovingView);

    Vector2i delta(cooMap.x-m_saveCooMapFinger1StartMovingView.x, cooMap.y-m_saveCooMapFinger1StartMovingView.y);

    /*Broadcast::announce(to_string(cooScreen.x)+" "+to_string(cooScreen.y)+"\n"
                        +to_string(m_saveCooMapFinger1StartMovingView.x)+" "+to_string(m_saveCooMapFinger1StartMovingView.y)+"\n"
                        +to_string(delta.x)+" "+to_string(delta.y)+"\n"
                        +to_string(m_saveViewStartMovingView.getCenter().x)+" "+to_string(m_saveViewStartMovingView.getCenter().y)+"\n",1,Vector2f(10,200));*/

    m_view.setCenter(m_saveViewStartMovingView.getCenter().x-delta.x, m_saveViewStartMovingView.getCenter().y-delta.y);
    checkViewPos();

    //m_buildMode.updatePos(m_posTileBuilding);
}

void Map::zoomViewWithFinger() {
    Vector2i cooScreenFinger1 = m_listCooScreenFinger.at(0);
    Vector2i cooScreenFinger2 = m_listCooScreenFinger.at(1);

    Vector2f cooMapFinger1CurrentView = m_renderWindow->mapPixelToCoords(cooScreenFinger1,m_view);
    Vector2f cooMapFinger2CurrentView = m_renderWindow->mapPixelToCoords(cooScreenFinger2,m_view);

    /*Vector2f center = getCenter(cooMapFinger1CurrentView, cooMapFinger2CurrentView);
    Vector2f deltaCenter(center.x-m_view.getCenter().x, center.y-m_view.getCenter().y);*/

    //previous dist
    double dist = distance(m_saveCooScreenFinger1StartMovingView, m_saveCooScreenFinger2StartMovingView);

    //new dist
    double newDist = distance(cooScreenFinger1, cooScreenFinger2);

    double scalar = dist/newDist;
    //double scalar_1 = newDist/dist;

    /*Broadcast::announce(to_string(cooScreenFinger1.x)+" "+to_string(cooScreenFinger1.y)+"\n"
                        +to_string(cooScreenFinger2.x)+" "+to_string(cooScreenFinger2.y)+"\n"
                        +to_string(m_saveCooScreenFinger1StartMovingView.x)+" "+to_string(m_saveCooScreenFinger1StartMovingView.y)+"\n"
                        +to_string(m_saveCooScreenFinger2StartMovingView.x)+" "+to_string(m_saveCooScreenFinger2StartMovingView.y)+"\n"
                        +to_string(dist)+"\n"
                        +to_string(newDist)+"\n"
                        +to_string(scalar)+"\n",1,Vector2f(10,200));*/

    m_view.setSize(m_saveViewStartMovingView.getSize().x*scalar, m_saveViewStartMovingView.getSize().y*scalar);

    /*if(scalar >= 1){
        m_view.setCenter(m_view.getCenter().x+deltaCenter.x*scalar, m_view.getCenter().y+deltaCenter.y*scalar);
    }else{
        m_view.setCenter(m_view.getCenter().x+deltaCenter.x*scalar_1, m_view.getCenter().y+deltaCenter.y*scalar_1);
    }*/

    if(m_view.getSize().x <= 500){
        m_view.setSize(500,281.25);
    }
    if(m_view.getSize().x >= 3500){
        m_view.setSize(3500,1968.75);
    }

    checkViewPos();
    //m_buildMode.updatePos(m_posTileBuilding);
}

//---------------------------------------------------------------------------------------

void Map::createAndPlaceWorkInProgressFromBuildMode(){
    Building *b = m_buildMode.getBuilding();
    BuildingType *bt = b->getBuildingType();
    sf::Vector2i posTileWIP = b->getPosTile();

    sf::Vector2i sizeTile = bt->getTileSizeBox()[b->getNumTexture()];

    //test valid
    for(int row=posTileWIP.y;row<posTileWIP.y+sizeTile.y;row++){
        for(int col=posTileWIP.x;col<posTileWIP.x+sizeTile.x;col++){
            if(!canBuildOnIt(sf::Vector2i(col,row))){
                Broadcast::error("impossible de mettre un WIP ici",5,{0,0});
                return;
            }
        }
    }


    WorkInProgress *wip = new WorkInProgress(this,&m_clock,CONSTRUCTION,b);

    //push back
    m_listEntity.push_back(wip);
    //cout << "ajout d'un wip dans m_listEntity" << endl;
    m_listBuildingToRender[posTileWIP.y].push_back(wip);
    //cout << "ajout d'un wip dans m_listBuildingToBuild" << endl;
    for(int row=posTileWIP.y;row<posTileWIP.y+sizeTile.y;row++){
        for(int col=posTileWIP.x;col<posTileWIP.x+sizeTile.x;col++){
            m_listBuildingToBuild[row][col] = wip;
            //cout << "ajout d'un wip dans m_listBuildingToBuild" << endl;
        }
    }

    //create robot
    createRobot({-50,-50},wip);
}

void Map::createAndPlaceWorkInProgressDeconstruct(Building* buildingToDeconstruct){
    sf::Vector2i sizeTile = buildingToDeconstruct->getBuildingType()->getTileSizeBox()[buildingToDeconstruct->getNumTexture()];
    sf::Vector2i posTileWIP = buildingToDeconstruct->getPosTile();

    //test valid
    for(int row=posTileWIP.y;row<posTileWIP.y+sizeTile.y;row++){
        for(int col=posTileWIP.x;col<posTileWIP.x+sizeTile.x;col++){
            if(!canBuildOnIt(sf::Vector2i(col,row))){
                cout << "impossible de mettre un WIP ici" << endl;
                return;
            }
        }
    }

    WorkInProgress *wip = new WorkInProgress(this,&m_clock,DECONSTRUCTION,buildingToDeconstruct);

    //push back
    m_listEntity.push_back(wip);
    //cout << "ajout d'un wip dans m_listEntity" << endl;
    m_listBuildingToRender[posTileWIP.y].push_back(wip);
    //cout << "ajout d'un wip dans m_listBuildingToBuild" << endl;
    for(int row=posTileWIP.y;row<posTileWIP.y+sizeTile.y;row++){
        for(int col=posTileWIP.x;col<posTileWIP.x+sizeTile.x;col++){
            m_listBuildingToBuild[row][col] = wip;
            //cout << "ajout d'un wip dans m_listBuildingToBuild" << endl;
        }
    }

    //create robot
    createRobot({-50,-50},wip);
}

void Map::finishWIP(const sf::Vector2i &posTile, WorkInProgress *wip){
    //call by WorkInProgress::update()
    //cout << "end WIP" << endl;


    //Delete wip
    for(unsigned i=0;i<m_listEntity.size();i++){
        if(m_listEntity[i] == wip){
            m_listEntity.erase(m_listEntity.begin()+i);
            break;
        }
    }

    for(unsigned row=0;row<m_listBuildingToBuild.size();row++){
        for(unsigned col=0;col<m_listBuildingToBuild[row].size();col++){
            if(m_listBuildingToBuild[row][col] == wip){
                m_listBuildingToBuild[row][col] = nullptr;
            }
        }
    }

    for(unsigned row=0;row<m_listBuildingToRender.size();row++){
        for(unsigned i=0;i<m_listBuildingToRender[row].size();i++){
            if(m_listBuildingToRender[row][i] == wip){
                m_listBuildingToRender[row].erase(m_listBuildingToRender[row].begin()+i);
                i--;
            }
        }
    }

    if(wip->getWipStatus() == CONSTRUCTION) {
        //place
        BuildingType *bt = wip->getBuildingType();

        Direction from;
        Direction to;
        wip->getDirection(from, to);
        createAndPlaceBuilding(bt->getNum(), posTile, wip->getQuality(), from, to);
    }

    if (wip == m_buildingSelected) {
        m_buildingSelected = nullptr;
    }
    delete wip;
}


void Map::createAndPlaceBuilding(unsigned numBuildingType, const sf::Vector2i &posTile, QualityEnum quality, Direction from, Direction to){
    Building *b = nullptr;

    if(numBuildingType==0) {
        b = new Belt1(this, &m_clock, posTile, from, to);
    }

    else if(numBuildingType==1) {
        b = new Chest1(this, &m_clock, posTile);
    }

    else if(numBuildingType==2) {
        b = new AssemblingMachine1(this, &m_clock, posTile, quality);
    }

    else if(numBuildingType==3) {
        Drill1 *d = new Drill1(this, &m_clock, posTile, quality);
        sf::Vector2i sizeBuilding = d->getBuildingType()->getTileSizeBox()[d->getNumTexture()];
        vector<ItemType*> listItemToProduce;
        for(int row=posTile.y;row<posTile.y+sizeBuilding.y;row++){
            for(int col=posTile.x;col<posTile.x+sizeBuilding.x;col++){
                Vector2i cooTileTerrain = getCooTileTerrainByCooTileBuilding({col,row});
                //cout << cooTileTerrain << endl;
                ItemType *it = m_tileMap->getTerrainType(cooTileTerrain.y,cooTileTerrain.x)->getItemTypeCollected();
                if(it)
                    listItemToProduce.push_back(it);
            }
        }
        d->setListItemTypeToProduce(listItemToProduce);
        b = d;
    }

    else if(numBuildingType==4) {
        Pump *p = new Pump(this, &m_clock, posTile, from, to, quality);
        sf::Vector2i tileSizeBox = p->getBuildingType()->getTileSizeBox()[p->getNumTexture()];
        vector<ItemType*> listItemToProduce;
        if(from == NORD){
            Vector2i cooTileTerrain = getCooTileTerrainByCooTileBuilding({posTile.x,posTile.y-1});
            if(posTile.y-1 > 0) {
                TerrainType* tt = m_tileMap->getTerrainType(cooTileTerrain.y, cooTileTerrain.x);
                if(tt->isLiquid()) {
                    ItemType *it = tt->getItemTypeCollected();
                    if (it)
                        listItemToProduce.push_back(it);
                }
            }
        }else if(from == EAST){
            Vector2i cooTileTerrain = getCooTileTerrainByCooTileBuilding({posTile.x+tileSizeBox.x,posTile.y});
            if(posTile.x+tileSizeBox.x < m_listBuildingToBuild[posTile.y].size()) {
                TerrainType* tt = m_tileMap->getTerrainType(cooTileTerrain.y, cooTileTerrain.x);
                if(tt->isLiquid()) {
                    ItemType *it = tt->getItemTypeCollected();
                    if (it)
                        listItemToProduce.push_back(it);
                }
            }
        }else if(from == SOUTH){
            Vector2i cooTileTerrain = getCooTileTerrainByCooTileBuilding({posTile.x,posTile.y+tileSizeBox.y});
            if(posTile.y+tileSizeBox.y < m_listBuildingToBuild.size()) {
                TerrainType* tt = m_tileMap->getTerrainType(cooTileTerrain.y, cooTileTerrain.x);
                if(tt->isLiquid()) {
                    ItemType *it = tt->getItemTypeCollected();
                    if (it)
                        listItemToProduce.push_back(it);
                }
            }
        }else if(from == WEST){
            Vector2i cooTileTerrain = getCooTileTerrainByCooTileBuilding({posTile.x-1,posTile.y});
            if(posTile.x-1 > 0) {
                TerrainType* tt = m_tileMap->getTerrainType(cooTileTerrain.y, cooTileTerrain.x);
                if(tt->isLiquid()) {
                    ItemType *it = tt->getItemTypeCollected();
                    if (it)
                        listItemToProduce.push_back(it);
                }
            }
        }
        p->setListItemTypeToProduce(listItemToProduce);
        b = p;
    }

    else if(numBuildingType==5) {
        b = new Boiler(this, &m_clock, posTile, from, to, quality);
    }

    else if(numBuildingType==6) {
        b = new Pole1(this, &m_clock, posTile, quality);
    }


    else {
        cout << "the numBuildingType in createAndPlaceBuilding is not managed: Map.cpp" << endl;
        b = nullptr;
        return;
    }

    if(!canBuildHere(posTile, b->getBuildingType()->getTileSizeBox()[b->getNumTexture()])) {
        delete b;
        return;
    }
    if(!addBuilding(b, posTile)) {
        delete b;
    }
}

bool Map::canBuildHere(const sf::Vector2i &posTileBuilding, const sf::Vector2i &sizeBuilding){
    //test valid
    for(int row=posTileBuilding.y;row<posTileBuilding.y+sizeBuilding.y;row++){
        for(int col=posTileBuilding.x;col<posTileBuilding.x+sizeBuilding.x;col++){
            if(!canBuildOnIt(sf::Vector2i(col,row))){
                cout << "impossible de mettre un building ici" << endl;
                return false;
            }
        }
    }
    return true;
}

bool Map::addBuilding(Building *b, const sf::Vector2i &posTileBuilding){

    sf::Vector2i sizeTile = b->getBuildingType()->getTileSizeBox()[b->getNumTexture()];

    //push back
    //cout << "ajout d'un building dans m_listEntity" << endl;
    m_listEntity.push_back(b);
    //cout << "ajout d'un building dans m_listBuildingToRender" << endl;
    m_listBuildingToRender[posTileBuilding.y].push_back(b);

    ElectricalBuilding *eb = dynamic_cast<ElectricalBuilding*>(b);
    if(eb){
        m_listElectricalBuilding.push_back(eb);
        computeElectricalCircuit(eb);
    }else{
        //the building is not an electrical buidling
        findElectricalBuilding(b);
    }

    ProcessBuilding *pb = dynamic_cast<ProcessBuilding*>(b);
    if(pb){
        //cout << "the building is a ProcessBuilding, we add it in m_listProcessBuildingToBuild" << endl;
        for(int row=posTileBuilding.y;row<posTileBuilding.y+sizeTile.y;row++){
            for(int col=posTileBuilding.x;col<posTileBuilding.x+sizeTile.x;col++){
                m_listProcessBuildingToBuild[row][col] = pb;
                //cout << "ajout d'un building dans m_listBuildingToBuild" << endl;
            }
        }

        findIOProcessBuilding(pb,posTileBuilding);

        TransportSystem *ts = dynamic_cast<TransportSystem*>(pb);
        if(ts){
            //cout << "the ProcessBuilding is a TransportSystem, we add it in m_listTransportSystemToRender" << endl;
            m_listTransportSystemToRender.push_back(ts);
        }
    }

    for(int row=posTileBuilding.y;row<posTileBuilding.y+sizeTile.y;row++){
        for(int col=posTileBuilding.x;col<posTileBuilding.x+sizeTile.x;col++){
            m_listBuildingToBuild[row][col] = b;
            //cout << "ajout d'un building dans m_listBuildingToBuild" << endl;
        }
    }
    return true;
}

bool Map::deconstructionBuilding(Building *b){
    if(b->canBeDeconstructed()) {
        BuildingType * saveBt = b->getBuildingType();
        Vector2i savePosTile = b->getPosTile();

        deleteFromListBuilding(b);
        //add wip
        createAndPlaceWorkInProgressDeconstruct(b);

        //delete from memory
        delete b;
    }
}

void Map::deleteFromListBuilding(Building *b){
    sf::Vector2i sizeTile = b->getBuildingType()->getTileSizeBox()[b->getNumTexture()];
    sf::Vector2i posTileBuilding = b->getPosTile();

    ProcessBuilding *pb = dynamic_cast<ProcessBuilding*>(b);
    if(pb){
        pb->deselectOutputProcessBuilding(); //will remove input for the output's pb
        //remove output for input
        vector<ProcessBuilding*> listInput = pb->getAllInputBuilding();
        for (unsigned i = 0; i < listInput.size(); ++i) {
            listInput[i]->removeOutputProcessBuilding(pb);
        }
        //remove from lists

        for(int row=posTileBuilding.y;row<posTileBuilding.y+sizeTile.y;row++){
            for(int col=posTileBuilding.x;col<posTileBuilding.x+sizeTile.x;col++){
                m_listProcessBuildingToBuild[row][col] = nullptr;
                //cout << "remove d'un building dans m_listBuildingToBuild" << endl;
            }
        }

        TransportSystem* ts = dynamic_cast<TransportSystem*>(b);
        if(ts) {
            for (unsigned i = 0; i < m_listTransportSystemToRender.size(); ++i) {
                if (m_listTransportSystemToRender[i] == b) {
                    m_listTransportSystemToRender.erase(m_listTransportSystemToRender.begin() + i);
                    break;
                }
            }
        }
    }

    if(b == m_buildingSelected){
        m_buildingSelected = nullptr;
    }

    for (unsigned i = 0; i < m_listEntity.size(); ++i) {
        if(m_listEntity[i] == b){
            m_listEntity.erase(m_listEntity.begin()+i);
            break;
        }
    }

    for(int row=posTileBuilding.y;row<posTileBuilding.y+sizeTile.y;row++){
        for(int col=posTileBuilding.x;col<posTileBuilding.x+sizeTile.x;col++){
            m_listBuildingToBuild[row][col] = nullptr;
            //cout << "remove d'un building dans m_listBuildingToBuild" << endl;
        }
    }

    for (unsigned i = 0; i < m_listBuildingToRender[posTileBuilding.y].size(); ++i) {
        if(m_listBuildingToRender[posTileBuilding.y][i] == b){
            m_listBuildingToRender[posTileBuilding.y].erase(m_listBuildingToRender[posTileBuilding.y].begin()+i);
        }
    }


    //if we just removed a electrical pole
    ElectricalBuilding *eb = dynamic_cast<ElectricalBuilding*>(b);
    if(eb){
        sf::Time startTime = m_clock.getElapsedTime();
        //remove the eb from the list
        for (unsigned i = 0; i < m_listElectricalBuilding.size(); ++i) {
            if(m_listElectricalBuilding[i] == eb){
                m_listElectricalBuilding.erase(m_listElectricalBuilding.begin()+i);
                break;
            }
        }
        //recompute the electrical pole for each building in the area
        sf::Vector2i startPosTileArea = eb->getStartPosTileArea();
        sf::Vector2i sizeTileArea = eb->getSizeTileArea();
        for(unsigned row=startPosTileArea.y;row<startPosTileArea.y+sizeTileArea.y;row++){
            for(unsigned col=startPosTileArea.x;col<startPosTileArea.x+sizeTileArea.x;col++){
                if(row > 0 && row < m_listBuildingToBuild.size()){
                    if(col > 0 && col < m_listBuildingToBuild[row].size()){
                        if(m_listBuildingToBuild[row][col]) {
                            //the pole will not be here because we just removed it
                            findElectricalBuilding(m_listBuildingToBuild[row][col]);
                        }
                    }
                }
            }
        }
        ElectricalCircuit* ec = eb->getElectricalCircuit();
        //get all eb of the ec for compute related components
        std::vector<ElectricalBuilding*> allEBofEC = ec->getCopyListEB();
        //just remove the eb of it
        for (unsigned i = 0; i < allEBofEC.size(); ++i) {
            if(allEBofEC[i] == eb){
                allEBofEC.erase(allEBofEC.begin()+i);
                break;
            }
        }

        //remove connections
        ec->removeConnectionsContain(eb);

        //remove the eb of linkedEB
        std::vector<ElectricalBuilding*> EBLinked = eb->getCopyListLinkedEB();
        for (int i = 0; i < EBLinked.size(); ++i) {
            EBLinked[i]->removeLinkedEB(eb);
        }

        //compute of related components
        std::vector<std::vector<ElectricalBuilding*>> relatedComponent;
        while(allEBofEC.size() > 0){
            relatedComponent.push_back({});
            allEBofEC[0]->getRelatedComponent(relatedComponent[relatedComponent.size()-1],allEBofEC);
        }

        bool hasToSplit = (relatedComponent.size() > 1);
        /*cout << "relatedComponent size: " << relatedComponent.size() << endl;
        cout << "HAS TO SPLIT: " << boolToString(hasToSplit) << endl;

        cout << "verif: " << endl;
        for (unsigned i = 0; i < relatedComponent.size(); ++i) {
            for (unsigned j = 0; j < relatedComponent[i].size(); ++j) {
                cout << relatedComponent[i][j] << " ";
            }
            cout << endl;
        }*/
        bool deleteEC = false;
        if(hasToSplit){
            deleteEC = true;
            vector<ConnectionBetweenEB*> listConn = ec->getConnections();
            for (unsigned i = 0; i < relatedComponent.size(); ++i) {
                ElectricalCircuit *newEC = new ElectricalCircuit();
                //cout << "create new electrical circuit: " << newEC << endl;
                newEC->addElectricalBuildings(relatedComponent[i]);
                for (unsigned j = 0; j < relatedComponent[i].size(); ++j) {
                    for (unsigned k = 0; k < listConn.size(); ++k) {
                        if(listConn[k]->contains(relatedComponent[i][j])){
                            newEC->addConnection(listConn[k]);
                            listConn.erase(listConn.begin()+k);
                            k--;
                        }
                    }
                }

                m_listElectricalCircuit.push_back(newEC);
            }
        }else{
            ec->removeElectricalBuilding(eb);
            if(ec->getNbElectricalBuilding() == 0){
                deleteEC = true;
            }
        }
        if(deleteEC){
            //we can remove the EC
            if(m_electricalCircuitSelected == ec){
                m_electricalCircuitSelected = nullptr;
            }
            for (unsigned i = 0; i < m_listElectricalCircuit.size(); ++i) {
                if(m_listElectricalCircuit[i] == ec){
                    m_listElectricalCircuit.erase(m_listElectricalCircuit.begin()+i);
                    delete ec;
                }
            }
        }
        sf::Time endTime = m_clock.getElapsedTime();
        sf::Time fullTime = endTime-startTime;
        cout << "TODO: reduce time to compute delete EB of map: " << fullTime.asMicroseconds() << " microSec" << endl;
    }
}

void Map::findIOProcessBuilding(ProcessBuilding *pb,const sf::Vector2i &posTileBuilding){
    //WE HAVE JUST PUT pb
    vector<ProcessBuilding*> toAddtoPb;
    vector<ProcessBuilding*> toAddPb;

    sf::Vector2i sizePB = pb->getBuildingType()->getTileSizeBox()[pb->getNumTexture()];

    OrientedProcessBuilding *opb = dynamic_cast<OrientedProcessBuilding*>(pb);
    ProcessBuilding *otherPB;
    OrientedProcessBuilding *otherOPB;
    if(opb){
        Direction from, to;
        opb->getDirection(from, to);

        //test to add Outuput to the opb
        if(to == NORD){
            if(posTileBuilding.y-1>=0){
                for(unsigned int i=0;i<sizePB.x;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y-1][posTileBuilding.x+i];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(from == SOUTH){
                                addUniqueInVector(toAddtoPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddtoPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }else if(to == WEST){
            //WEST
            if(posTileBuilding.x-1>=0){
                for(unsigned int i=0;i<sizePB.y;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+i][posTileBuilding.x-1];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(from == EAST){
                                addUniqueInVector(toAddtoPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddtoPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }else if(to == SOUTH){
            //SOUTH
            if(posTileBuilding.y+sizePB.y<m_listProcessBuildingToBuild.size()){
                for(unsigned int i=0;i<sizePB.x;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+sizePB.y][posTileBuilding.x+i];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(from == NORD){
                                addUniqueInVector(toAddtoPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddtoPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }else if(to == EAST){
            //EAST
            if(posTileBuilding.x+sizePB.x<m_listProcessBuildingToBuild[posTileBuilding.y].size()){
                for(unsigned int i=0;i<sizePB.y;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+i][posTileBuilding.x+sizePB.x];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(from == WEST){
                                addUniqueInVector(toAddtoPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddtoPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }

        //test to update output to the near pb
        if(from == NORD){
            if(posTileBuilding.y-1>=0){
                for(unsigned int i=0;i<sizePB.x;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y-1][posTileBuilding.x+i];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(to == SOUTH){
                                addUniqueInVector(toAddPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }else if(from == WEST){
            if(posTileBuilding.x-1>=0){
                for(unsigned int i=0;i<sizePB.y;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+i][posTileBuilding.x-1];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(to == EAST){
                                addUniqueInVector(toAddPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }else if(from == SOUTH){
            if(posTileBuilding.y+sizePB.y<m_listProcessBuildingToBuild.size()){
                for(unsigned int i=0;i<sizePB.x;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+sizePB.y][posTileBuilding.x+i];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(to == NORD){
                                addUniqueInVector(toAddPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }else if(from == EAST){
            if(posTileBuilding.x+sizePB.x<m_listProcessBuildingToBuild[posTileBuilding.y].size()){
                for(unsigned int i=0;i<sizePB.y;i++){
                    otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+i][posTileBuilding.x+sizePB.x];
                    if(otherPB){
                        //cout << "there is a ProcessBuilding in this tile" << endl;
                        otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                        if(otherOPB){
                            //the other has an orientation -> check orientation
                            Direction from, to;
                            otherOPB->getDirection(from, to);
                            if(to == WEST){
                                addUniqueInVector(toAddPb, otherOPB);
                            }
                        }else{
                            //the other hasnt an orientation -> OK
                            addUniqueInVector(toAddPb, otherPB);
                        }
                    }//else there is nothing in this tile
                }
            }
        }
    }else{ //the pb jsut placed is not oriented
        //NORTH SIZE
        if(posTileBuilding.y-1>=0){
            for(unsigned int i=0;i<sizePB.x;i++){
                otherPB = m_listProcessBuildingToBuild[posTileBuilding.y-1][posTileBuilding.x+i];
                if(otherPB){
                    //cout << "there is a ProcessBuilding in this tile" << endl;
                    otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                    if(otherOPB){
                        //the other has an orientation -> check orientation
                        Direction from, to;
                        otherOPB->getDirection(from, to);
                        if(otherOPB->hasInput() && from == SOUTH){
                            addUniqueInVector(toAddtoPb, otherOPB);
                        }
                        if(otherOPB->hasOutput() && to == SOUTH){
                            addUniqueInVector(toAddPb, otherOPB);
                        }
                    }else{
                        //the other hasnt an orientation -> OK
                        addUniqueInVector(toAddtoPb, otherPB);
                        addUniqueInVector(toAddPb, otherPB);
                    }
                }//else there is nothing in this tile
            }
        }//else that is outside the tileMap
        //WEST
        if(posTileBuilding.x-1>=0){
            for(unsigned int i=0;i<sizePB.y;i++){
                otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+i][posTileBuilding.x-1];
                if(otherPB){
                    //cout << "there is a ProcessBuilding in this tile" << endl;
                    otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                    if(otherOPB){
                        //the other has an orientation -> check orientation
                        Direction from, to;
                        otherOPB->getDirection(from, to);
                        if(otherOPB->hasInput() && from == EAST){
                            addUniqueInVector(toAddtoPb, otherOPB);
                        }
                        if(otherOPB->hasOutput() && to == EAST){
                            addUniqueInVector(toAddPb, otherOPB);
                        }
                    }else{
                        //the other hasnt an orientation -> OK
                        addUniqueInVector(toAddtoPb, otherPB);
                        addUniqueInVector(toAddPb, otherPB);
                    }
                }//else there is nothing in this tile
            }
        }//else that is outside the tileMap
        //SOUTH
        if(posTileBuilding.y+sizePB.y<m_listProcessBuildingToBuild.size()){
            for(unsigned int i=0;i<sizePB.x;i++){
                otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+sizePB.y][posTileBuilding.x+i];
                if(otherPB){
                    //cout << "there is a ProcessBuilding in this tile" << endl;
                    otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                    if(otherOPB){
                        //the other has an orientation -> check orientation
                        Direction from, to;
                        otherOPB->getDirection(from, to);
                        if(otherOPB->hasInput() && from == NORD){
                            addUniqueInVector(toAddtoPb, otherOPB);
                        }
                        if(otherOPB->hasOutput() && to == NORD){
                            addUniqueInVector(toAddPb, otherOPB);
                        }
                    }else{
                        //the other hasnt an orientation -> OK
                        addUniqueInVector(toAddtoPb, otherPB);
                        addUniqueInVector(toAddPb, otherPB);
                    }
                }//else there is nothing in this tile
            }
        }//else that is outside the tileMap
        //EAST
        if(posTileBuilding.x+sizePB.x<m_listProcessBuildingToBuild[posTileBuilding.y].size()){
            for(unsigned int i=0;i<sizePB.y;i++){
                otherPB = m_listProcessBuildingToBuild[posTileBuilding.y+i][posTileBuilding.x+sizePB.x];
                if(otherPB){
                    //cout << "there is a ProcessBuilding in this tile" << endl;
                    otherOPB = dynamic_cast<OrientedProcessBuilding*>(otherPB);
                    if(otherOPB){
                        //the other has an orientation -> check orientation
                        Direction from, to;
                        otherOPB->getDirection(from, to);
                        if(otherOPB->hasInput() && from == WEST){
                            addUniqueInVector(toAddtoPb, otherOPB);
                        }
                        if(otherOPB->hasOutput() && to == WEST){
                            addUniqueInVector(toAddPb, otherOPB);
                        }
                    }else{
                        //the other hasnt an orientation -> OK
                        addUniqueInVector(toAddtoPb, otherPB);
                        addUniqueInVector(toAddPb, otherPB);
                    }
                }//else there is nothing in this tile
            }
        }//else that is outside the tileMap
    }

    for(unsigned i=0;i<toAddtoPb.size();i++){
        pb->addOutputProcessBuilding(toAddtoPb[i]);
    }

    for(unsigned i=0;i<toAddPb.size();i++){
        toAddPb[i]->addOutputProcessBuilding(pb);
    }
}

void Map::computeElectricalCircuit(ElectricalBuilding *eb){
    std::vector<ElectricalBuilding*> listEBInRange;
    std::vector<ElectricalCircuit*> listECInRange;
    for(unsigned i = 0; i < m_listElectricalBuilding.size(); ++i) {
        if(m_listElectricalBuilding[i] != eb && eb->isInRangeForCircuit(m_listElectricalBuilding[i])){
            listEBInRange.push_back(m_listElectricalBuilding[i]);
            addUniqueInVector(listECInRange, m_listElectricalBuilding[i]->getElectricalCircuit());
        }
    }
    for (unsigned i = 0; i < listEBInRange.size(); ++i) {
        eb->addLinkedElectricalBuilding(listEBInRange[i]);
        listEBInRange[i]->addLinkedElectricalBuilding(eb);
    }
    if(listECInRange.size() == 1) {
        //cout << "add a eb into a ec" << endl;
        listECInRange[0]->addElectricalBuilding(eb);
    }else if(listECInRange.size() > 1) {
        //cout << "merge " << listECInRange.size() << " ec" << endl;
        ElectricalCircuit *newEc = ElectricalCircuit::createByMergingWith(listECInRange);
        newEc->addElectricalBuilding(eb);
        m_listElectricalCircuit.push_back(newEc);
        removeFromListEC(listECInRange);
        //cout << "new size of m_listEC: " << m_listElectricalCircuit.size() << endl;
    }else{
        ElectricalCircuit *ec = new ElectricalCircuit;
        ec->addElectricalBuilding(eb);
        m_listElectricalCircuit.push_back(ec);
    }


    //set the electric building of all the building in the zone except the pole
    sf::Vector2i startPosTileArea = eb->getStartPosTileArea();
    sf::Vector2i sizeTileArea = eb->getSizeTileArea();
    sf::Vector2i posEB = eb->getPosTile();
    for(unsigned row=startPosTileArea.y;row<startPosTileArea.y+sizeTileArea.y;row++){
        for(unsigned col=startPosTileArea.x;col<startPosTileArea.x+sizeTileArea.x;col++){
            if(row > 0 && row < m_listBuildingToBuild.size()){
                if(col > 0 && col < m_listBuildingToBuild[row].size()){
                    if(m_listBuildingToBuild[row][col] && (row != posEB.y || col != posEB.x)) {
                        m_listBuildingToBuild[row][col]->setElectricalBuilding(eb);
                    }
                }
            }
        }
    }
}

void Map::findElectricalBuilding(Building *building){
    //building is not en electrical building
    if(m_listElectricalBuilding.size() == 0) {
        building->setElectricalBuilding(nullptr);
        return;
    }

    sf::Vector2i posTile = building->getPosTile();
    //cout << "posTile: " << posTile << endl;
    sf::Vector2i sizeBox = building->getBuildingType()->getTileSizeBox()[building->getNumTexture()];
    //cout << "sizeBox: " << sizeBox << endl;

    //cout << "m_listElectricalBuilding.size(): " << m_listElectricalBuilding.size() << endl;

    bool find = false;
    for(unsigned row=posTile.y;row<posTile.y+sizeBox.y;row++){
        if(find)break;
        for(unsigned col=posTile.x;col<posTile.x+sizeBox.x;col++){
            if(find)break;
            for (int i = m_listElectricalBuilding.size()-1; i >= 0; i--) {
                if(m_listElectricalBuilding[i]->isInArea(col, row)){
                    //cout << "SET " << building << " eb -> " << m_listElectricalBuilding[i] << endl;
                    building->setElectricalBuilding(m_listElectricalBuilding[i]);
                    find = true;
                    break;
                }
            }
        }
    }

    if(!find) {
        building->setElectricalBuilding(nullptr);
    }

}

void Map::addUniqueInVector(std::vector<ProcessBuilding*> &vec, ProcessBuilding* ptr){
    for(unsigned i=0;i<vec.size();i++){
        if(vec[i] == ptr)
            return;
    }
    vec.push_back(ptr);
}

void Map::addUniqueInVector(std::vector<ElectricalCircuit*> &vec, ElectricalCircuit* ec){
    for(unsigned i=0;i<vec.size();i++){
        if(vec[i] == ec)
            return;
    }
    vec.push_back(ec);
}

void Map::removeFromListEC(const std::vector<ElectricalCircuit*> &listECInRange){
    for (unsigned i = 0; i < listECInRange.size(); ++i) {
        for (unsigned j = 0; j < m_listElectricalCircuit.size(); ++j) {
            if(m_listElectricalCircuit[j] == listECInRange[i]){

                if(m_electricalCircuitSelected == m_listElectricalCircuit[j]){
                    m_electricalCircuitSelected = nullptr;
                }

                delete m_listElectricalCircuit[j];
                m_listElectricalCircuit.erase(m_listElectricalCircuit.begin()+j);
                break;
            }
        }
    }
}

/*void Map::createAndPlaceTransportSystem(unsigned numTransportType, const sf::Vector2i &posTile, Direction from, Direction to){
    TransportSystem *ts = nullptr;

    switch (numTransportType){
        case 1:
            ts = new Belt1(&m_clock, posTile, from, to);
            break;
        default:
            cout << "the numTransportType in Map::addTransportSystem is not managed" << endl;
            ts = nullptr;
            return;
    }

    //test valid
    if(!canBuildOnIt(posTile)){
        cout << "impossible de mettre un transportSystem ici" << endl;
        return;
    }

    //push back
    m_listEntity.push_back(ts);

    m_listTransportSystemToRender[posTile.y].push_back(ts);
    m_listTransportSystemToBuild[posTile.y][posTile.x] = ts;

    Direction newFrom = from;
    Direction newTo = to;
    adaptDirection(posTile, from, to, newFrom, newTo);
    ts->setDirection(newFrom,newTo);

    nearAdaptDirection(posTile, newFrom, newTo);
}

void Map::adaptDirection(const sf::Vector2i &posTile, const Direction currentFrom, const Direction currentTo, Direction &returnFrom, Direction &returnTo){
    Direction otherFrom;
    Direction otherTo;

    if(posTile.y+1 < m_listTransportSystemToBuild.size() && m_listTransportSystemToBuild[posTile.y+1][posTile.x]){
        m_listTransportSystemToBuild[posTile.y+1][posTile.x]->getDirection(otherFrom, otherTo);
        if(otherTo == NORD){
            if(currentFrom == WEST && currentTo == EAST){
                returnFrom = SOUTH;
                returnTo = EAST;
            }else if(currentFrom == EAST && currentTo == WEST){
                returnFrom = SOUTH;
                returnTo = WEST;
            }
        }
    }else if(posTile.y-1 >= 0 && m_listTransportSystemToBuild[posTile.y-1][posTile.x]){
        m_listTransportSystemToBuild[posTile.y-1][posTile.x]->getDirection(otherFrom, otherTo);
        if(otherTo == SOUTH){
            if(currentFrom == WEST && currentTo == EAST){
                returnFrom = NORD;
                returnTo = EAST;
            }else if(currentFrom == EAST && currentTo == WEST){
                returnFrom = NORD;
                returnTo = WEST;
            }
        }
    }else if(posTile.x-1 >= 0 && m_listTransportSystemToBuild[posTile.y][posTile.x-1]){
        m_listTransportSystemToBuild[posTile.y][posTile.x-1]->getDirection(otherFrom, otherTo);
        if(otherTo == EAST){
            if(currentFrom == SOUTH && currentTo == NORD){
                returnFrom = WEST;
                returnTo = NORD;
            }else if(currentFrom == NORD && currentTo == SOUTH){
                returnFrom = WEST;
                returnTo = SOUTH;
            }
        }
    }else if(posTile.x+1 < m_listTransportSystemToBuild[posTile.y].size() && m_listTransportSystemToBuild[posTile.y][posTile.x+1]){
        m_listTransportSystemToBuild[posTile.y][posTile.x+1]->getDirection(otherFrom, otherTo);
        if(otherTo == WEST){
            if(currentFrom == SOUTH && currentTo == NORD){
                returnFrom = EAST;
                returnTo = NORD;
            }else if(currentFrom == NORD && currentTo == SOUTH){
                returnFrom = EAST;
                returnTo = SOUTH;
            }
        }
    }
}

void Map::nearAdaptDirection(const sf::Vector2i &posTile, const Direction currentFrom, const Direction currentTo){
    Direction otherFrom;
    Direction otherTo;
    if(currentTo == NORD){
        if(posTile.y-1 >= 0 && m_listTransportSystemToBuild[posTile.y-1][posTile.x]){
            m_listTransportSystemToBuild[posTile.y-1][posTile.x]->getDirection(otherFrom, otherTo);
            if(otherFrom == WEST && otherTo == EAST){
                m_listTransportSystemToBuild[posTile.y-1][posTile.x]->setDirection(SOUTH,EAST);
            }else if(otherFrom == EAST && otherTo == WEST){
                m_listTransportSystemToBuild[posTile.y-1][posTile.x]->setDirection(SOUTH,WEST);
            }
        }
    }else if(currentTo == SOUTH){
        if(posTile.y+1 < m_listTransportSystemToBuild.size() && m_listTransportSystemToBuild[posTile.y+1][posTile.x]){
            m_listTransportSystemToBuild[posTile.y+1][posTile.x]->getDirection(otherFrom, otherTo);
            if(otherFrom == WEST && otherTo == EAST){
                m_listTransportSystemToBuild[posTile.y+1][posTile.x]->setDirection(NORD,EAST);
            }else if(otherFrom == EAST && otherTo == WEST){
                m_listTransportSystemToBuild[posTile.y+1][posTile.x]->setDirection(NORD,WEST);
            }
        }
    }else if(currentTo == EAST){
        if(posTile.x+1 < m_listTransportSystemToBuild[posTile.y].size() && m_listTransportSystemToBuild[posTile.y][posTile.x+1]){
            m_listTransportSystemToBuild[posTile.y][posTile.x+1]->getDirection(otherFrom, otherTo);
            if(otherFrom == SOUTH && otherTo == NORD){
                m_listTransportSystemToBuild[posTile.y][posTile.x+1]->setDirection(WEST,NORD);
            }else if(otherFrom == NORD && otherTo == SOUTH){
                m_listTransportSystemToBuild[posTile.y][posTile.x+1]->setDirection(WEST,NORD);
            }
        }
    }else if(currentTo == WEST){
        if(posTile.x-1 >= 0 && m_listTransportSystemToBuild[posTile.y][posTile.x-1]){
            m_listTransportSystemToBuild[posTile.y][posTile.x-1]->getDirection(otherFrom, otherTo);
            if(otherFrom == SOUTH && otherTo == NORD){
                m_listTransportSystemToBuild[posTile.y][posTile.x-1]->setDirection(EAST,NORD);
            }else if(otherFrom == NORD && otherTo == SOUTH){
                m_listTransportSystemToBuild[posTile.y][posTile.x-1]->setDirection(EAST,NORD);
            }
        }
    }
}*/

void Map::createRobot(const sf::Vector2f &cooMap, WorkInProgress *wip){
    Robot *r = new Robot(this, &m_clock, cooMap, wip);
    m_listEntity.push_back(r);
    m_listRobotToRender.push_back(r);

    wip->setRobot(r);
}

void Map::destroyRobot(Robot *robot){
    for(unsigned i=0;i<m_listEntity.size();i++){
        if(m_listEntity[i] == robot){
            m_listEntity.erase(m_listEntity.begin()+i);
            break;
        }
    }
    for(unsigned i=0;i<m_listRobotToRender.size();i++){
        if(m_listRobotToRender[i] == robot){
            m_listRobotToRender.erase(m_listRobotToRender.begin()+i);
            break;
        }
    }

    delete robot;
}

/*void Map::clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent){
    //updatePosVariables();
    if(m_buildingSelected){
        m_buildingSelected->clickHandler(mouseButtonEvent, m_cooMap, m_cooScreen);
    }
    if(mouseButtonEvent.button == Mouse::Button::Left){
        bool pressed = Mouse::isButtonPressed(Mouse::Button::Left);
        if(pressed){
            if(!m_buildingSelected) {
                //no building selected
                if (m_buildMode.isReady()) {
                    createAndPlaceWorkInProgressFromBuildMode();
                } else {
                    sf::Vector2i posTileBuilding = getCooTileBuildingByCooMap(m_cooMap);
                    if (m_listBuildingToBuild[posTileBuilding.y][posTileBuilding.x]) {
                        TransportSystem *ts = dynamic_cast<TransportSystem *>(m_listBuildingToBuild[posTileBuilding.y][posTileBuilding.x]);
                        if (ts) {
                            ts->addPhysicalItemStart(new PhysicalItem(this, new ItemTypeWithData(DataLoader::getItemType(2)), {0, 0}));
                        }
                    }
                }
            }
        }
    }else if(mouseButtonEvent.button == Mouse::Button::Right){
        bool pressed = Mouse::isButtonPressed(Mouse::Button::Right);
        if(pressed){
            sf::Vector2i posTile = getCooTileBuildingByCooMap(m_cooMap);
            selectBuildingOrTransport(posTile);
        }
    }
}*/

void Map::touchBeganEventHandler(const sf::Event::TouchEvent &touchEvent){
    unsigned int finger = touchEvent.finger;
    m_listFinger.push_back(finger);
    m_listCooScreenFinger.push_back(Vector2i(0,0));
    m_listCooMapFinger.push_back(Vector2f(0,0));

    updatePosVariables();
}

void Map::touchMovedEventHandler(const sf::Event::TouchEvent &touchEvent){
    unsigned int finger = touchEvent.finger;
    updatePosVariables();

    if(m_listFinger.size() == 1){ //moveView
        if(!m_movingView){
            m_movingView = true;
            m_saveCooMapFinger1StartMovingView = m_listCooMapFinger.at(0);
            m_saveViewStartMovingView = m_view;
        }else{
            moveViewWithFinger();
        }
    }else if(m_listFinger.size() == 2){ //zoomView
        if(!m_zoomingView){
            m_zoomingView = true;
            m_saveCooScreenFinger1StartMovingView = m_listCooScreenFinger.at(0);
            m_saveCooScreenFinger2StartMovingView = m_listCooScreenFinger.at(1);
            m_saveViewStartMovingView = m_view;
        }else{
            zoomViewWithFinger();
        }
    }
}

void Map::touchEndedEventHandler(const sf::Event::TouchEvent &touchEvent){
    unsigned int finger = touchEvent.finger;
    updatePosVariables();

    if(m_listFinger.size() == 0){ //end moveView (or end zoomView)

        if(!m_movingView){
            if(m_buildingSelected){
                //m_buildingSelected->clickHandler(mouseButtonEvent, m_cooMap, m_cooScreen);
            }

            //sf::Vector2i posTile = getCooTileBuildingByCooMap(m_cooMap);
            //selectBuildingOrTransport(posTile);
        }
        m_movingView = false;
        m_zoomingView = false;

    }else if(m_listFinger.size() == 1){ //end zoomView
        m_zoomingView = false;
    }
}

/*void Map::mouseMoveHandler(){
    //updatePosVariables();
    bool leftClick = Mouse::isButtonPressed(Mouse::Button::Left);
    bool rightClick = Mouse::isButtonPressed(Mouse::Button::Right);

    m_buildMode.updatePos(m_posTileBuilding);
    if(leftClick){

    }
    if(m_buildingSelected){
        m_buildingSelected->mouseMoveHandler(m_cooScreen, m_cooMap, leftClick, rightClick);
    }
}*/

/*void Map::wheelEventHandler(sf::Event::MouseWheelEvent &mouseWheelEvent){
    if(m_buildMode.isActive()){
        m_buildMode.wheelEventHandler(mouseWheelEvent);
    }else{
        zoomView(mouseWheelEvent.delta);
    }
}*/

void Map::keyPressedEventHandler(sf::Event::KeyEvent &keyEvent){
    //cout << keyEvent.code << endl;
    //buildMode
    m_buildMode.keyPressedEventHandler(keyEvent);
    //gameplay
    if (keyEvent.code == Keyboard::O){
        if(m_buildingSelected){
            ProcessBuilding *pb = dynamic_cast<ProcessBuilding*>(m_buildingSelected);
            if(pb){
                pb->setOutputProcessBuilding(0);
                //cout << "select" << endl;
            }
        }
    }
    else if (keyEvent.code == Keyboard::T){
        if(m_buildingSelected){
            ItemContainer *ic = dynamic_cast<ItemContainer*>(m_buildingSelected);
            if(ic){
                ic->addItem(new ItemStack(new ItemTypeWithData(DataLoader::getItemType(4),{new QualityDataItem(EXCELLENT)}),1));
            }
        }
    }
    else if (keyEvent.code == Keyboard::L){
        if(m_buildingSelected){
            ProcessBuilding *pb = dynamic_cast<ProcessBuilding*>(m_buildingSelected);
            if(pb){
                pb->deselectOutputProcessBuilding();
                //cout << "deselect" << endl;
            }
        }
    }
    else if (keyEvent.code == Keyboard::E){
        if(m_electricalCircuitSelected){
            m_electricalCircuitSelected = nullptr;
        }else {
            if (m_buildingSelected) {
                ElectricalBuilding *eb = dynamic_cast<ElectricalBuilding *>(m_buildingSelected);
                if (eb) {
                    ElectricalCircuit *ec = eb->getElectricalCircuit();
                    if (ec) {
                        m_electricalCircuitSelected = ec;
                    }
                }
            }
        }
    }
    else if (keyEvent.code == Keyboard::Delete){
        if(m_buildingSelected){
            deconstructionBuilding(m_buildingSelected);
        }
    }
}

void Map::keyReleasedEventHandler(sf::Event::KeyEvent &keyEvent){
    //cout << keyEvent.code << endl;
}

void Map::selectBuildingOrTransport(const sf::Vector2i &posTile){
    if(m_listBuildingToBuild[posTile.y][posTile.x]){
        sf::Vector2i newPosTile = posTile;
        while(newPosTile.y-1 >= 0 && m_listBuildingToBuild[newPosTile.y-1][newPosTile.x] && m_listBuildingToBuild[newPosTile.y-1][newPosTile.x] == m_listBuildingToBuild[posTile.y][posTile.x]){
            newPosTile.y --;
        }
        while(newPosTile.x-1 >= 0 && m_listBuildingToBuild[newPosTile.y][newPosTile.x-1] && m_listBuildingToBuild[newPosTile.y][newPosTile.x-1] == m_listBuildingToBuild[posTile.y][posTile.x]){
            newPosTile.x --;
        }
        Building *b = m_listBuildingToBuild[posTile.y][posTile.x];
        m_buildingSelected = b;
        sf::Vector2i tileSizeBox = b->getBuildingType()->getTileSizeBox()[b->getNumTexture()];
        m_listSpriteSelected[0].setPosition(newPosTile.x*s_sizeTileBuildings, newPosTile.y*s_sizeTileBuildings);
        m_listSpriteSelected[1].setPosition((newPosTile.x+tileSizeBox.x)*s_sizeTileBuildings, newPosTile.y*s_sizeTileBuildings);
        m_listSpriteSelected[2].setPosition((newPosTile.x+tileSizeBox.x)*s_sizeTileBuildings, (newPosTile.y+tileSizeBox.y)*s_sizeTileBuildings);
        m_listSpriteSelected[3].setPosition(newPosTile.x*s_sizeTileBuildings, (newPosTile.y+tileSizeBox.y)*s_sizeTileBuildings);
    }else{
        m_buildingSelected = nullptr;
    }
}

void Map::update(){
    m_actualTime = m_clock.getElapsedTime();
    m_elapsedTime = m_actualTime-m_previousTime;
    m_previousTime = m_actualTime;
    //m_lagTime += m_elapsedTime;
    //cout << m_lagTime.asMilliseconds() << endl;

    //moveView();

    Belt1::updateIndexSprite(m_elapsedTime);

    for(unsigned int i=0;i<m_listEntity.size();i++){
        m_listEntity[i]->update();
    }
}

void Map::updatePosVariables(){
    //on update les listes des fingers
    for (unsigned int i = 0; i < m_listFinger.size(); ++i) {
        unsigned int finger = m_listFinger[i];
        if(Touch::isDown(finger)){
            Vector2i cooScreen = Touch::getPosition(finger, *m_renderWindow);
            m_listCooScreenFinger[i] = cooScreen;
            Vector2f cooMap = m_renderWindow->mapPixelToCoords(cooScreen,m_view);
            m_listCooMapFinger[i] = cooMap;
        }else{
            m_listFinger.erase(m_listFinger.begin()+i);
            m_listCooScreenFinger.erase(m_listCooScreenFinger.begin()+i);
            m_listCooMapFinger.erase(m_listCooMapFinger.begin()+i);
            i--;
        }
    }
}

void Map::draw(sf::RenderWindow &window){
    window.setView(m_view);

    window.draw(*m_tileMap);

    for(unsigned int row=0;row<m_listBuildingToRender.size();row++){
        for(unsigned int i=0;i<m_listBuildingToRender[row].size();i++){
            m_listBuildingToRender[row][i]->draw(window);
        }
    }

    for(unsigned int i=0;i<m_listTransportSystemToRender.size();i++){
        m_listTransportSystemToRender[i]->drawItem(window);
    }

    for(unsigned int i=0;i<m_listRobotToRender.size();i++){
        m_listRobotToRender[i]->draw(window);
    }

    //ui selected
    if(m_buildingSelected){
        for(unsigned i=0;i<4;i++){
            window.draw(m_listSpriteSelected[i]);
        }
    }

    m_buildMode.draw(window);
}

void Map::drawWorldPosUI(sf::RenderWindow &window){
    window.setView(m_view);
    if(m_buildingSelected){
        m_buildingSelected->drawWorldPosUI(window);
    }

    if(m_buildMode.isActive()){
        for (unsigned i = 0; i < m_listElectricalCircuit.size(); ++i) {
            m_listElectricalCircuit[i]->drawWorldPosUI(window);
        }
    }else if(m_electricalCircuitSelected){
        m_electricalCircuitSelected->drawWorldPosUI(window);
    }
}

void Map::drawScreenPosUI(sf::RenderWindow &window){
    window.setView(window.getDefaultView());
    if(m_buildingSelected){
        m_buildingSelected->drawScreenPosUI(window);
    }

    if(m_electricalCircuitSelected){
        m_electricalCircuitSelected->drawScreenPosUI(window);
    }

    m_textCamera.setString("view: center x: "+to_string(m_view.getCenter().x)+" center y: "+to_string(m_view.getCenter().y)+" size: "+to_string(m_view.getSize().x)+" "+to_string(m_view.getSize().y));
    m_textCamera.setPosition(0,0);
    window.draw(m_textCamera);
    m_textCamera.setString("m_listFinger: size: "+to_string(m_listFinger.size()));
    m_textCamera.setPosition(0,40);
    window.draw(m_textCamera);
    for (int i = 0; i < m_listFinger.size(); ++i) {
        m_textCamera.setString("finger: num: "+to_string(m_listFinger[i])+" cooScreen: "+to_string(m_listCooScreenFinger[i].x)+" "+to_string(m_listCooScreenFinger[i].y)+" cooMap: "+to_string(m_listCooMapFinger[i].x)+" "+to_string(m_listCooMapFinger[i].y));
        m_textCamera.setPosition(0,60+i*20);
        window.draw(m_textCamera);
    }

}

