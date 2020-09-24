#ifndef ASSEMBLINGMACHINE1_H
#define ASSEMBLINGMACHINE1_H

#include "Building.h"

class AssemblingMachine1: public Building{

public:
    AssemblingMachine1(sf::Clock *clock, sf::Vector2i posTile);
    virtual ~AssemblingMachine1();

    virtual void update();

private:

};

#endif //ASSEMBLINGMACHINE1_H


