//
// Created by natha on 07/12/2020.
//

#include "ConnectionBetweenEB.h"

using namespace std;
using namespace sf;

ConnectionBetweenEB::ConnectionBetweenEB(ElectricalBuilding *eb1, ElectricalBuilding *eb2) {
    m_eb1 = eb1;
    m_eb2 = eb2;
    
    float dir = atan2(eb1->getMidPos().y-eb2->getMidPos().y, eb1->getMidPos().x-eb2->getMidPos().x);
    m_line.setFillColor(sf::Color::White);
    m_line.setSize({sqrt((eb1->getMidPos().x-eb2->getMidPos().x)*(eb1->getMidPos().x-eb2->getMidPos().x)+(eb1->getMidPos().y-eb2->getMidPos().y)*(eb1->getMidPos().y-eb2->getMidPos().y)),4});
    m_line.setOrigin(m_line.getGlobalBounds().width/2.0, m_line.getGlobalBounds().height/2.0);
    m_line.setPosition((eb1->getMidPos().x+eb2->getMidPos().x)/2.0, (eb1->getMidPos().y+eb2->getMidPos().y)/2.0);
    m_line.setRotation(dir*180.0/PI);
}

ConnectionBetweenEB::~ConnectionBetweenEB() {

}

bool ConnectionBetweenEB::equalsTo(ConnectionBetweenEB *conn) const {
    if(m_eb1 == conn->m_eb1 && m_eb2 == conn->m_eb2){
        return true;
    }else if(m_eb1 == conn->m_eb2 && m_eb2 == conn->m_eb1){
        return true;
    }
    return false;
}

bool ConnectionBetweenEB::equalsTo(ElectricalBuilding* eb1,ElectricalBuilding* eb2) const{
    if(m_eb1 == eb1 && m_eb2 == eb2){
        return true;
    }else if(m_eb1 == eb2 && m_eb2 == eb1){
        return true;
    }
    return false;
}

bool ConnectionBetweenEB::contains(ElectricalBuilding* eb) const{
    if(eb == m_eb1 ||eb == m_eb2){
        return true;
    }
    return false;
}

void ConnectionBetweenEB::drawWorldPosUI(sf::RenderWindow &window) {
    window.draw(m_line);
}