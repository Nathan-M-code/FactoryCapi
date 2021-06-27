#include "Chest1.h"
#include "../../Map/Map.h"

using namespace std;

Chest1::Chest1(Map *map, sf::Clock *clock, sf::Vector2i posTile):
ProcessBuilding(map, clock, posTile, DataLoader::getBuildingType(1), true, true),
Building(map,clock,posTile,DataLoader::getBuildingType(1)),
ItemContainer(3,{500,200})
{
    auto &value = *m_buildingType->getValueJSON();
    setMaxItemStack(value["particularity"]["slot"].GetInt());

    addStringTextinfo("\nNumber of slot : "+to_string(m_maxItemStack));
}
Chest1::~Chest1()
{

}

bool Chest1::canAcceptItem(ItemStack *is){
    return canAddItem(is);
}
void Chest1::transmitItem(ItemStack *is){
    //cout << "transmit: " << this << " to " << m_outputProcessBuilding << endl;
    removeItem(is);
    m_outputProcessBuilding->receiveItem(is);
    m_outputReady = nullptr;
}
void Chest1::receiveItem(ItemStack *is){
    //cout << "receive item in: " << this << endl;
    addItem(is);
    delete is;
}

void Chest1::clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen) {
    ProcessBuilding::clickHandler(mouseButtonEvent, cooMap, cooScreen);
    ItemContainer::clickHandler(mouseButtonEvent, cooMap, cooScreen);
}
void Chest1::mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick) {
    ProcessBuilding::mouseMoveHandler(cooScreen, cooMap, leftClick, rightClick);
    ItemContainer::mouseMoveHandler(cooScreen, cooMap, leftClick, rightClick);
}

void Chest1::update(){

    //cout << "update chest in: " << this << endl;
    if(!m_outputReady) {
        ItemStack *first = getFirstItemStack();
        if (first) {
            m_outputReady = new ItemStack(first->getItemTypeWithData(), 1);
            //cout << "m_outputReady READY" << endl;
        }
    }

    ProcessBuilding::update();
}

void Chest1::drawScreenPosUI(sf::RenderWindow &window){
    ProcessBuilding::drawScreenPosUI(window);
    ItemContainer::drawInfo(window);
}

void Chest1::drawWorldPosUI(sf::RenderWindow &window){
    ProcessBuilding::drawWorldPosUI(window);
}
