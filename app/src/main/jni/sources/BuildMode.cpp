#include "BuildMode.h"
#include "Map/Map.h"

using namespace std;
using namespace sf;

BuildMode::BuildMode(Map* map){
    m_active = false;
    m_okForThisTile = true;

    m_from = SOUTH;
    m_to = NORD;

    m_map = map;

    m_building = nullptr;

    m_indexTiledEntity = 0;
    BuildingType *bt = DataLoader::getBuildingType(m_indexTiledEntity);
    setBuilding(bt,NORMAL);
}
BuildMode::~BuildMode(){

}

bool BuildMode::isReady(){
    if(m_okForThisTile){
        m_okForThisTile = false;
        return (m_active && m_building);
    }
    return false;
}

void BuildMode::setBuildMode(bool act){
    m_active = act;
    if(m_active){
        m_building->setPos(m_posTileMouse);
    }
}

void BuildMode::setBuilding(BuildingType *buildingType, QualityEnum quality){
    if(!buildingType){
        cout << "impossible to BuildMode::setBuilding(buildingType = nullptr)" << endl;
        return;
    }
    m_okForThisTile = true;

    m_from = SOUTH;
    m_to = NORD;

    Building *b = nullptr;

    switch (buildingType->getNum()){
        case 0:
            b = new Belt1(m_map, m_map->getClock(), m_posTileMouse, m_from, m_to);
            break;
        case 1:
            b = new Chest1(m_map, m_map->getClock(), m_posTileMouse);
            break;
        case 2:
            b = new AssemblingMachine1(m_map, m_map->getClock(), m_posTileMouse, quality);
            break;
        case 3:
            b = new Drill1(m_map, m_map->getClock(), m_posTileMouse, quality);
            break;
        case 4:
            b = new Pump(m_map, m_map->getClock(), m_posTileMouse, m_from, m_to, quality);
            break;
        case 5:
            b = new Boiler(m_map, m_map->getClock(), m_posTileMouse, m_from, m_to, quality);
            break;
        case 6:
            b = new Pole1(m_map, m_map->getClock(), m_posTileMouse, quality);
            break;
        default:
            cout << "the numBuildingType in BuildMode::setBuilding(BuildingType *buildingType, QualityEnum quality) is not managed" << endl;
            return;
    }

    b->setAlpha(125);
    if(m_building) {
        delete m_building;
        m_building = nullptr;
    }
    m_building = b;
}

void BuildMode::changeDirectionClockwise(){
    OrientedBuilding *ob = dynamic_cast<OrientedBuilding*>(m_building);
    if(ob == nullptr){
        cout << "try to changeDirectionClockwise of a non OrientedBuilding -> impossible in BuildMode::changeDirectionClockwise()" << endl;
        return;
    }

    if(ob->getBuildingType()->getOrientationMode() == "0"){
        return;
    }else if(ob->getBuildingType()->getOrientationMode() == "4"){
        if(m_from == SOUTH && m_to == NORD){
            m_from = WEST;
            m_to = EAST;
        }else if(m_from == WEST && m_to == EAST){
            m_from = NORD;
            m_to = SOUTH;
        }else if(m_from == NORD && m_to == SOUTH){
            m_from = EAST;
            m_to = WEST;
        }else if(m_from == EAST && m_to == WEST){
            m_from = SOUTH;
            m_to = NORD;
        }
    }else if(ob->getBuildingType()->getOrientationMode() == "8"){
        if(m_from == WEST && m_to == EAST){
            m_from = WEST;
            m_to = SOUTH;
        }else if(m_from == WEST && m_to == SOUTH){
            m_from = NORD;
            m_to = SOUTH;
        }else if(m_from == NORD && m_to == SOUTH){
            m_from = NORD;
            m_to = WEST;
        }else if(m_from == NORD && m_to == WEST){
            m_from = EAST;
            m_to = WEST;
        }else if(m_from == EAST && m_to == WEST){
            m_from = EAST;
            m_to = NORD;
        }else if(m_from == EAST && m_to == NORD){
            m_from = SOUTH;
            m_to = NORD;
        }else if(m_from == SOUTH && m_to == NORD){
            m_from = SOUTH;
            m_to = EAST;
        }else if(m_from == SOUTH && m_to == EAST){
            m_from = WEST;
            m_to = EAST;
        }
    }else{
        cout << "WARNING: BuildMode::changeDirectionClockwise() orientationMode not implemented yet" << endl;
    }
    ob->setDirection(m_from,m_to);
}

void BuildMode::changeDirectionAntiClockwise(){
    OrientedBuilding *ob = dynamic_cast<OrientedBuilding*>(m_building);
    if(ob == nullptr){
        cout << "try to changeDirectionAntiClockwise of a non OrientedBuilding -> impossible in BuildMode::changeDirectionAntiClockwise()" << endl;
        return;
    }

    if(ob->getBuildingType()->getOrientationMode() == "0"){
        return;
    }else if(ob->getBuildingType()->getOrientationMode() == "4"){
        if(m_from == SOUTH && m_to == NORD){
            m_from = EAST;
            m_to = WEST;
        }else if(m_from == EAST && m_to == WEST){
            m_from = NORD;
            m_to = SOUTH;
        }else if(m_from == NORD && m_to == SOUTH){
            m_from = WEST;
            m_to = EAST;
        }else if(m_from == WEST && m_to == EAST){
            m_from = SOUTH;
            m_to = NORD;
        }
    }else if(ob->getBuildingType()->getOrientationMode() == "8"){
        if(m_from == WEST && m_to == EAST){
            m_from = WEST;
            m_to = NORD;
        }else if(m_from == WEST && m_to == NORD){
            m_from = SOUTH;
            m_to = NORD;
        }else if(m_from == SOUTH && m_to == NORD){
            m_from = SOUTH;
            m_to = WEST;
        }else if(m_from == SOUTH && m_to == WEST){
            m_from = EAST;
            m_to = WEST;
        }else if(m_from == EAST && m_to == WEST){
            m_from = EAST;
            m_to = SOUTH;
        }else if(m_from == EAST && m_to == SOUTH){
            m_from = NORD;
            m_to = SOUTH;
        }else if(m_from == NORD && m_to == SOUTH){
            m_from = NORD;
            m_to = EAST;
        }else if(m_from == NORD && m_to == EAST){
            m_from = WEST;
            m_to = EAST;
        }
    }else{
        cout << "WARNING: BuildMode::changeDirectionAntiClockwise() orientationMode not implemented yet" << endl;
    }
    ob->setDirection(m_from,m_to);
}

void BuildMode::updatePos(sf::Vector2i &posTile){
    if(posTile != m_posTileMouse){
        m_posTileMouse = posTile;
        m_okForThisTile = true;

        if(m_active){
            m_building->setPos(posTile);
        }
    }
}

void BuildMode::keyPressedEventHandler(sf::Event::KeyEvent &keyEvent){

    //cout << keyEvent.code << endl;
    if (keyEvent.code == Keyboard::B){
        setBuildMode(!m_active);
    }else if (keyEvent.code == Keyboard::R){
        if(m_active){
            if(keyEvent.shift){
                changeDirectionAntiClockwise();
            }else{
                changeDirectionClockwise();
            }
        }
    }
}

void BuildMode::wheelEventHandler(sf::Event::MouseWheelEvent &mouseWheelEvent){
    m_indexTiledEntity += mouseWheelEvent.delta;
    if(m_indexTiledEntity<0){
        m_indexTiledEntity = 0;
    }else if(m_indexTiledEntity > 6){
        m_indexTiledEntity = 6;
        //cout << "WARNING max m_indexTiledEntity = 4" << endl;
    }

    BuildingType *bt = DataLoader::getBuildingType(m_indexTiledEntity);
    if(bt != nullptr)
        setBuilding(bt,NORMAL);

}

void BuildMode::draw(sf::RenderWindow &window){
    if(m_active){
        if(m_building){
            m_building->draw(window);
        }
    }
}
