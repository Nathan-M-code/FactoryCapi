#include "WorldMap.h"

using namespace std;
using namespace sf;

WorldMap::WorldMap(sf::RenderWindow* renderWindow){
    m_renderWindow = renderWindow;

    Vector2i screenSize = OptionManager::getScreenSize();
    m_view = View(FloatRect(0,0,screenSize.x,screenSize.y));
    m_speedView = OptionManager::getSpeedCam();

    initTextStyle(m_textInfo, DataLoader::getFont(), 24);
    m_textInfo.setPosition(0,0);

    m_sizeChunk = {100,100};
    m_sizeChunkLoaded = {9, 9};
    m_sizeChunkFree = {3, 3};

    for (int col = 0; col < m_sizeChunkLoaded.x; ++col) {
        m_doubleArrayChunk.push_back({});
        for (int row = 0; row < m_sizeChunkLoaded.y; ++row) {
            m_doubleArrayChunk[col].push_back(nullptr);
        }
    }

    m_spriteSelected.setTexture(*DataLoader::getTextureTileWorldMapSelected());
    m_spriteSelected.setScale(WorldMapChunk::getSizeTile()/m_spriteSelected.getLocalBounds().width, WorldMapChunk::getSizeTile()/m_spriteSelected.getLocalBounds().height);
    m_factorySelected = nullptr;

    initRectStyle(m_backgroundInfoMapTile);
    initTextStyle(m_textInfoMapTile, DataLoader::getFont(), 25);

    //updateChunks();

    setCooChunkCenter({4,4 }, true);
    m_view.setCenter(4*WorldMapChunk::getLittleSize().x+200, 4*WorldMapChunk::getLittleSize().y+200);

}
WorldMap::~WorldMap() {

}

void WorldMap::selectedTile(sf::Vector2i cooChunk, sf::Vector2i posTileRelative, const sf::Vector2i& posTileAbsolute){
    if(cooChunk.x >= 0 && cooChunk.x<m_doubleArrayChunk.size()){
        if(cooChunk.y >= 0 && cooChunk.y<m_doubleArrayChunk[cooChunk.x].size()) {
            Vector2f pos = m_doubleArrayChunk[cooChunk.x][cooChunk.y]->getPosWorldByPosTileRelative(posTileRelative);
            m_spriteSelected.setPosition(pos);
            m_factorySelected = m_doubleArrayChunk[cooChunk.x][cooChunk.y]->getFactory(posTileRelative);
            m_mapTileSelected = m_doubleArrayChunk[cooChunk.x][cooChunk.y]->getMapTile(posTileRelative);

            m_textInfoMapTile.setString(WorldMapChunk::getStrInfo(m_mapTileSelected));
            m_backgroundInfoMapTile.setSize({max(250.f,m_textInfoMapTile.getGlobalBounds().width+20),m_textInfoMapTile.getGlobalBounds().height+50});
            m_backgroundInfoMapTile.setPosition(OptionManager::getScreenSize().x-m_backgroundInfoMapTile.getSize().x-10,50);
            m_textInfoMapTile.setPosition(m_backgroundInfoMapTile.getPosition().x+5, m_backgroundInfoMapTile.getPosition().y+25);
        }
    }
}

void WorldMap::setCooChunkCenter(const sf::Vector2i &cooChunk, bool fullLoad){
    if(cooChunk.x < 0 || cooChunk.x >= m_sizeChunk.x){
        return;
    }
    if(cooChunk.y < 0 || cooChunk.y >= m_sizeChunk.y){
        return;
    }

    sf::Vector2i deltaCoo = {cooChunk.x - m_cooChunkCenter.x,cooChunk.y - m_cooChunkCenter.y};

    if(deltaCoo.x > 0){
        //cout << m_cooChunkCenter.x+m_sizeChunkLoaded.x/2+deltaCoo.x << endl;
        if(m_cooChunkCenter.x+m_sizeChunkLoaded.x/2+deltaCoo.x >= m_sizeChunk.x){
            deltaCoo.x = m_sizeChunk.x-(m_cooChunkCenter.x+m_sizeChunkLoaded.x/2);
        }
    }else if(deltaCoo.x < 0){
        //cout << m_cooChunkCenter.x-m_sizeChunkLoaded.x/2+deltaCoo.x << endl;
        if(m_cooChunkCenter.x-m_sizeChunkLoaded.x/2+deltaCoo.x < 0){
            deltaCoo.x = -(m_cooChunkCenter.x-m_sizeChunkLoaded.x/2);
        }
    }
    if(deltaCoo.y > 0){
        //cout << m_cooChunkCenter.y+m_sizeChunkLoaded.y/2+deltaCoo.y << endl;
        if(m_cooChunkCenter.y+m_sizeChunkLoaded.y/2+deltaCoo.y >= m_sizeChunk.y){
            deltaCoo.y = m_sizeChunk.y-(m_cooChunkCenter.y+m_sizeChunkLoaded.y/2);
        }
    }else if(deltaCoo.y < 0){
        //cout << m_cooChunkCenter.y-m_sizeChunkLoaded.y/2+deltaCoo.y << endl;
        if(m_cooChunkCenter.y-m_sizeChunkLoaded.y/2+deltaCoo.y < 0){
            deltaCoo.y = -(m_cooChunkCenter.y-m_sizeChunkLoaded.y/2);
        }
    }

    //cout << "coo chunk param: " << cooChunk.x << " " << cooChunk.y << endl;
    //cout << "actual coo center: " << m_cooChunkCenter << endl;
    //cout << "CHANGE COO CENTER deltaCoo: " << deltaCoo << endl;

    if(fullLoad){
        m_cooChunkCenter += deltaCoo;
        //cout << "m_cooChunkCenter: " << m_cooChunkCenter << endl;
        //cout << "load all" << endl;
        //remove all
        m_listChunk.clear();

        for (int col = 0; col < m_sizeChunkLoaded.x; ++col) {
            for (int row = 0; row < m_sizeChunkLoaded.y; ++row) {
                if(m_doubleArrayChunk[col][row]) {
                    delete m_doubleArrayChunk[col][row];
                    m_doubleArrayChunk[col][row] = nullptr;
                }
            }
        }

        for (int col = 0; col < m_sizeChunkLoaded.x; ++col) {
            for (int row = 0; row < m_sizeChunkLoaded.y; ++row) {
                auto chunk = loadChunk({m_cooChunkCenter.x-m_sizeChunkLoaded.x/2+col,m_cooChunkCenter.y-m_sizeChunkLoaded.y/2+row});
                //cout << "chunk loaded: " << chunk << endl;
                //cout << "col: " << col << " row: " << row << endl;
                m_doubleArrayChunk[col][row] = chunk;
                if(chunk)
                    m_listChunk.push_back(chunk);
            }
        }
    }else if(deltaCoo.x != 0 && deltaCoo.y != 0){
        //cout << m_cooChunkCenter << endl;
        //cout << deltaCoo << endl;
        //cout << m_cooChunkCenter.x+deltaCoo.x << " " << m_cooChunkCenter.y << " | " << m_cooChunkCenter.x+deltaCoo.x << " " << m_cooChunkCenter.y+deltaCoo.y << endl;
        setCooChunkCenter({m_cooChunkCenter.x+deltaCoo.x, m_cooChunkCenter.y});
        setCooChunkCenter({m_cooChunkCenter.x, m_cooChunkCenter.y+deltaCoo.y});
    }else{
        m_cooChunkCenter += deltaCoo;
        //cout << "m_cooChunkCenter: " << m_cooChunkCenter << endl;
        if(deltaCoo.x > 0){
            for (int nb = 0; nb < deltaCoo.x; ++nb) {
                for (unsigned i = 0; i < m_sizeChunkLoaded.y; ++i) {
                    deleteFromlistChunk(m_doubleArrayChunk[0][i]);
                    delete m_doubleArrayChunk[0][i];
                }
                for (unsigned i = 0; i < m_sizeChunkLoaded.y; ++i) {
                    m_doubleArrayChunk[0].erase(m_doubleArrayChunk[0].begin());
                }
                m_doubleArrayChunk.erase(m_doubleArrayChunk.begin());

                m_doubleArrayChunk.push_back({});
                for (int i = 0; i < m_sizeChunkLoaded.y; ++i) {
                    auto chunk = loadChunk({m_cooChunkCenter.x+m_sizeChunkLoaded.x/2-(deltaCoo.x-1)+nb,m_cooChunkCenter.y-m_sizeChunkLoaded.y/2+i});
                    m_doubleArrayChunk[m_doubleArrayChunk.size()-1].push_back(chunk);
                    if(chunk)
                        m_listChunk.push_back(chunk);
                }
            }


        }else if(deltaCoo.x < 0){
            for (int nb = 0; nb < -deltaCoo.x; ++nb) {
                unsigned lastIndex = m_doubleArrayChunk.size()-1;
                for (unsigned i = 0; i < m_sizeChunkLoaded.y; ++i) {
                    deleteFromlistChunk(m_doubleArrayChunk[lastIndex][i]);
                    delete m_doubleArrayChunk[lastIndex][i];
                }
                for (unsigned i = 0; i < m_sizeChunkLoaded.y; ++i) {
                    m_doubleArrayChunk[lastIndex].erase(m_doubleArrayChunk[lastIndex].begin());
                }
                m_doubleArrayChunk.pop_back();

                std::vector<WorldMapChunk*> newCol;
                for (int i = 0; i < m_sizeChunkLoaded.y; ++i) {
                    auto chunk = loadChunk({m_cooChunkCenter.x-m_sizeChunkLoaded.x/2+(-deltaCoo.x-1)-nb,m_cooChunkCenter.y-m_sizeChunkLoaded.y/2+i});
                    newCol.push_back(chunk);
                    if(chunk)
                        m_listChunk.push_back(chunk);
                }
                m_doubleArrayChunk.insert(m_doubleArrayChunk.begin(),newCol);
            }
        }

        if(deltaCoo.y > 0){
            for (int nb = 0; nb < deltaCoo.y; ++nb) {
                for (int col = 0; col < m_sizeChunkLoaded.x; ++col) {
                    deleteFromlistChunk(m_doubleArrayChunk[col][0]);
                    delete m_doubleArrayChunk[col][0];
                    m_doubleArrayChunk[col].erase(m_doubleArrayChunk[col].begin());
                }

                for (int col = 0; col < m_sizeChunkLoaded.x; ++col) {
                    auto chunk = loadChunk({m_cooChunkCenter.x-m_sizeChunkLoaded.x/2+col,m_cooChunkCenter.y+m_sizeChunkLoaded.y/2-(deltaCoo.y-1)+nb});
                    m_doubleArrayChunk[col].push_back(chunk);
                    if(chunk)
                        m_listChunk.push_back(chunk);
                }
            }

        }else if(deltaCoo.y < 0){
            for (int nb = 0; nb < -deltaCoo.y; ++nb) {
                for (int col = 0; col < m_sizeChunkLoaded.x; ++col) {
                    deleteFromlistChunk(m_doubleArrayChunk[col][m_doubleArrayChunk[col].size()-1]);
                    delete m_doubleArrayChunk[col][m_doubleArrayChunk[col].size()-1];
                    m_doubleArrayChunk[col].pop_back();
                }

                for (int col = 0; col < m_sizeChunkLoaded.x; ++col) {
                    auto chunk = loadChunk({m_cooChunkCenter.x-m_sizeChunkLoaded.x/2+col,m_cooChunkCenter.y-m_sizeChunkLoaded.y/2+(-deltaCoo.y-1)-nb});
                    m_doubleArrayChunk[col].insert(m_doubleArrayChunk[col].begin(),chunk);
                    if(chunk)
                        m_listChunk.push_back(chunk);
                }
            }
        }

        /*cout << "nb listeChunk: " << m_listChunk.size() << endl;
        for(auto &val : m_listChunk){
            cout << val << endl;
        }
        cout << "verif doublArrayChunk:" << endl;
        for (auto &col : m_doubleArrayChunk) {
            for (auto &row : col) {
                cout << row << " ";
            }
            cout << endl;
        }*/

    }
}

WorldMapChunk* WorldMap::loadChunk(sf::Vector2i posChunk){
    //cout << "loading chunk " << posChunk << endl;
    WorldMapChunk* chunk = nullptr;
    if(posChunk.x >= 0 && posChunk.x < m_sizeChunk.x){
        if(posChunk.y >= 0 && posChunk.y < m_sizeChunk.y){
            chunk = new WorldMapChunk(posChunk);
        }
    }
    return chunk;
}

void WorldMap::updateChunks(){
    std::vector<sf::Vector2i> listCooChunk = getCooChunkByCooMap(m_view.getCenter());
    m_textInfo.setString(vectorOfVector2iToString(listCooChunk));
    if(listCooChunk[0] != m_saveCooChunk){
        m_saveCooChunk = listCooChunk[0];
        if(m_saveCooChunk.x > m_cooChunkCenter.x+m_sizeChunkFree.x/2 || m_saveCooChunk.x < m_cooChunkCenter.x-m_sizeChunkFree.x/2){
            setCooChunkCenter(m_saveCooChunk);
        }else if(m_saveCooChunk.y > m_cooChunkCenter.y+m_sizeChunkFree.y/2 || m_saveCooChunk.y < m_cooChunkCenter.y-m_sizeChunkFree.y/2){
            setCooChunkCenter(m_saveCooChunk);
        }
    }
}

void WorldMap::deleteFromlistChunk(WorldMapChunk *chunk){
    for (unsigned i = 0; i < m_listChunk.size(); ++i) {
        if(m_listChunk[i] == chunk){
            m_listChunk.erase(m_listChunk.begin()+i);
        }
    }
}

void WorldMap::updatePosVariables(){
    m_cooScreen = Mouse::getPosition(*m_renderWindow);
    m_cooMap = m_renderWindow->mapPixelToCoords(m_cooScreen,m_view);
}

std::vector<sf::Vector2i> WorldMap::getCooChunkByCooMap(const sf::Vector2f& cooMap) const{
    float h = WorldMapChunk::getH();
    float r = WorldMapChunk::getR();
    sf::Vector2f littleSize = WorldMapChunk::getLittleSize();
    sf::Vector2i actualCooChunk = {(int)(cooMap.x/littleSize.x),(int)(cooMap.y/littleSize.y)};

    std::vector<sf::Vector2i> ret;
    ret.push_back(actualCooChunk);

    if(((int)cooMap.x%(int)littleSize.x) <= r && ((int)cooMap.y%(int)littleSize.y) <= h){
        ret.push_back({actualCooChunk.x-1, actualCooChunk.y-1});
    }
    else if(((int)cooMap.x%(int)littleSize.x) <= r){
        ret.push_back({actualCooChunk.x-1, actualCooChunk.y});
    }
    else if(((int)cooMap.y%(int)littleSize.y) <= h){
        ret.push_back({actualCooChunk.x, actualCooChunk.y-1});
    }

    return ret;
}

//------------------------------------------------------------------------
void WorldMap::checkViewPos(){
    sf::Vector2f littleSize = WorldMapChunk::getLittleSize();

    if(m_view.getCenter().y<= m_view.getSize().y/2.0){
        m_view.setCenter(m_view.getCenter().x,m_view.getSize().y/2.0);
    }
    if(m_view.getCenter().x<= m_view.getSize().x/2.0){
        m_view.setCenter(m_view.getSize().x/2.0,m_view.getCenter().y);
    }
    if(m_sizeChunk.y*littleSize.y - m_view.getCenter().y <= m_view.getSize().y/2.0){
        m_view.setCenter(m_view.getCenter().x,m_sizeChunk.y*littleSize.y-m_view.getSize().y/2.0);
    }
    if(m_sizeChunk.x*littleSize.x - m_view.getCenter().x <= m_view.getSize().x/2.0){
        m_view.setCenter(m_sizeChunk.x*littleSize.x-m_view.getSize().x/2.0,m_view.getCenter().y);
    }
    m_view.setCenter(round(m_view.getCenter().x), round(m_view.getCenter().y));
}

void WorldMap::moveView(){
    bool z = Keyboard::isKeyPressed(Keyboard::Key::Z);
    bool q = Keyboard::isKeyPressed(Keyboard::Key::Q);
    bool s = Keyboard::isKeyPressed(Keyboard::Key::S);
    bool d = Keyboard::isKeyPressed(Keyboard::Key::D);
    if(z||q||s||d){
        if(z){
            m_view.move(0,-m_speedView);
        }
        if(q){
            m_view.move(-m_speedView,0);
        }
        if(s){
            m_view.move(0,m_speedView);
        }
        if(d){
            m_view.move(m_speedView,0);
        }
        checkViewPos();
        updatePosVariables();
        updateChunks();
    }
}

void WorldMap::zoomView(int delta){
    if(delta>0){ //zoom in delta>0
        m_view.zoom(0.7);
        if(m_view.getSize().x <= 500){
            m_view.setSize(500,281.25);
        }
    }else if(delta<0){ //zoom out delat<0
        m_view.zoom(1.3);
        /*if(m_view.getSize().x >= m_sizeChunkFree.x*WorldMapChunk::getLittleSize().x){
            m_view.setSize(m_sizeChunkFree.x*WorldMapChunk::getLittleSize().x,m_sizeChunkFree.x*WorldMapChunk::getLittleSize().x*9.0/16.0);
        }*/
    }
    checkViewPos();
    updatePosVariables();
    updateChunks();

    m_speedView = m_view.getSize().x/200;
}

void WorldMap::goToPos(const sf::Vector2f &worldPos){
    ///TODO
    //sf::Vector2i cooChunk = getCooChunkByCooMap(worldPos)[0];
}

//---------------------------------------------------------------------------------------

void WorldMap::clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent){
    updatePosVariables();
    if(mouseButtonEvent.button == Mouse::Button::Left){
        bool pressed = Mouse::isButtonPressed(Mouse::Button::Left);
        if(pressed){
            //cout << "m_cooMap: " << m_cooMap << endl;
            if(m_cooMap.x >= 0 && m_cooMap.y>=0){
                std::vector<sf::Vector2i> listCooChunk = getCooChunkByCooMap(m_cooMap);
                for (unsigned i = 0; i < listCooChunk.size(); ++i) {
                    Vector2i cooChunk = listCooChunk[i];
                    //cout << "test chunk: " << cooChunk << endl;

                    Vector2i posTileRelative;
                    Vector2i posTileAbsolute;

                    //std::vector<sf::Vector2i> listCooChunkViewCenter = getCooChunkByCooMap();
                    sf::Vector2i cooInArray = cooChunk - (m_cooChunkCenter - Vector2i{m_sizeChunkLoaded.x/2, m_sizeChunkLoaded.y/2});
                    //cout << "cooInArray: " << cooInArray << endl;

                    if(cooInArray.x >= 0 && cooInArray.x<m_doubleArrayChunk.size()){
                        if(cooInArray.y >= 0 && cooInArray.y<m_doubleArrayChunk[cooInArray.x].size()) {

                            if(m_doubleArrayChunk[cooInArray.x][cooInArray.y]) {
                                m_doubleArrayChunk[cooInArray.x][cooInArray.y]->getCooTileByPosWorld(m_cooMap,
                                                                                                 posTileRelative,
                                                                                                 posTileAbsolute);
                            }else{
                                continue;
                            }
                        }
                    }

                    //cout << "posTileRelative: " << posTileRelative << endl;
                    //cout << "posTileAbsolute: " << posTileAbsolute << endl;

                    Vector2i nbTile = WorldMapChunk::getNbTile();
                    if(posTileRelative.x >= 0 && posTileRelative.x < nbTile.x && posTileRelative.y >= 0 && posTileRelative.y < nbTile.y) {
                        selectedTile(cooInArray, posTileRelative, posTileAbsolute);
                        break;
                    }
                }
            }
        }
    }/*else if(mouseButtonEvent.button == Mouse::Button::Right){
        bool pressed = Mouse::isButtonPressed(Mouse::Button::Right);
        if(pressed){

        }
    }*/
}

void WorldMap::mouseMoveHandler(){
    updatePosVariables();
    bool leftClick = Mouse::isButtonPressed(Mouse::Button::Left);
    bool rightClick = Mouse::isButtonPressed(Mouse::Button::Right);

    /*if(leftClick){

    }*/
}

void WorldMap::wheelEventHandler(sf::Event::MouseWheelEvent &mouseWheelEvent){
    zoomView(mouseWheelEvent.delta);
}

void WorldMap::keyPressedEventHandler(sf::Event::KeyEvent &keyEvent){
    //cout << keyEvent.code << endl;
    //cout << "keyPressedEventHandler" << endl;
    /*if(keyEvent.code == sf::Keyboard::Right){
        setCooChunkCenter({m_cooChunkCenter.x+2,m_cooChunkCenter.y});
    }
    if(keyEvent.code == sf::Keyboard::Down){
        setCooChunkCenter({m_cooChunkCenter.x,m_cooChunkCenter.y+2});
    }
    if(keyEvent.code == sf::Keyboard::Left){
        setCooChunkCenter({m_cooChunkCenter.x-2,m_cooChunkCenter.y});
    }
    if(keyEvent.code == sf::Keyboard::Up){
        setCooChunkCenter({m_cooChunkCenter.x,m_cooChunkCenter.y-2});
    }*/
}

void WorldMap::keyReleasedEventHandler(sf::Event::KeyEvent &keyEvent){
    //cout << keyEvent.code << endl;
}

//---------------------------------------------------------------------------------

void WorldMap::update(){
    moveView();
}

void WorldMap::draw(sf::RenderWindow &window) {
    window.setView(m_view);
    for (unsigned i = 0; i < m_listChunk.size(); ++i) {
        m_listChunk[i]->draw(window);
    }

    for (unsigned i = 0; i < m_listChunk.size(); ++i) {
        m_listChunk[i]->drawWorldPosUI(window);
    }

    if(m_factorySelected || m_mapTileSelected){
        window.draw(m_spriteSelected);
    }

    window.setView(window.getDefaultView());
    window.draw(m_textInfo);


    if(m_factorySelected){
        m_factorySelected->drawUIScreenPos(window);
    }

    if(m_mapTileSelected){
        window.draw(m_backgroundInfoMapTile);
        window.draw(m_textInfoMapTile);
    }
}
