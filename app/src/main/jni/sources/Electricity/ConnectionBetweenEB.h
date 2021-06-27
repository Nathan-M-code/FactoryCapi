//
// Created by natha on 07/12/2020.
//

#ifndef CONNECTIONBETWEENEB_H
#define CONNECTIONBETWEENEB_H

#include "../Entity/Building/ElectricalBuilding/ElectricalBuilding.h"

class ConnectionBetweenEB {
public:
    ConnectionBetweenEB(ElectricalBuilding* eb1,ElectricalBuilding* eb2);
    ~ConnectionBetweenEB();

    bool equalsTo(ConnectionBetweenEB* conn) const;
    bool equalsTo(ElectricalBuilding* eb1,ElectricalBuilding* eb2) const;
    bool contains(ElectricalBuilding* eb) const;

    void drawWorldPosUI(sf::RenderWindow &window);
private:
    ElectricalBuilding* m_eb1;
    ElectricalBuilding* m_eb2;

    sf::RectangleShape m_line;
};


#endif //CONNECTIONBETWEENEB_H
