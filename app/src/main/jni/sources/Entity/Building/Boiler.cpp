#include "Boiler.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

Boiler::Boiler(Map *map, sf::Clock *clock, sf::Vector2i posTile, Direction from, Direction to, QualityEnum quality):
        OrientedProcessBuilding(map, clock, posTile, DataLoader::getBuildingType(5), from, to,true,true),
        Building(map,clock,posTile,DataLoader::getBuildingType(5)),
        RecipeProcessing(clock, true)
{
    setQuality(quality);

    auto &value = *m_buildingType->getValueJSON();
    int nbInput = value["particularity"]["nbInput"].GetInt();
    RecipeProcessing::initLate(nbInput, qualityToMult(m_quality));

    //--------GRAPHIC------------------

    addStringTextinfo("\nNb input max : "+to_string(m_nbInputIngre));

    setDirection(from, to);
}
Boiler::~Boiler()
{

}

void Boiler::setDirection(Direction from, Direction to) {
    m_from = from;
    m_to = to;

    if(m_from == SOUTH && m_to == NORD){
        setNumTexture(0);
    }else if(m_from == WEST && m_to == EAST){
        setNumTexture(1);
    }else if(m_from == NORD && m_to == SOUTH){
        setNumTexture(2);
    }else if(m_from == EAST && m_to == WEST){
        setNumTexture(3);
    }

    m_sizeOneSheet = {(float)m_sprite.getTexture()->getSize().x/m_buildingType->getTileSizeSheet()[m_numTexture].x, (float)m_sprite.getTexture()->getSize().y/m_buildingType->getTileSizeSheet()[m_numTexture].y};
    m_sprite.setScale(1,1);
    m_sprite.setTextureRect(IntRect(m_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,m_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
    m_sprite.setScale(m_buildingType->getSizeSprite()[m_numTexture].x/m_sprite.getGlobalBounds().width,m_buildingType->getSizeSprite()[m_numTexture].y/m_sprite.getGlobalBounds().height);
}

void Boiler::setElectricalBuilding(ElectricalBuilding *electricalBuilding){
    Building::setElectricalBuilding(electricalBuilding);
    //m_electricalBuilding->getElectricalCircuit()->addCapacityBuilding(1);
}

bool Boiler::canAcceptItem(ItemStack *is){
    return RecipeProcessing::canAcceptItem(is);
}
void Boiler::transmitItem(ItemStack *is){ //is == m_outputReady
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
void Boiler::receiveItem(ItemStack *is){
    addIngredient(is);
    delete is;
}

void Boiler::startProducing(){
    cout << "debut Boiler::startProducing" << endl;

    RecipeProcessing::startProducing();
    startSpriteProcessing();

    cout << "RecipeProcessing::m_isProducingElectricity: " << boolToString(m_isProducingElectricity) << endl;
    if(RecipeProcessing::m_isProducingElectricity){
        cout << "m_electricalBuilding: " << m_electricalBuilding << endl;
        if(m_electricalBuilding) {
            m_electricalBuilding->getElectricalCircuit()->addPowerProduced(RecipeProcessing::m_powerProduced);
        }
    }

    cout << "fin Boiler::startProducing" << endl;
}
void Boiler::stopProducing(){
    cout << "debut Boiler::stopProducing" << endl;

    RecipeProcessing::stopProducing();
    stopSpriteProcessing();

    if(m_electricalBuilding){
        m_electricalBuilding->getElectricalCircuit()->removePowerProduced(RecipeProcessing::m_powerProduced);
    }

    cout << "fin Boiler::stopProducing" << endl;
}

void Boiler::update(){
    ProcessBuilding::update();
    RecipeProcessing::update();

    if(RecipeProcessing::m_flushing){
        if(!m_outputReady){
            ItemStack *first = RecipeProcessing::getFirstItemStackIngredient();
            if (first) {
                m_outputReady = new DrawableItemStack(first->getItemTypeWithData(), m_bgOutput.getSize(), m_bgOutput.getPosition(), 1);
                //cout << "m_outputReady READY" << endl;
            }
        }
    }else{
        //put ouput to outputReady
        if(!m_outputReady){
            ItemStack *first = RecipeProcessing::getFirstItemStackResult();
            if (first) {
                m_outputReady = new DrawableItemStack(first->getItemTypeWithData(), m_bgOutput.getSize(), m_bgOutput.getPosition(), 1);
                //cout << "m_outputReady READY" << endl;
            }
        }
    }
}

void Boiler::clickHandler(const Event::MouseButtonEvent &mouseButtonEvent, const Vector2f &cooMap, const Vector2i &cooScreen) {
    ProcessBuilding::clickHandler(mouseButtonEvent, cooMap, cooScreen);
    RecipeProcessing::clickHandler(mouseButtonEvent, cooMap, cooScreen);
}

void Boiler::mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick){
    ProcessBuilding::mouseMoveHandler(cooScreen,cooMap,leftClick,rightClick);
    RecipeProcessing::mouseMoveHandler(cooScreen,cooMap,leftClick,rightClick);
}

void Boiler::drawScreenPosUI(sf::RenderWindow &window){
    ProcessBuilding::drawScreenPosUI(window);
    RecipeProcessing::drawScreenPosUI(window);

    if(m_outputReady) {
        m_outputReady->draw(window);
    }
}