#include "Building.h"

#include "../../Map/Map.h"
#include "../../OptionManager.h"

using namespace rapidjson;
using namespace std;
using namespace sf;

Building::Building():
Entity()
{

}

Building::Building(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType):
Entity(map)
{
    //cout << "start Building::Building" << endl;

    m_clock = clock;

    m_buildingType = buildingType;

    m_posTile = posTile;

    m_previousTime = m_clock->getElapsedTime();
    m_elapsedTime = milliseconds(0);
    //cout << "m_msEachSheet: " << m_msEachSheet.asMilliseconds() << endl;

    m_spriteProcessing = true;
    m_isReversing = false;
    m_indexSpriteSheet = 0;

    setNumTexture(0);

    initTextStyle(m_textInfo,DataLoader::getFont(),35);
    initRectStyle(m_bgInfo);
    m_textInfo.setPosition(0,0);
    addStringTextinfo("BuidingType : "+m_buildingType->getName());

    setElectricalBuilding(nullptr);


    //cout << "end Building::Building" << endl;
}

Building::Building(Map *map, sf::Clock* clock, Building *buildingToCopy):
Entity(map)
{
    //cout << "start Building::Building copy" << endl;

    m_clock = clock;

    m_buildingType = buildingToCopy->getBuildingType();

    m_previousTime = m_clock->getElapsedTime();
    m_elapsedTime = milliseconds(0);
    //cout << "m_msEachSheet: " << m_msEachSheet.asMilliseconds() << endl;

    m_spriteProcessing = buildingToCopy->m_spriteProcessing;
    m_isReversing = buildingToCopy->m_isReversing;
    m_indexSpriteSheet = buildingToCopy->m_indexSpriteSheet;

    setNumTexture(buildingToCopy->getNumTexture());

    //m_sizeOneSheet = {m_buildingType->getTexture(m_numTexture)->getSize().x/(float)m_buildingType->getTileSizeSheet()[m_numTexture].x, m_buildingType->getTexture(m_numTexture)->getSize().y/(float)m_buildingType->getTileSizeSheet()[m_numTexture].y};
    m_sizeOneSheet = buildingToCopy->m_sizeOneSheet;
    //cout << m_sizeOneSheet.x << " " << m_sizeOneSheet.y << endl;
    m_sprite = Sprite(*m_buildingType->getTexture(m_numTexture),IntRect(m_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,m_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].y*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
    m_sprite.setScale(m_buildingType->getSizeSprite()[m_numTexture].x/m_sprite.getGlobalBounds().width,m_buildingType->getSizeSprite()[m_numTexture].y/m_sprite.getGlobalBounds().height);

    m_posTile = buildingToCopy->m_posTile;

    m_pos = buildingToCopy->m_pos;
    /*m_pos.x = m_posTile.x*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().width-m_buildingType->getTileSizeBox()[m_numTexture].x*Map::s_sizeTileBuildings)/2.0);
    if(m_sprite.getGlobalBounds().height > m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings){
        m_pos.y = m_posTile.y*Map::s_sizeTileBuildings-(m_sprite.getGlobalBounds().height-m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings);
    }else{
        m_pos.y = m_posTile.y*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().height-m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings)/2.0);
    }*/
    m_sprite.setPosition(m_pos);

    initTextStyle(m_textInfo,DataLoader::getFont(),35);
    initRectStyle(m_bgInfo);
    m_textInfo.setPosition(0,0);
    addStringTextinfo("BuidingType : "+m_buildingType->getName());

    setElectricalBuilding(nullptr);
}

Building::~Building(){

}

void Building::setPos(sf::Vector2i posTile){
    m_posTile = posTile;

    m_pos.x = posTile.x*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().width-m_buildingType->getTileSizeBox()[m_numTexture].x*Map::s_sizeTileBuildings)/2.0);
    if(m_sprite.getGlobalBounds().height > m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings){
        m_pos.y = posTile.y*Map::s_sizeTileBuildings-(m_sprite.getGlobalBounds().height-m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings);
    }else{
        m_pos.y = posTile.y*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().height-m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings)/2.0);
    }
    m_sprite.setPosition(m_pos);
}

void Building::setQuality(QualityEnum quality){
    if(m_buildingType->hasQuality()){
        m_quality = quality;
        m_msEachSheet = sf::milliseconds(1000.0/(m_buildingType->getNbCyclePerSecond()*qualityToMult(m_quality))/m_buildingType->getNbSheet(m_numTexture));
        //cout << "set m_msEachSheet to: " << m_msEachSheet.asMilliseconds() << endl;
    }else{
        cout << "cant set Quality to a building that doesnt have quality" << endl;
    }
    addStringTextinfo("\nQuality : "+qualityToString(m_quality));
}

sf::Vector2f Building::getMidPos() const{
    return {m_posTile.x*Map::s_sizeTileBuildings+m_buildingType->getTileSizeBox()[m_numTexture].x*Map::s_sizeTileBuildings/2, m_posTile.y*Map::s_sizeTileBuildings+m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings/2};
}

void Building::stopSpriteProcessing(){
    m_spriteProcessing = false;
}
void Building::startSpriteProcessing(){
    m_spriteProcessing = true;
    m_previousTime = m_clock->getElapsedTime();
}

void Building::setNumTexture(unsigned num){
    //cout << this << " Building::setNumTexture change num" << endl;
    m_numTexture = num;
    //cout << "m_numTexture: " << m_numTexture << endl;
    m_msEachSheet = m_buildingType->getMSEachTileSheet(m_numTexture);

    sf::Color saveColor = m_sprite.getColor();

    m_sprite.setTexture(*m_buildingType->getTexture(m_numTexture));

    m_sizeOneSheet = {m_buildingType->getTexture(m_numTexture)->getSize().x/(float)m_buildingType->getTileSizeSheet()[m_numTexture].x, m_buildingType->getTexture(m_numTexture)->getSize().y/(float)m_buildingType->getTileSizeSheet()[m_numTexture].y};
    m_sprite = Sprite(*m_buildingType->getTexture(m_numTexture),IntRect(m_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,m_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].y*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
    /*cout << "m_indexSpriteSheet: " << m_indexSpriteSheet << endl;
    cout << "x sprite: " << m_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x << endl;
    cout << "y sprite: " << m_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].y*m_sizeOneSheet.y << endl;
    cout << "width sprite: " << m_sizeOneSheet.x << endl;
    cout << "height sprite: " << m_sizeOneSheet.y << endl;*/
    m_sprite.setScale(m_buildingType->getSizeSprite()[m_numTexture].x/m_sprite.getGlobalBounds().width,m_buildingType->getSizeSprite()[m_numTexture].y/m_sprite.getGlobalBounds().height);

    m_pos.x = m_posTile.x*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().width-m_buildingType->getTileSizeBox()[m_numTexture].x*Map::s_sizeTileBuildings)/2.0);
    if(m_sprite.getGlobalBounds().height > m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings){
        m_pos.y = m_posTile.y*Map::s_sizeTileBuildings-(m_sprite.getGlobalBounds().height-m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings);
    }else{
        m_pos.y = m_posTile.y*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().height-m_buildingType->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings)/2.0);
    }
    m_sprite.setPosition(m_pos);

    m_sprite.setColor(saveColor);
}

void Building::setElectricalBuilding(ElectricalBuilding *electricalBuilding){
    //cout << this << " Building::setElectricalBuilding: " << electricalBuilding << endl;
    m_electricalBuilding = electricalBuilding;
    if(m_electricalBuilding){
        m_arrowToElecBuilding.setTexture(*DataLoader::getTextureArrowUI());
        m_arrowToElecBuilding.setColor(sf::Color(100,100,255,170));
        m_arrowToElecBuilding.setOrigin(m_arrowToElecBuilding.getTexture()->getSize().x/2.0, m_arrowToElecBuilding.getTexture()->getSize().y/2.0);

        float dir = atan2(electricalBuilding->getMidPos().y-getMidPos().y, electricalBuilding->getMidPos().x-getMidPos().x);
        m_arrowToElecBuilding.setScale(1,1);
        m_arrowToElecBuilding.setRotation(0);
        m_arrowToElecBuilding.setPosition((electricalBuilding->getMidPos().x+getMidPos().x)/2.0, (electricalBuilding->getMidPos().y+getMidPos().y)/2.0);
        m_arrowToElecBuilding.setScale((sqrt((electricalBuilding->getMidPos().x-getMidPos().x)*(electricalBuilding->getMidPos().x-getMidPos().x)+(electricalBuilding->getMidPos().y-getMidPos().y)*(electricalBuilding->getMidPos().y-getMidPos().y)))/m_arrowToElecBuilding.getGlobalBounds().width,1);
        m_arrowToElecBuilding.setRotation(dir*180.0/PI);
    }
}

bool Building::canBeDeconstructed() const{
    return true;
}

void Building::addStringTextinfo(std::string str){
    m_textInfo.setString(m_textInfo.getString()+str);
    m_bgInfo.setSize({m_textInfo.getGlobalBounds().width+20, m_textInfo.getGlobalBounds().height+10});
    m_bgInfo.setPosition(m_textInfo.getPosition().x-10, m_textInfo.getPosition().y);
}

void Building::update(){

    if(m_buildingType->getNbSheet(m_numTexture) > 1 && m_spriteProcessing){
        m_actualTime = m_clock->getElapsedTime();
        m_elapsedTime += m_actualTime-m_previousTime;
        m_previousTime = m_actualTime;

        while(m_elapsedTime >= m_msEachSheet){
            m_elapsedTime -= m_msEachSheet;
            m_indexSpriteSheet+=1;

            //cout << m_indexSpriteSheet << " " << m_buildingType->getNbSheet() << endl;
            if(m_indexSpriteSheet == m_buildingType->getNbSheet(m_numTexture)){
                //cout << "reversing" << endl;
                m_isReversing = !m_isReversing;
                m_indexSpriteSheet = 0;
            }
        }
        //cout << m_buildingType->isBoucingSpriteSheet(0) << " " << m_isReversing << endl;
        if(m_buildingType->isBoucingSpriteSheet(0) && m_isReversing){
            //cout << " index: " << m_indexSpriteSheet << " x: " << (m_buildingType->getTileSizeSheet().x-(m_indexSpriteSheet%m_buildingType->getTileSizeSheet().x)-1) << " y: " << (m_buildingType->getTileSizeSheet().y-(m_indexSpriteSheet/m_buildingType->getTileSizeSheet().x)-1) << endl;
            m_sprite.setTextureRect(IntRect((m_buildingType->getTileSizeSheet()[m_numTexture].x-(m_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x)-1)*m_sizeOneSheet.x,(m_buildingType->getTileSizeSheet()[m_numTexture].y-(m_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].x)-1)*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
        }else{
            //cout << " index: " << m_indexSpriteSheet << " x: " << m_indexSpriteSheet%m_buildingType->getTileSizeSheet().x << " y: " << m_indexSpriteSheet/m_buildingType->getTileSizeSheet().y << endl;
            m_sprite.setTextureRect(IntRect(m_indexSpriteSheet%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,m_indexSpriteSheet/m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));
        }
    }
}

void Building::clickHandler(const Event::MouseButtonEvent &mouseButtonEvent, const Vector2f &cooMap, const Vector2i &cooScreen) {
    m_saveCooScreen = cooScreen;
}

void Building::mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick){
    m_saveCooScreen = cooScreen;
}

void Building::draw(sf::RenderWindow &window){
    Entity::draw(window);
}

void Building::drawScreenPosUI(sf::RenderWindow &window){
    window.draw(m_bgInfo);
    window.draw(m_textInfo);
}

void Building::drawWorldPosUI(sf::RenderWindow &window){
    if(m_electricalBuilding){
        window.draw(m_arrowToElecBuilding);
    }
}


