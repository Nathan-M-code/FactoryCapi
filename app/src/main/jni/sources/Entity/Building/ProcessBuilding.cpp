#include "ProcessBuilding.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;

ProcessBuilding::ProcessBuilding(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, bool hasInput, bool hasOutput):
Building(map,clock,posTile,buildingType)
{
    m_hasInput = hasInput;
    m_hasOutput = hasOutput;

    addStringTextinfo("\nhasInput : "+boolToString(m_hasInput));
    addStringTextinfo("\nhasOutput : "+boolToString(m_hasOutput));

    m_outputReady = nullptr;

    m_outputProcessBuilding = nullptr;

    m_spriteArrowOutput.setTexture(*DataLoader::getTextureArrowUI());
    m_spriteArrowOutput.setColor(sf::Color(255,100,100,170));
    m_spriteArrowOutput.setOrigin(m_spriteArrowOutput.getTexture()->getSize().x/2.0, m_spriteArrowOutput.getTexture()->getSize().y/2.0);

    m_spriteArrowInput.setTexture(*DataLoader::getTextureArrowUI());
    m_spriteArrowInput.setColor(sf::Color(100,255,100,170));
    m_spriteArrowInput.setOrigin(m_spriteArrowInput.getTexture()->getSize().x/2.0, m_spriteArrowInput.getTexture()->getSize().y/2.0);
}
ProcessBuilding::~ProcessBuilding(){
}

void ProcessBuilding::addOutputProcessBuilding(ProcessBuilding* outputProcessBuilding){
    //cout << "addOutputProcessBuilding" << endl;
    if(m_hasOutput){
        if(outputProcessBuilding->hasInput()){

            //test to avoid double
            //not needed anymore
            /*if(m_outputProcessBuilding == outputProcessBuilding){
                cout << "already handled" << endl;
                return;
            }
            for(unsigned int i=0;i<m_processBuildingOutputAvailable.size();i++){
                if(m_processBuildingOutputAvailable[i] == outputProcessBuilding){
                    cout << "already handled" << endl;
                    return;
                }
            }*/
            //cout << "ADD" <<endl;
            m_processBuildingOutputAvailable.push_back(outputProcessBuilding);
            if(!m_outputProcessBuilding){
                setOutputProcessBuilding(0);
            }
        }else{
            cout << "WARNING: you cant ProcessBuilding::addOutputProcessBuilding a building without input" << endl;
        }

    }else{
        cout << "WARNING: you cant ProcessBuilding::addOutputProcessBuilding a building without output" << endl;
    }
}
void ProcessBuilding::removeOutputProcessBuilding(ProcessBuilding* outputProcessBuilding){
    if(m_outputProcessBuilding == outputProcessBuilding){
        m_outputProcessBuilding = nullptr;
    }

    for (unsigned i = 0; i < m_processBuildingOutputAvailable.size(); ++i) {
        if(m_processBuildingOutputAvailable[i] == outputProcessBuilding){
            m_processBuildingOutputAvailable.erase(m_processBuildingOutputAvailable.begin()+i);
        }
    }
}

void ProcessBuilding::setOutputProcessBuilding(unsigned index){

    if(m_processBuildingOutputAvailable.size()==0)
        return;

    if(index >= m_processBuildingOutputAvailable.size())
        return;

    if(m_outputProcessBuilding){
        m_outputProcessBuilding->removeInputProcessBuilding(this);
        ProcessBuilding* save = m_outputProcessBuilding;
        m_outputProcessBuilding = m_processBuildingOutputAvailable[index];
        m_processBuildingOutputAvailable.erase(m_processBuildingOutputAvailable.begin()+index);
        m_processBuildingOutputAvailable.push_back(save);
    }else{
        m_outputProcessBuilding = m_processBuildingOutputAvailable[index];
        m_processBuildingOutputAvailable.erase(m_processBuildingOutputAvailable.begin()+index);
    }

    //cout << "setOutputProcessBuilding in: " << this << " -> " << m_outputProcessBuilding << endl;
    //cout << "m_outputProcessBuilding: " << m_outputProcessBuilding << endl;
    //cout << "m_processBuildingOutputAvailable.size: " << m_processBuildingOutputAvailable.size() << endl;

    m_outputProcessBuilding->addInputProcessBuilding(this);

    float dir = atan2(m_outputProcessBuilding->getMidPos().y-getMidPos().y, m_outputProcessBuilding->getMidPos().x-getMidPos().x);
    m_spriteArrowOutput.setScale(1,1);
    m_spriteArrowOutput.setRotation(0);
    m_spriteArrowOutput.setPosition((m_outputProcessBuilding->getMidPos().x+getMidPos().x)/2.0, (m_outputProcessBuilding->getMidPos().y+getMidPos().y)/2.0);
    m_spriteArrowOutput.setScale((sqrt((m_outputProcessBuilding->getMidPos().x-getMidPos().x)*(m_outputProcessBuilding->getMidPos().x-getMidPos().x)+(m_outputProcessBuilding->getMidPos().y-getMidPos().y)*(m_outputProcessBuilding->getMidPos().y-getMidPos().y)))/m_spriteArrowOutput.getGlobalBounds().width,1);
    m_spriteArrowOutput.setRotation(dir*180.0/PI);
}

void ProcessBuilding::deselectOutputProcessBuilding(){
    //cout << "deselectOutputProcessBuilding in: " << this << endl;
    if(m_outputProcessBuilding){
        m_outputProcessBuilding->removeInputProcessBuilding(this);
        m_processBuildingOutputAvailable.push_back(m_outputProcessBuilding);
        m_outputProcessBuilding = nullptr;
    }
}

void ProcessBuilding::addInputProcessBuilding(ProcessBuilding* inputProcessBuilding){
    //cout << "addOutputProcessBuilding" << endl;
    if(m_hasInput){
        if(inputProcessBuilding->hasOutput()){

            //test to avoid double
            //not needed anymore
            /*if(m_outputProcessBuilding == outputProcessBuilding){
                cout << "already handled" << endl;
                return;
            }
            for(unsigned int i=0;i<m_processBuildingOutputAvailable.size();i++){
                if(m_processBuildingOutputAvailable[i] == outputProcessBuilding){
                    cout << "already handled" << endl;
                    return;
                }
            }*/
            //cout << "ADD" <<endl;
            m_listInputProcessBuilding.push_back(inputProcessBuilding);
        }else{
            cout << "WARNING: you cant ProcessBuilding::addInputProcessBuilding a building without output" << endl;
        }

    }else{
        cout << "WARNING: you cant ProcessBuilding::addInputProcessBuilding a building without input" << endl;
    }
}

void ProcessBuilding::removeInputProcessBuilding(ProcessBuilding* inputProcessBuilding){
    for(unsigned int i=0;i<m_listInputProcessBuilding.size();i++){
        if(m_listInputProcessBuilding[i] == inputProcessBuilding){
            m_listInputProcessBuilding.erase(m_listInputProcessBuilding.begin()+i);
            break;
        }
    }
}

void ProcessBuilding::update(){
    Building::update();

    //cout << "update: " << this << endl;

    //cout << "m_outputReady: " << m_outputReady << endl;
    if(m_outputReady){
        //cout << "m_outputReady OK" << endl;
        if(m_outputProcessBuilding){
            //cout << "m_outputProcessBuilding OK" << endl;
            if(m_outputProcessBuilding->canAcceptItem(m_outputReady)){
                //cout << "Transmit!!!" << endl;
                transmitItem(m_outputReady);
                //now the next two lines are handled by the transmitItem method
                /*delete m_outputReady;
                m_outputReady = nullptr;*/
            }
        }
    }
}

void ProcessBuilding::clickHandler(const Event::MouseButtonEvent &mouseButtonEvent, const Vector2f &cooMap, const Vector2i &cooScreen) {
    Building::clickHandler(mouseButtonEvent, cooMap, cooScreen);
}

void ProcessBuilding::mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick){
    Building::mouseMoveHandler(cooScreen, cooMap, leftClick, rightClick);
}

void ProcessBuilding::draw(sf::RenderWindow &window){
    Building::draw(window);
}

void ProcessBuilding::drawWorldPosUI(sf::RenderWindow &window){
    Building::drawWorldPosUI(window);
    //TODO: precalculate position of lines and position of sprites
    sf::VertexArray lines(sf::Lines, m_processBuildingOutputAvailable.size()*2);

    for(unsigned int i=0;i<m_processBuildingOutputAvailable.size();i++){
        lines[i*2].position = getMidPos();
        lines[i*2].color = sf::Color(255,100,100);
        lines[i*2+1].position = m_processBuildingOutputAvailable[i]->getMidPos();
        lines[i*2+1].color = sf::Color(255,100,100);
        //cout << lines[i*2].position << " " << lines[i*2+1].position << endl;
    }

    window.draw(lines);

    if(m_outputProcessBuilding){
        window.draw(m_spriteArrowOutput);
    }

    for(unsigned int i=0;i<m_listInputProcessBuilding.size();i++){
        float dir = atan2(m_listInputProcessBuilding[i]->getMidPos().y-getMidPos().y, m_listInputProcessBuilding[i]->getMidPos().x-getMidPos().x);
        //cout << "dir: " << dir << endl;
        m_spriteArrowInput.setScale(1,1);
        m_spriteArrowInput.setRotation(0);
        m_spriteArrowInput.setPosition((m_listInputProcessBuilding[i]->getMidPos().x+getMidPos().x)/2.0+cos(dir+PI/2.0)*m_listInputProcessBuilding[i]->getBuildingType()->getTileSizeBox()[m_numTexture].x*Map::s_sizeTileBuildings/6.0, (m_listInputProcessBuilding[i]->getMidPos().y+getMidPos().y)/2.0+sin(dir+PI/2.0)*m_listInputProcessBuilding[i]->getBuildingType()->getTileSizeBox()[m_numTexture].y*Map::s_sizeTileBuildings/6.0);
        m_spriteArrowInput.setScale((sqrt((m_listInputProcessBuilding[i]->getMidPos().x-getMidPos().x)*(m_listInputProcessBuilding[i]->getMidPos().x-getMidPos().x)+(m_listInputProcessBuilding[i]->getMidPos().y-getMidPos().y)*(m_listInputProcessBuilding[i]->getMidPos().y-getMidPos().y)))/2.0/m_spriteArrowInput.getGlobalBounds().width,1);
        if(dir >= 0){
            m_spriteArrowInput.setRotation(-180+dir*180.0/PI);
        }else{
            m_spriteArrowInput.setRotation(180+dir*180.0/PI);
        }
        //cout << "actual deg: " << m_spriteArrowInput.getRotation() << endl;
        window.draw(m_spriteArrowInput);
    }
}

void ProcessBuilding::drawScreenPosUI(sf::RenderWindow &window){
    Building::drawScreenPosUI(window);
}
