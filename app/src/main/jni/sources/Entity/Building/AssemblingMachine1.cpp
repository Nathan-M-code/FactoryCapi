#include "AssemblingMachine1.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

AssemblingMachine1::AssemblingMachine1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality):
ProcessBuilding(map, clock, posTile, DataLoader::getBuildingType(2),true,true),
Building(map,clock,posTile,DataLoader::getBuildingType(2)),
RecipeProcessing(clock, false)
{
    setQuality(quality);

    auto &value = *m_buildingType->getValueJSON();
    int nbInput = value["particularity"]["nbInput"].GetInt();
    RecipeProcessing::initLate(nbInput, qualityToMult(m_quality));


    //--------GRAPHIC------------------

    addStringTextinfo("\nNb input max : "+to_string(m_nbInputIngre));

}
AssemblingMachine1::~AssemblingMachine1()
{

}

bool AssemblingMachine1::canAcceptItem(ItemStack *is){
    return RecipeProcessing::canAcceptItem(is);
}
void AssemblingMachine1::transmitItem(ItemStack *is){ //is == m_outputReady
    if(m_flushing && isAnIngredient(is)) {
        //cout << "its an ingrdient" << endl;
        removeItemStackIngredient(is);
    }else if(isAnOutput(is)) {
        //cout << "its an output" << endl;
        removeItemStackOutput(is);
    }
    m_outputProcessBuilding->receiveItem(is);
    m_outputReady = nullptr;
}
void AssemblingMachine1::receiveItem(ItemStack *is){
    addIngredient(is);
    delete is;
}

void AssemblingMachine1::startProducing(){
    RecipeProcessing::startProducing();
    startSpriteProcessing();
}
void AssemblingMachine1::stopProducing(){
    RecipeProcessing::stopProducing();
    stopSpriteProcessing();
}

void AssemblingMachine1::update(){
    ProcessBuilding::update();
    RecipeProcessing::update();

    if(m_flushing){
        if(!m_outputReady){
            ItemStack *first = getFirstItemStackIngredient();
            if (first) {
                m_outputReady = new DrawableItemStack(first->getItemTypeWithData(), m_bgOutput.getSize(), m_bgOutput.getPosition(), 1);
                //cout << "m_outputReady READY" << endl;
            }
        }
    }else{
        //put ouput to outputReady
        if(!m_outputReady){
            ItemStack *first = getFirstItemStackResult();
            if (first) {
                m_outputReady = new DrawableItemStack(first->getItemTypeWithData(), m_bgOutput.getSize(), m_bgOutput.getPosition(), 1);
                //cout << "m_outputReady READY" << endl;
            }
        }
    }
}

void AssemblingMachine1::clickHandler(const Event::MouseButtonEvent &mouseButtonEvent, const Vector2f &cooMap, const Vector2i &cooScreen) {
    ProcessBuilding::clickHandler(mouseButtonEvent, cooMap, cooScreen);
    RecipeProcessing::clickHandler(mouseButtonEvent, cooMap, cooScreen);
}

void AssemblingMachine1::mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick){
    ProcessBuilding::mouseMoveHandler(cooScreen,cooMap,leftClick,rightClick);
    RecipeProcessing::mouseMoveHandler(cooScreen,cooMap,leftClick,rightClick);
}

void AssemblingMachine1::drawScreenPosUI(sf::RenderWindow &window){
    ProcessBuilding::drawScreenPosUI(window);
    RecipeProcessing::drawScreenPosUI(window);

    if(m_outputReady) {
        m_outputReady->draw(window);
    }
}



