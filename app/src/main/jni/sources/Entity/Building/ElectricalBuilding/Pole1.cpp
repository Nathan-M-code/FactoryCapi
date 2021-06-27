//
// Created by natha on 29/11/2020.
//

#include "Pole1.h"
#include "../../../Map/Map.h"

Pole1::Pole1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality):
ElectricalBuilding(map, clock, posTile, DataLoader::getBuildingType(6))
{

    setQuality(quality);

    auto &value = *m_buildingType->getValueJSON();
    unsigned nbTileNORD = value["particularity"]["nbTileNORD"].GetUint();
    unsigned nbTileSOUTH = value["particularity"]["nbTileSOUTH"].GetUint();
    unsigned nbTileWEST = value["particularity"]["nbTileWEST"].GetUint();
    unsigned nbTileEAST = value["particularity"]["nbTileEAST"].GetUint();

    setAeraElec(nbTileNORD, nbTileSOUTH, nbTileWEST, nbTileEAST);

    //--------GRAPHIC------------------

}
Pole1::~Pole1(){

}
