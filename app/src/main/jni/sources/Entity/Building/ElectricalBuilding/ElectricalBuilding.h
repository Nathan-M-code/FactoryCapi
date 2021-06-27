//
// Created by natha on 30/11/2020.
//

#ifndef FACTORYCAPICLION_ELECTRICALBUILDING_H
#define FACTORYCAPICLION_ELECTRICALBUILDING_H

#include "../Building.h"

class ElectricalCircuit;

class ElectricalBuilding: public Building {
public:
    ElectricalBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType);
    virtual ~ElectricalBuilding();

    void getRelatedComponent(std::vector<ElectricalBuilding*> &component, std::vector<ElectricalBuilding*>& allEBofEC);

    void setAeraElec(unsigned nbTileElecNORD,
                     unsigned nbTileElecSOUTH,
                     unsigned nbTileElecWEST,
                     unsigned nbTileElecEAST);

    bool isInArea(unsigned col, unsigned row);

    bool isInRangeForCircuit(ElectricalBuilding* eb);

    void addLinkedElectricalBuilding(ElectricalBuilding* electricalBuilding);
    void removeLinkedEB(ElectricalBuilding* electricalBuilding);
    void clearLinkedEB();

    void setElectricalCircuit(ElectricalCircuit* electricalCircuit);
    ElectricalCircuit* getElectricalCircuit() const;

    const std::vector<ElectricalBuilding*>& getListLinkedEB() const;
    std::vector<ElectricalBuilding*> getCopyListLinkedEB();

    sf::Vector2i getStartPosTileArea() const;
    sf::Vector2i getSizeTileArea() const;

    virtual void drawWorldPosUI(sf::RenderWindow &window) override;

private:
    ElectricalCircuit* m_electricalCircuit;

    std::vector<ElectricalBuilding*> m_listLinkedEB;

    int m_nbTileElecNORD;
    int m_nbTileElecSOUTH;
    int m_nbTileElecWEST;
    int m_nbTileElecEAST;

    sf::RectangleShape m_rectVisuArea;
};


#endif //FACTORYCAPICLION_ELECTRICALBUILDING_H
