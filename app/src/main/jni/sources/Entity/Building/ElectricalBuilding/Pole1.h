//
// Created by natha on 29/11/2020.
//

#ifndef POLE1_H
#define POLE1_H

#include "ElectricalBuilding.h"

class Pole1 : public ElectricalBuilding{
public:
    Pole1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality);
    ~Pole1();
private:
};


#endif //POLE1_H
