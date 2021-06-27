#include "OrientedProcessBuilding.h"

using namespace std;
using namespace sf;

OrientedProcessBuilding::OrientedProcessBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, Direction from, Direction to, bool hasInput, bool hasOutput):
ProcessBuilding(map,clock,posTile,buildingType,hasInput,hasOutput),
OrientedBuilding(map,clock,posTile,buildingType,from,to)
{
    //cout << "OrientedProcessBuilding::OrientedProcessBuilding" << endl;
}

OrientedProcessBuilding::~OrientedProcessBuilding(){
}

void OrientedProcessBuilding::update(){
    ProcessBuilding::update();
}

void OrientedProcessBuilding::clickHandler(const Event::MouseButtonEvent &mouseButtonEvent, const Vector2f &cooMap, const Vector2i &cooScreen) {
    ProcessBuilding::clickHandler(mouseButtonEvent, cooMap, cooScreen);
}

void OrientedProcessBuilding::draw(sf::RenderWindow &window){
    ProcessBuilding::draw(window);
}
