#include "AssemblingMachine1.h"

using namespace std;

AssemblingMachine1::AssemblingMachine1(sf::Clock *clock, sf::Vector2i posTile): Building(clock, posTile, TextureLoader::getBuildingType("AssemblingMachine1"))
{

}
AssemblingMachine1::~AssemblingMachine1()
{

}

void AssemblingMachine1::update(){
    Building::update();
}

