#include "Drill1.h"

using namespace std;
using namespace sf;
using namespace rapidjson;

Drill1::Drill1(Map *map, sf::Clock *clock, sf::Vector2i posTile, QualityEnum quality):
ProcessBuilding(map, clock, posTile, DataLoader::getBuildingType(3),false,true),
Building(map,clock,posTile,DataLoader::getBuildingType(3))
{
    setQuality(quality);

    m_previousTimeProd = m_clock->getElapsedTime();

    m_indexToProduce = 0;
    m_canProcess = false;

    initRectStyle(m_bg);
    m_bg.setSize({400, 200});
    m_bg.setPosition(500, 200);

    initRectStyleBis(m_bgSprite);
    m_bgSprite.setSize({64,64});
    m_bgSprite.setPosition(m_bg.getPosition().x + m_bg.getSize().x - m_bgSprite.getSize().x - 10, m_bg.getPosition().y + m_bg.getSize().y / 4.0 - m_bgSprite.getSize().y / 2.0);

    m_spriteItemProduced.setPosition(m_bgSprite.getPosition());
    m_spriteItemProduced.setColor(sf::Color(255,255,255,150));

    initRectStyleBis(m_bgOutput);
    m_bgOutput.setSize({64,64});
    m_bgOutput.setPosition(m_bg.getPosition().x + m_bg.getSize().x - m_bgSprite.getSize().x - 10, m_bg.getPosition().y + m_bg.getSize().y - m_bg.getSize().y / 4.0 - m_bgSprite.getSize().y / 2.0);

    m_bgBarre.setFillColor(sf::Color(50,50,50));
    m_bgBarre.setSize({m_bg.getSize().x - m_bgSprite.getSize().x - 30, 20});
    m_bgBarre.setPosition(m_bgSprite.getPosition().x-10-m_bgBarre.getSize().x, m_bgSprite.getPosition().y+m_bgSprite.getSize().y/2-m_bgBarre.getSize().y/2);

    m_barre.setFillColor(sf::Color(255,0,0));
    m_barre.setSize({0,m_bgBarre.getSize().y});
    m_barre.setPosition(m_bgBarre.getPosition());

    initTextStyle(m_text, DataLoader::getFont(), 20);
    m_text.setString("can't produce");
    m_text.setPosition(m_bgBarre.getPosition().x+m_bgBarre.getSize().x/2-m_text.getGlobalBounds().width/2, m_bgBarre.getPosition().y+m_bgBarre.getSize().y+5);
}
Drill1::~Drill1() = default;

void Drill1::setListItemTypeToProduce(std::vector<ItemType*> listItemTypeToProduce){
    m_listItemTypeToProduce = listItemTypeToProduce;
    //cout << "size m_listItemTypeToProduce: " << m_listItemTypeToProduce.size() << endl;
    if(m_listItemTypeToProduce.size()==0){
        stopSpriteProcessing();
    }else{
        m_canProcess = true;

        m_msItemProduct = seconds(m_listItemTypeToProduce[m_indexToProduce]->getNbSecCollectTime()/qualityToMult(m_quality));

        m_spriteItemProduced.setTexture(*m_listItemTypeToProduce[m_indexToProduce]->getTexture());
        m_spriteItemProduced.setScale(m_bgSprite.getSize().x/m_spriteItemProduced.getLocalBounds().width, m_bgSprite.getSize().y/m_spriteItemProduced.getLocalBounds().height);

        addStringTextinfo("\nItem produced : ");
        for(unsigned i=0;i<m_listItemTypeToProduce.size();i++){
            addStringTextinfo("\n-"+m_listItemTypeToProduce[i]->getItemName()+" : "+to_string(m_listItemTypeToProduce[i]->getNbSecCollectTime()/qualityToMult(m_quality))+" sec");
        }
    }
}

bool Drill1::canAcceptItem(ItemStack *is){
    return false;
}
void Drill1::transmitItem(ItemStack *is){
    m_outputProcessBuilding->receiveItem(is);
    m_outputReady = nullptr;
}
void Drill1::receiveItem(ItemStack *is){

}

void Drill1::update(){
    ProcessBuilding::update();

    if(m_canProcess){
        m_actualTimeProd = m_clock->getElapsedTime();
        m_elapsedTimeProd += m_actualTimeProd - m_previousTimeProd;
        m_previousTimeProd = m_actualTimeProd;


        if (m_spriteProcessing) {
            m_text.setString(timeToStr(m_msItemProduct-m_elapsedTimeProd));
            m_text.setPosition(m_bgBarre.getPosition().x+m_bgBarre.getSize().x/2-m_text.getGlobalBounds().width/2, m_bgBarre.getPosition().y+m_bgBarre.getSize().y+5);
            m_barre.setSize({m_elapsedTimeProd.asMilliseconds()*m_bgBarre.getSize().x/m_msItemProduct.asMilliseconds(),m_bgBarre.getSize().y});
        }

        while (m_elapsedTimeProd >= m_msItemProduct) {
            m_elapsedTimeProd -= m_msItemProduct;
            if (!m_outputReady) {
                //cout << "item ready : " << m_listItemTypeToProduce[m_indexToProduce]->getItemName() << endl;
                ItemTypeWithData *itemTypeWithData = new ItemTypeWithData(m_listItemTypeToProduce[m_indexToProduce]);
                m_outputReady = new DrawableItemStack(itemTypeWithData,{64,64},m_bgOutput.getPosition(), 1);

                //index item produced
                m_indexToProduce++;
                if(m_indexToProduce == m_listItemTypeToProduce.size()){
                    m_indexToProduce = 0;
                }

                m_msItemProduct = seconds(m_listItemTypeToProduce[m_indexToProduce]->getNbSecCollectTime()/qualityToMult(m_quality));

                m_spriteItemProduced.setTexture(*m_listItemTypeToProduce[m_indexToProduce]->getTexture());
                m_spriteItemProduced.setScale(m_bgSprite.getSize().x/m_spriteItemProduced.getLocalBounds().width, m_bgSprite.getSize().y/m_spriteItemProduced.getLocalBounds().height);

                //update processing
                if (!m_spriteProcessing) {
                    startSpriteProcessing();
                }
            } else {
                stopSpriteProcessing();
                m_text.setString("can't produce");
            }
        }
    }
}

void Drill1::drawScreenPosUI(sf::RenderWindow &window){
    ProcessBuilding::drawScreenPosUI(window);

    window.draw(m_bg);

    window.draw(m_bgSprite);
    window.draw(m_spriteItemProduced);

    window.draw(m_bgOutput);

    window.draw(m_bgBarre);
    window.draw(m_barre);

    window.draw(m_text);

    if(m_outputReady)
        m_outputReady->draw(window);
}


