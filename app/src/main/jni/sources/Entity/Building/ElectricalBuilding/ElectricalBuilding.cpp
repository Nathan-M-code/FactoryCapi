//
// Created by natha on 30/11/2020.
//

#include "ElectricalBuilding.h"
#include "../../../Map/Map.h"

using namespace std;
using namespace sf;

ElectricalBuilding::ElectricalBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType):
Building(map,clock,posTile,buildingType)
{
    m_nbTileElecNORD = 0;
    m_nbTileElecSOUTH = 0;
    m_nbTileElecWEST = 0;
    m_nbTileElecEAST = 0;

    setNumTexture(rand_a_b(0,m_buildingType->getNbTexture()));

    const void * address = static_cast<const void*>(this);
    std::stringstream ss;
    ss << address;
    std::string name = ss.str();
    m_textInfo.setString(m_textInfo.getString()+"\n"+name);
}
ElectricalBuilding::~ElectricalBuilding(){

}

void ElectricalBuilding::getRelatedComponent(std::vector<ElectricalBuilding*> &component, std::vector<ElectricalBuilding*>& allEBofEC){
    //cout << this << " ElectricalBuilding::getRelatedComponent" << endl;
    component.push_back(this);

    for (unsigned i = 0; i < allEBofEC.size(); ++i) {
        if(allEBofEC[i] == this){
            allEBofEC.erase(allEBofEC.begin()+i);
            break;
        }
    }

    for (unsigned i = 0; i < m_listLinkedEB.size(); ++i) {
        bool visited = false;
        for (int j = 0; j < component.size(); ++j) {
            if(m_listLinkedEB[i] == component[j]){
                visited = true;
                break;
            }
        }
        if(!visited){
            m_listLinkedEB[i]->getRelatedComponent(component, allEBofEC);
        }
    }

}

void ElectricalBuilding::setAeraElec(unsigned nbTileElecNORD,
                                     unsigned nbTileElecSOUTH,
                                     unsigned nbTileElecWEST,
                                     unsigned nbTileElecEAST)
{
    //cout << "m_quality: " << m_quality << endl;
    int addScope = (int)m_quality-1;
    //cout << "addScope: " << addScope << endl;
    m_nbTileElecNORD = max(0u,nbTileElecNORD+addScope);
    m_nbTileElecSOUTH = max(0u,nbTileElecSOUTH+addScope);
    m_nbTileElecWEST = max(0u,nbTileElecWEST+addScope);
    m_nbTileElecEAST = max(0u,nbTileElecEAST+addScope);

    float sizeTileBuild = Map::s_sizeTileBuildings;
    sf::Vector2i sizeTileBox = m_buildingType->getTileSizeBox()[m_numTexture];

    m_rectVisuArea.setFillColor(sf::Color(18,199,223,100));
    m_rectVisuArea.setSize({m_nbTileElecWEST*sizeTileBuild+m_nbTileElecEAST*sizeTileBuild + sizeTileBox.x*sizeTileBuild,
                            m_nbTileElecNORD*sizeTileBuild+m_nbTileElecSOUTH*sizeTileBuild + sizeTileBox.y*sizeTileBuild});
    m_rectVisuArea.setPosition((m_posTile.x-m_nbTileElecWEST)*sizeTileBuild, (m_posTile.y-m_nbTileElecNORD)*sizeTileBuild);

    string strNbQuality = " (including +"+to_string(addScope)+")";
    addStringTextinfo("\nnbTileElecNORD : "+to_string(m_nbTileElecNORD)+strNbQuality);
    addStringTextinfo("\nnbTileElecSOUTH : "+to_string(m_nbTileElecSOUTH)+strNbQuality);
    addStringTextinfo("\nnbTileElecWEST : "+to_string(m_nbTileElecWEST)+strNbQuality);
    addStringTextinfo("\nnbTileElecEAST : "+to_string(m_nbTileElecEAST)+strNbQuality);
}

bool ElectricalBuilding::isInArea(unsigned col, unsigned row){
    sf::Vector2i startArea = getStartPosTileArea();
    sf::Vector2i sizeArea = getSizeTileArea();
    if(col >= startArea.x && col < startArea.x+sizeArea.x){
        if(row >= startArea.y && row < startArea.y+sizeArea.y){
            return true;
        }
    }
    return false;
}

bool ElectricalBuilding::isInRangeForCircuit(ElectricalBuilding* eb){
    /*cout << abs(m_posTile.x-eb->m_posTile.x)-1 << " " << m_nbTileElecEAST+eb->m_nbTileElecWEST << endl;
    cout << abs(m_posTile.y-eb->m_posTile.y)-1 << " " << m_nbTileElecNORD+eb->m_nbTileElecSOUTH << endl;
    cout << ((abs(m_posTile.x-eb->m_posTile.x)-1) <= int(m_nbTileElecEAST+eb->m_nbTileElecWEST)) << endl;
    cout << ((abs(m_posTile.y-eb->m_posTile.y)-1) <= int(m_nbTileElecNORD+eb->m_nbTileElecSOUTH)) << endl;*/
    if((abs(m_posTile.x-eb->m_posTile.x)-1) <= (m_nbTileElecEAST+eb->m_nbTileElecWEST)){
        if((abs(m_posTile.y-eb->m_posTile.y)-1) <= (m_nbTileElecNORD+eb->m_nbTileElecSOUTH)){
            return true;
        }
    }
    return false;
}

void ElectricalBuilding::addLinkedElectricalBuilding(ElectricalBuilding* electricalBuilding){
    m_listLinkedEB.push_back(electricalBuilding);
}

void ElectricalBuilding::removeLinkedEB(ElectricalBuilding* electricalBuilding){
    for (int i = 0; i < m_listLinkedEB.size(); ++i) {
        if(m_listLinkedEB[i] == electricalBuilding){
            //cout << this << " ElectricalBuilding::removeLinkedEB: " << electricalBuilding << endl;
            m_listLinkedEB.erase(m_listLinkedEB.begin()+i);
            break;
        }
    }
}

void ElectricalBuilding::clearLinkedEB(){
    m_listLinkedEB.clear();
}



void ElectricalBuilding::setElectricalCircuit(ElectricalCircuit* electricalCircuit){
    m_electricalCircuit = electricalCircuit;
}
ElectricalCircuit* ElectricalBuilding::getElectricalCircuit() const{
    return m_electricalCircuit;
}

const std::vector<ElectricalBuilding*>& ElectricalBuilding::getListLinkedEB() const{
    return m_listLinkedEB;
}

std::vector<ElectricalBuilding*> ElectricalBuilding::getCopyListLinkedEB(){
    return m_listLinkedEB;
}

sf::Vector2i ElectricalBuilding::getStartPosTileArea() const{
    return {(m_posTile.x-m_nbTileElecWEST), (m_posTile.y-m_nbTileElecNORD)};
}
sf::Vector2i ElectricalBuilding::getSizeTileArea() const{
    sf::Vector2i sizeTileBox = m_buildingType->getTileSizeBox()[m_numTexture];
    return {m_nbTileElecWEST+m_nbTileElecEAST+sizeTileBox.x, m_nbTileElecNORD+m_nbTileElecSOUTH+sizeTileBox.y};
}

void ElectricalBuilding::drawWorldPosUI(sf::RenderWindow &window) {
    window.draw(m_rectVisuArea);
}