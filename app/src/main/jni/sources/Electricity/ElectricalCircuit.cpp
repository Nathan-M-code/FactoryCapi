//
// Created by natha on 29/11/2020.
//

#include "ElectricalCircuit.h"

using namespace std;
using namespace sf;

ElectricalCircuit::ElectricalCircuit(){
    initRectStyle(m_bgInfo);
    initTextStyle(m_textInfo, DataLoader::getFont(), 12);

    m_bgInfo.setSize({500,200});
    m_bgInfo.setPosition({1500,200});

    m_capacityBuilding = 0;
    m_powerProduced = Power();
    m_powerDelivered = Power();

    updateStringInfo();
    m_textInfo.setPosition(m_bgInfo.getPosition()+Vector2f{50,50});
}
ElectricalCircuit::~ElectricalCircuit(){
}

unsigned ElectricalCircuit::getNbElectricalBuilding() const{
    return m_listElectricalBuilding.size();
}

std::vector<ElectricalBuilding*> ElectricalCircuit::getCopyListEB() const{
    return m_listElectricalBuilding;
}

std::vector<ConnectionBetweenEB*> ElectricalCircuit::getConnections() const{
    return m_listConnection;
}

void ElectricalCircuit::addConnections(const std::vector<ConnectionBetweenEB*> &listConn){
    for (unsigned j = 0; j < listConn.size(); ++j) {
        m_listConnection.push_back(listConn[j]);
    }
    updateStringInfo();
}

void ElectricalCircuit::addConnection(ConnectionBetweenEB* conn){
    m_listConnection.push_back(conn);
    updateStringInfo();
}

void ElectricalCircuit::removeConnectionsContain(ElectricalBuilding* electricalBuilding){
    for (unsigned j = 0; j < m_listConnection.size(); ++j) {
        if(m_listConnection[j]->contains(electricalBuilding)){
            delete m_listConnection[j];
            m_listConnection.erase(m_listConnection.begin()+j);
            j--;
        }
    }
}


ElectricalCircuit* ElectricalCircuit::createByMergingWith(std::vector<ElectricalCircuit*> &listEC){
    ElectricalCircuit* newEc = new ElectricalCircuit();
    for (unsigned i = 0; i < listEC.size(); ++i) {
        newEc->addElectricalBuildings(listEC[i]->m_listElectricalBuilding);
        newEc->addConnections(listEC[i]->getConnections());
        for (unsigned j = 0; j < listEC[i]->m_listElectricalBuilding.size(); ++j) {
            listEC[i]->m_listElectricalBuilding[j]->setElectricalCircuit(newEc);
        }
    }
    return newEc;
}

/*
 * handle creation of connections
 * Called by adding a pole to the circuit
 */
void ElectricalCircuit::addElectricalBuilding(ElectricalBuilding* electricalBuilding){
    //cout << this << " ElectricalCircuit::addElectricalBuilding " << electricalBuilding << endl;
    electricalBuilding->setElectricalCircuit(this);

    m_listElectricalBuilding.push_back(electricalBuilding);

    vector<ElectricalBuilding*> listLinked = electricalBuilding->getListLinkedEB();
    for (unsigned i = 0; i < listLinked.size(); ++i) {
        m_listConnection.push_back(new ConnectionBetweenEB(electricalBuilding, listLinked[i]));
    }

    updateStringInfo();
}

/*
 * !!!!!!!!! Do not handle creation of connections !!!!!!!!!!!!!
 * Called by merging circuits
 */
void ElectricalCircuit::addElectricalBuildings(std::vector<ElectricalBuilding*> listElectricalBuilding){
    for(unsigned i=0;i<listElectricalBuilding.size();i++){
        listElectricalBuilding[i]->setElectricalCircuit(this);
        m_listElectricalBuilding.push_back(listElectricalBuilding[i]);
    }
    updateStringInfo();
}

void ElectricalCircuit::removeElectricalBuilding(ElectricalBuilding* electricalBuilding){
    for(unsigned i=0;i<m_listElectricalBuilding.size();i++){
        if(m_listElectricalBuilding[i] == electricalBuilding){
            m_listElectricalBuilding.erase(m_listElectricalBuilding.begin()+i);
        }
    }
    updateStringInfo();
}

void ElectricalCircuit::addCapacityBuilding(int num){
    m_capacityBuilding += num;
}
void ElectricalCircuit::addPowerProduced(const Power &p){
    cout << this << " ElectricalCircuit::addPowerProduced " << p.asStrAutoUnit() << endl;
    cout << this << " avant add m_powerProduced: " << m_powerProduced.asStrAutoUnit() << endl;
    m_powerProduced += p;
    cout << this << " apres add m_powerProduced: " << m_powerProduced.asStrAutoUnit() << endl;
    updateStringInfo();
}
void ElectricalCircuit::removePowerProduced(const Power &p){
    cout << this << " ElectricalCircuit::removePowerProduced " << p.asStrAutoUnit() << endl;
    cout << this << " avant remove m_powerProduced: " << m_powerProduced.asStrAutoUnit() << endl;
    m_powerProduced -= p;
    cout << this << " apres remove m_powerProduced: " << m_powerProduced.asStrAutoUnit() << endl;
    updateStringInfo();
}


void ElectricalCircuit::updateStringInfo(){
    const void * address = static_cast<const void*>(this);
    std::stringstream ss;
    ss << address;
    std::string name = ss.str();
    m_textInfo.setString(name+"\n"
                            +to_string(m_listElectricalBuilding.size())+" poles\n"
                            +to_string(m_listConnection.size())+" connections\n"
                            +"Number of building available for production: "+to_string(m_capacityBuilding)+"\n"
                            +"Power produced: "+m_powerProduced.asStrAutoUnit()+"\n"
                            +"Power delivered: "+m_powerDelivered.asStrAutoUnit()+"\n"
                            +"Power left: "+(m_powerProduced-m_powerDelivered).asStrAutoUnit()+"\n");
}

void ElectricalCircuit::drawScreenPosUI(sf::RenderWindow &window){
    window.draw(m_bgInfo);
    window.draw(m_textInfo);
}

void ElectricalCircuit::drawWorldPosUI(sf::RenderWindow &window){
    for (int i = 0; i < m_listElectricalBuilding.size(); ++i) {
        m_listElectricalBuilding[i]->drawWorldPosUI(window);
    }

    for (unsigned i = 0; i < m_listConnection.size(); ++i) {
        m_listConnection[i]->drawWorldPosUI(window);
    }
}

