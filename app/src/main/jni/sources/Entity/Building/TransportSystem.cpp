#include "TransportSystem.h"
#include "../../Map/Map.h"

using namespace std;
using namespace sf;
using namespace rapidjson;

unsigned TransportSystem::s_density = 2;

TransportSystem::TransportSystem(Map *map, sf::Clock* clock, sf::Vector2i posTile, BuildingType *buildingType, Direction from, Direction to):
OrientedProcessBuilding(map,clock,posTile,buildingType,from,to,true,true)
{
    m_sizeOneSheet = {(float)m_buildingType->getTexture(0)->getSize().x/m_buildingType->getTileSizeSheet()[m_numTexture].x, (float)m_buildingType->getTexture(0)->getSize().y/20/m_buildingType->getTileSizeSheet()[m_numTexture].y};
    //cout << "m_sizeOneSheet: " << m_sizeOneSheet.x  << " " << m_sizeOneSheet.y << endl;

    m_indexSpriteOffsetY = TransportSystem::getIndexSpriteByDirection(m_from, m_to);
    //cout << m_indexSpriteOffsetY << endl;
    //cout << 0%m_buildingType->getTileSizeSheet().x*m_sizeOneSheet.x << " " << (m_indexSpriteOffsetY*m_sizeOneSheet.y)+0/m_buildingType->getTileSizeSheet().x*m_sizeOneSheet.y << " " << m_sizeOneSheet.x << " " << m_sizeOneSheet.y <<endl;

    m_sprite.setTextureRect(IntRect(0%m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.x,(m_indexSpriteOffsetY*m_sizeOneSheet.y)+0/m_buildingType->getTileSizeSheet()[m_numTexture].x*m_sizeOneSheet.y,m_sizeOneSheet.x,m_sizeOneSheet.y));

    m_sprite.setScale(m_buildingType->getSizeSprite()[m_numTexture].x/m_sprite.getTextureRect().width,m_buildingType->getSizeSprite()[m_numTexture].y/m_sprite.getTextureRect().height);

    setPos(posTile);

    auto &value = *m_buildingType->getValueJSON();
    m_itemPerSec = value["particularity"]["itemPerSec"].GetDouble();

    m_speed = s_density/m_itemPerSec;
    //cout << "calcul speed TransportSystem: " << m_speed << endl;
    m_pxlPerTick = Map::s_sizeTileBuildings/(m_speed*UPS);
    //cout << std::setprecision(9) << "calcul m_pxlPerTick TransportSystem: " << m_pxlPerTick << endl;

    m_nextTS = nullptr;

    addStringTextinfo("\nItem per sec : "+to_string(m_itemPerSec));
}
TransportSystem::~TransportSystem(){

}

void TransportSystem::setPos(sf::Vector2i posTile){
    m_posTile = posTile;
    m_pos.x = posTile.x*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().width-Map::s_sizeTileBuildings)/2.0);
    m_pos.y = posTile.y*Map::s_sizeTileBuildings-((m_sprite.getGlobalBounds().height-Map::s_sizeTileBuildings)/2.0);
    updatePosBoundary();
    m_sprite.setPosition(m_pos);
}

void TransportSystem::setDirection(Direction from, Direction to){
    m_from = from;
    m_to = to;
    m_indexSpriteOffsetY = TransportSystem::getIndexSpriteByDirection(m_from, m_to);
    //cout << "new m_indexSpriteOffsetY: " << m_indexSpriteOffsetY << endl;
    updatePosBoundary();
}

PhysicalItem* TransportSystem::getLastPhysicalItem() const{
    if(m_listPhysicalItem.size()>0){
        return m_listPhysicalItem[m_listPhysicalItem.size()-1];
    }
    return nullptr;
}

unsigned TransportSystem::getIndexSpriteByDirection(Direction from, Direction to){
    //cout << "TransportSystem::getIndexSpriteByDirection from: " << from << " to: " << to << endl;
    if(from == WEST && to == EAST){
        return 0;
    }else if(from == EAST && to == WEST){
        return 1;
    }else if(from == SOUTH && to == NORD){
        return 2;
    }else if(from == NORD && to == SOUTH){
        return 3;
    }else if(from == EAST && to == NORD){
        return 4;
    }else if(from == NORD && to == EAST){
        return 5;
    }else if(from == WEST && to == NORD){
        return 6;
    }else if(from == NORD && to == WEST){
        return 7;
    }else if(from == SOUTH && to == EAST){
        return 8;
    }else if(from == EAST && to == SOUTH){
        return 9;
    }else if(from == SOUTH && to == WEST){
        return 10;
    }else if(from == WEST && to == SOUTH){
        return 11;
    }
}

//----------------------------------------------------------
void TransportSystem::updatePosBoundary(){
    if(m_to == EAST){
        m_posBoundary = (m_posTile.x+1)*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        if(m_from != WEST){ //EAST OR WEST
            m_posSemiBoundary = (m_posTile.y+0.5)*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        }
    }else if(m_to == WEST){
        m_posBoundary = m_posTile.x*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        if(m_from != EAST){ //EAST OR WEST
            m_posSemiBoundary = (m_posTile.y+0.5)*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        }
    }else if(m_to == NORD){
        m_posBoundary = m_posTile.y*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        if(m_from != SOUTH){ //EAST OR WEST
            m_posSemiBoundary = (m_posTile.x+0.5)*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        }
    }else if(m_to == SOUTH){
        m_posBoundary = (m_posTile.y+1)*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        if(m_from != NORD){ //EAST OR WEST
            m_posSemiBoundary = (m_posTile.x+0.5)*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0;
        }
    }
}

void TransportSystem::transmitPhysicalItemTo(TransportSystem* ts, PhysicalItem *pi, float remains){
    m_listPhysicalItem.erase(m_listPhysicalItem.begin());
    ts->addPhysicalItemStart(pi, remains);
}

void TransportSystem::addPhysicalItemStart(PhysicalItem *pi, float remains){
    m_listPhysicalItem.push_back(pi);
    if(m_from == SOUTH){
        pi->setPos(m_posTile.x*Map::s_sizeTileBuildings+Map::s_sizeTileBuildings/2.0-PhysicalItem::s_size/2.0,m_posTile.y*Map::s_sizeTileBuildings+Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0-remains);
    }else if(m_from == NORD){
        pi->setPos(m_posTile.x*Map::s_sizeTileBuildings+Map::s_sizeTileBuildings/2.0-PhysicalItem::s_size/2.0,m_posTile.y*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0+remains);
    }else if(m_from == WEST){
        pi->setPos(m_posTile.x*Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0+remains,m_posTile.y*Map::s_sizeTileBuildings+Map::s_sizeTileBuildings/2.0-PhysicalItem::s_size/2.0);
    }else if(m_from == EAST){
        pi->setPos(m_posTile.x*Map::s_sizeTileBuildings+Map::s_sizeTileBuildings-PhysicalItem::s_size/2.0-remains,m_posTile.y*Map::s_sizeTileBuildings+Map::s_sizeTileBuildings/2.0-PhysicalItem::s_size/2.0);
    }
}

//----------------------------------------------------------
bool TransportSystem::canAcceptItem(ItemStack *is){
    return true;
}
void TransportSystem::transmitItem(ItemStack *is){
    delete m_listPhysicalItem[0];
    m_listPhysicalItem.erase(m_listPhysicalItem.begin());
    m_outputProcessBuilding->receiveItem(is);
    m_outputReady = nullptr;
}
void TransportSystem::receiveItem(ItemStack *is){
    addPhysicalItemStart(new PhysicalItem(m_map,is->getItemTypeWithData(),{0,0}),0);
    delete m_outputReady;
}


void TransportSystem::update(){
    //update sprite
    ProcessBuilding::update();

    //update pi
    for(unsigned i=0;i<m_listPhysicalItem.size();i++){
        Vector2f posItem = m_listPhysicalItem[i]->getPos();

        Vector2f offsetMove;
        //cout << "direction: " << m_from << " " << m_to << endl;
        if(m_from == WEST && m_to == EAST){
            offsetMove = {m_pxlPerTick,0};
        }else if(m_from == EAST && m_to == WEST){
            offsetMove = {-m_pxlPerTick,0};
        }else if(m_from == SOUTH && m_to == NORD){
            offsetMove = {0,-m_pxlPerTick};
        }else if(m_from == NORD && m_to == SOUTH){
            offsetMove = {0,m_pxlPerTick};
        }


        else if(m_from == EAST && m_to == NORD){ //OK
            if(posItem.x > m_posSemiBoundary){
                //first move
                //check to pass into the second move
                if(posItem.x-m_pxlPerTick <= m_posSemiBoundary){
                    float remains = m_posSemiBoundary-(posItem.x-m_pxlPerTick);
                    m_listPhysicalItem[i]->setPosX(m_posSemiBoundary);
                    offsetMove = {0, -remains};
                }else{
                    //stil first move
                    offsetMove = {-m_pxlPerTick,0};
                }
            }else{
                //Second move
                offsetMove = {0,-m_pxlPerTick};
            }
        }

        else if(m_from == NORD && m_to == EAST){ //ok
            if(posItem.y < m_posSemiBoundary){
                //first move
                //check to pass into the second move
                if(posItem.y+m_pxlPerTick >= m_posSemiBoundary){
                    float remains = (posItem.y+m_pxlPerTick)-m_posSemiBoundary;
                    m_listPhysicalItem[i]->setPosY(m_posSemiBoundary);
                    offsetMove = {remains, 0};
                }else{
                    //stil first move
                    offsetMove = {0,m_pxlPerTick};
                }
            }else{
                //Second move
                offsetMove = {m_pxlPerTick,0};
            }
        }else if(m_from == WEST && m_to == NORD){ //ok
            if(posItem.x < m_posSemiBoundary){
                //first move
                //check to pass into the second move
                if(posItem.x+m_pxlPerTick >= m_posSemiBoundary){
                    float remains = (posItem.x+m_pxlPerTick)-m_posSemiBoundary;
                    m_listPhysicalItem[i]->setPosX(m_posSemiBoundary);
                    offsetMove = {0, -remains};
                }else{
                    //stil first move
                    offsetMove = {m_pxlPerTick,0};
                }
            }else{
                //Second move
                offsetMove = {0,-m_pxlPerTick};
            }
        }else if(m_from == NORD && m_to == WEST){ //ok
            if(posItem.y < m_posSemiBoundary){
                //first move
                //check to pass into the second move
                if(posItem.y+m_pxlPerTick >= m_posSemiBoundary){
                    float remains = (posItem.y+m_pxlPerTick)-m_posSemiBoundary;
                    m_listPhysicalItem[i]->setPosY(m_posSemiBoundary);
                    offsetMove = {-remains, 0};
                }else{
                    //stil first move
                    offsetMove = {0,m_pxlPerTick};
                }
            }else{
                //Second move
                offsetMove = {-m_pxlPerTick,0};
            }
        }else if(m_from == SOUTH && m_to == EAST){ //OK
            if(definitelyGreaterThan(posItem.y,m_posSemiBoundary)){
                //first move
                //check to pass into the second move
                if(posItem.y-m_pxlPerTick <= m_posSemiBoundary){
                    float remains = m_posSemiBoundary-(posItem.y-m_pxlPerTick);
                    m_listPhysicalItem[i]->setPosY(m_posSemiBoundary);
                    offsetMove = {remains, 0};
                }else{
                    //stil first move
                    offsetMove = {0,-m_pxlPerTick};
                }
            }else{
                //Second move
                offsetMove = {m_pxlPerTick,0};
            }
        }else if(m_from == EAST && m_to == SOUTH){ //ok
            if(definitelyGreaterThan(posItem.x,m_posSemiBoundary)){
                //first move
                //check to pass into the second move
                if(posItem.x-m_pxlPerTick <= m_posSemiBoundary){
                    float remains = m_posSemiBoundary-(posItem.x-m_pxlPerTick);
                    m_listPhysicalItem[i]->setPosX(m_posSemiBoundary);
                    offsetMove = {0, remains};
                }else{
                    //stil first move
                    offsetMove = {-m_pxlPerTick,0};
                }
            }else{
                //Second move
                offsetMove = {0,m_pxlPerTick};
            }
        }else if(m_from == SOUTH && m_to == WEST){ //ok
            if(posItem.y > m_posSemiBoundary){
                //first move
                //check to pass into the second move
                if(posItem.y-m_pxlPerTick <= m_posSemiBoundary){
                    float remains = m_posSemiBoundary-(posItem.y-m_pxlPerTick);
                    m_listPhysicalItem[i]->setPosY(m_posSemiBoundary);
                    offsetMove = {-remains, 0};
                }else{
                    //stil first move
                    offsetMove = {0,-m_pxlPerTick};
                }
            }else{
                //Second move
                offsetMove = {-m_pxlPerTick,0};
            }
        }else if(m_from == WEST && m_to == SOUTH){ //OK
            if(posItem.x < m_posSemiBoundary){
                //first move
                //check to pass into the second move
                if(posItem.x+m_pxlPerTick >= m_posSemiBoundary){
                    float remains = (posItem.x+m_pxlPerTick)-m_posSemiBoundary;
                    m_listPhysicalItem[i]->setPosX(m_posSemiBoundary);
                    offsetMove = {0, remains};
                }else{
                    //stil first move
                    offsetMove = {m_pxlPerTick,0};
                }
            }else{
                //Second move
                offsetMove = {0,m_pxlPerTick};
            }
        }

        //cout << "offsetMove: " << offsetMove.x << " " << offsetMove.y << endl;

        //update posItem because we modified it before
        posItem = m_listPhysicalItem[i]->getPos();
        Vector2f newPrePosItem = posItem += offsetMove;
        //cout << "newPrePosItem: " << newPrePosItem << endl;

        //if its the first item of the belt -> check the posBoundary
        if(i == 0){
            //check distance with this first of the next ts
            if(m_nextTS){
                //cout << "there is a nextTS" << endl;
                PhysicalItem *nextPi = m_nextTS->getLastPhysicalItem();
                bool canMoveForward = false;
                if(nextPi){
                    //cout << "there is an item on the next belt" << endl;
                    Vector2f posNextItem = nextPi->getPos();
                    //cout << "posNextItem: " << posNextItem.x << " " << posNextItem.y << endl;
                    //cout << "newPrePosItem: " << newPrePosItem.x << " " << newPrePosItem.y << endl;
                    float distance = sqrt((newPrePosItem.x-posNextItem.x)*(newPrePosItem.x-posNextItem.x)+(newPrePosItem.y-posNextItem.y)*(newPrePosItem.y-posNextItem.y));
                    //cout << "distance between this item and the previous one: " << distance << endl;
                    if(distance > PhysicalItem::s_size){
                        canMoveForward = true;
                    }
                }else{
                    canMoveForward = true;
                }
                if(canMoveForward){
                    m_listPhysicalItem[i]->setPos(newPrePosItem);
                    if(m_to == EAST){
                        if(newPrePosItem.x >= m_posBoundary){
                            transmitPhysicalItemTo(m_nextTS, m_listPhysicalItem[i], newPrePosItem.x-m_posBoundary);
                            //m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemType(),1);
                        }

                    }else if(m_to == WEST){
                        if(newPrePosItem.x <= m_posBoundary){
                            transmitPhysicalItemTo(m_nextTS, m_listPhysicalItem[i], m_posBoundary-newPrePosItem.x);
                            //m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemType(),1);
                        }
                    }else if(m_to == NORD){
                        if(newPrePosItem.y <= m_posBoundary){
                            transmitPhysicalItemTo(m_nextTS, m_listPhysicalItem[i], m_posBoundary-newPrePosItem.y);
                            //m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemType(),1);
                        }
                    }else if(m_to == SOUTH){
                        if(newPrePosItem.y >= m_posBoundary){
                            transmitPhysicalItemTo(m_nextTS, m_listPhysicalItem[i], newPrePosItem.y-m_posBoundary);
                            //m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemType(),1);
                        }
                    }
                }
            }else{
                if(m_to == EAST){
                    if(newPrePosItem.x < m_posBoundary){
                        m_listPhysicalItem[i]->setPos(newPrePosItem);
                    }else{
                        m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemTypeWithData(),1);
                    }

                }else if(m_to == WEST){
                    if(newPrePosItem.x > m_posBoundary){
                        m_listPhysicalItem[i]->setPos(newPrePosItem);
                    }else{
                        m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemTypeWithData(),1);
                    }

                }else if(m_to == NORD){
                    if(newPrePosItem.y > m_posBoundary){
                        m_listPhysicalItem[i]->setPos(newPrePosItem);
                    }else{
                        m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemTypeWithData(),1);
                    }

                }else if(m_to == SOUTH){
                    if(newPrePosItem.y < m_posBoundary){
                        m_listPhysicalItem[i]->setPos(newPrePosItem);
                    }else{
                        m_outputReady = new ItemStack(m_listPhysicalItem[i]->getItemTypeWithData(),1);
                    }

                }
            }
        }
        //else check the distance between the previous belt
        else{
            Vector2f posPreviousItem = m_listPhysicalItem[i-1]->getPos();
            //cout << "posPreviousItem: " << posPreviousItem.x << " " << posPreviousItem.y << endl;
            //cout << "newPrePosItem: " << newPrePosItem.x << " " << newPrePosItem.y << endl;
            float distance = sqrt((newPrePosItem.x-posPreviousItem.x)*(newPrePosItem.x-posPreviousItem.x)+(newPrePosItem.y-posPreviousItem.y)*(newPrePosItem.y-posPreviousItem.y));
            //cout << "distance between this item and the previous one: " << distance << endl;
            if(distance > PhysicalItem::s_size){
                m_listPhysicalItem[i]->setPos(newPrePosItem);
            }
        }
    //end for
    }
}
void TransportSystem::draw(sf::RenderWindow &window){
    OrientedBuilding::draw(window);
}
void TransportSystem::drawItem(sf::RenderWindow &window){
    for(unsigned i=0;i<m_listPhysicalItem.size();i++){
        m_listPhysicalItem[i]->draw(window);
    }
}
