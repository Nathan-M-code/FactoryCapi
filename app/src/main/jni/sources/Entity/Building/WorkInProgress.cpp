#include "WorkInProgress.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

WorkInProgress::WorkInProgress(Map *map, sf::Clock *clock, WIPStatus wipStatus, Building *buildingToCopy):
Building(map, clock, buildingToCopy)
{
    allInit(clock,wipStatus,buildingToCopy->getPosTile(),buildingToCopy->getBuildingType());

    if(buildingToCopy->getBuildingType()->hasQuality()){
        m_quality = buildingToCopy->getQuality();
    }

    OrientedBuilding *orientedBuilding = dynamic_cast<OrientedBuilding *>(buildingToCopy);
    if(orientedBuilding){
        Direction from, to;
        orientedBuilding->getDirection(from, to);
        m_from = from;
        m_to = to;
    }
}

void WorkInProgress::allInit(sf::Clock *clock, WIPStatus wipStatus, sf::Vector2i posTile, BuildingType* buildingType){
    m_wipStatus = wipStatus;

    m_posTile = posTile;

    m_spriteConstructionUI.setColor(sf::Color(255,255,255,128));
    m_spriteConstructionUI.setPosition(posTile.x*Map::s_sizeTileBuildings, posTile.y*Map::s_sizeTileBuildings);
    m_spriteConstructionUI.setTexture(*DataLoader::getTextureWipUI(wipStatus));
    m_spriteConstructionUI.setScale((buildingType->getTileSizeBox()[m_numTexture].x*Map::s_sizeTileBuildings)/m_spriteConstructionUI.getGlobalBounds().width,(buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings)/m_spriteConstructionUI.getGlobalBounds().height);

    sf::Vector2i box = buildingType->getTileSizeBox()[m_numTexture];
    for(unsigned row=0;row<box.y;row++){
        for(unsigned col=0;col<box.x;col++){
            m_listSpriteScaffolding.push_back(Sprite(*DataLoader::getTextureScaffolding()));
            m_listSpriteScaffolding[m_listSpriteScaffolding.size()-1].setPosition((posTile.x+col)*Map::s_sizeTileBuildings, (posTile.y+row)*Map::s_sizeTileBuildings);
            m_listSpriteScaffolding[m_listSpriteScaffolding.size()-1].setScale(Map::s_sizeTileBuildings/m_listSpriteScaffolding[m_listSpriteScaffolding.size()-1].getGlobalBounds().width,Map::s_sizeTileBuildings/m_listSpriteScaffolding[m_listSpriteScaffolding.size()-1].getGlobalBounds().height);
        }
    }

    m_timeLeft = buildingType->getConstructionTime();
    if (m_wipStatus == DECONSTRUCTION) m_timeLeft *= 2.f;
    m_finishTime = clock->getElapsedTime()+m_timeLeft;


    m_text.setFont(*DataLoader::getFont());
    m_text.setCharacterSize(32);
    m_text.setStyle(sf::Text::Bold);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setOutlineThickness(2);
    m_text.setString(timeToStr(m_timeLeft));
    m_midPos = {posTile.x*Map::s_sizeTileBuildings+(box.x*Map::s_sizeTileBuildings)/2.f, posTile.y*Map::s_sizeTileBuildings+(box.y*Map::s_sizeTileBuildings)/2.f};
    m_text.setPosition(m_midPos.x-m_text.getGlobalBounds().width/2.f, m_midPos.y-m_text.getGlobalBounds().height/2.f);
}

WorkInProgress::~WorkInProgress(){
    m_robot->finishWork();
}

void WorkInProgress::setPos(sf::Vector2i posTile){
    cout << "cannot setPos of a wip: WorkInProgress::setPos" << endl;
}

sf::Vector2f WorkInProgress::getSize() const{
    return {m_buildingType->getTileSizeBox()[m_numTexture].x*Map::s_sizeTileBuildings,m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings};
}

void WorkInProgress::update(){
    Building::update();

    m_timeLeft = m_finishTime-m_clock->getElapsedTime();
    m_text.setString(timeToStr(m_timeLeft));
    m_text.setPosition(m_midPos.x-m_text.getGlobalBounds().width/2.0, m_midPos.y-m_text.getGlobalBounds().height/2.0);

    if(m_timeLeft <= seconds(0)){
        //cout << "FINISH" << endl;
        m_map->finishWIP(m_posTile, this);
    }//else{
        //cout << m_timeLeft.asMilliseconds() << endl;
    //}
}

void WorkInProgress::draw(sf::RenderWindow &window){
    //TiledEntity::draw(window);

    window.draw(m_spriteConstructionUI);

    for(unsigned i=0;i<m_listSpriteScaffolding.size();i++){
        window.draw(m_listSpriteScaffolding[i]);
    }

    window.draw(m_text);
}

