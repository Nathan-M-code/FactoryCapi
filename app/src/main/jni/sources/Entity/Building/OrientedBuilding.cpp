#include "OrientedBuilding.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

OrientedBuilding::OrientedBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, Direction from, Direction to):
Building(map, clock, posTile, buildingType)
{
    //cout << "start OrientedBuilding::OrientedBuilding" << endl;

    m_from = from;
    m_to = to;

    addStringTextinfo("\nOrientation type : "+m_buildingType->getOrientationMode());
    addStringTextinfo("\nOrientation : "+directionToString(m_from)+" to "+directionToString(m_to));

    //cout << "end OrientedBuilding::OrientedBuilding" << endl;
}
OrientedBuilding::~OrientedBuilding(){

}

void OrientedBuilding::setPos(sf::Vector2i posTile){
    Building::setPos(posTile);
}

void OrientedBuilding::setDirection(Direction from, Direction to){
    m_from = from;
    m_to = to;
}

void OrientedBuilding::clickHandler(const Event::MouseButtonEvent &mouseButtonEvent, const Vector2f &cooMap, const Vector2i &cooScreen) {
    Building::clickHandler(mouseButtonEvent, cooMap, cooScreen);
}

void OrientedBuilding::draw(sf::RenderWindow &window){
    Building::draw(window);
}
