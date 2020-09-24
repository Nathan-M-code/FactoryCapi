#include "Chest1.h"

using namespace std;

Chest1::Chest1(sf::Clock *clock, sf::Vector2i posTile): Building(clock, posTile, TextureLoader::getBuildingType("Chest1"))
{

}
Chest1::~Chest1()
{

}

void Chest1::update(){
    //cout << "test1" << endl;
}
