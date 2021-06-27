//
// Created by natha on 29/11/2020.
//

#ifndef FACTORYCAPICLION_ELECTRICALCIRCUIT_H
#define FACTORYCAPICLION_ELECTRICALCIRCUIT_H

#include "../Entity/Building/ElectricalBuilding/ElectricalBuilding.h"
#include "Power.h"
#include "ConnectionBetweenEB.h"

class ElectricalCircuit {
public:
    ElectricalCircuit();
    ~ElectricalCircuit();

    unsigned getNbElectricalBuilding() const;
    std::vector<ElectricalBuilding*> getCopyListEB() const;
    std::vector<ConnectionBetweenEB*> getConnections() const;

    void addConnections(const std::vector<ConnectionBetweenEB*> &listConn);
    void addConnection(ConnectionBetweenEB* conn);
    void removeConnectionsContain(ElectricalBuilding* electricalBuilding);

    static ElectricalCircuit* createByMergingWith(std::vector<ElectricalCircuit*> &listEC);

    void addElectricalBuilding(ElectricalBuilding* electricalBuilding);
    void addElectricalBuildings(std::vector<ElectricalBuilding*> listElectricalBuilding);
    void removeElectricalBuilding(ElectricalBuilding* electricalBuilding);

    void addCapacityBuilding(int num);
    void addPowerProduced(const Power &p);
    void removePowerProduced(const Power &p);

    void updateStringInfo();

    void drawScreenPosUI(sf::RenderWindow &window);
    void drawWorldPosUI(sf::RenderWindow &window);
private:
    int m_capacityBuilding;
    Power m_powerProduced;
    Power m_powerDelivered;

    std::vector<ElectricalBuilding*> m_listElectricalBuilding;

    std::vector<ConnectionBetweenEB*> m_listConnection;

    sf::RectangleShape m_bgInfo;
    sf::Text m_textInfo;
};


#endif //FACTORYCAPICLION_ELECTRICALCIRCUIT_H
