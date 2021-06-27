#include "ItemContainer.h"

using namespace std;
using namespace sf;

ItemContainer::ItemContainer(unsigned nbRow, sf::Vector2f posScreen, unsigned maxItemStack){
    m_maxItemStack = maxItemStack;

    for(unsigned i=0;i<m_maxItemStack;i++){
        m_listItemStack.push_back(nullptr);
    }


    m_posScreen = posScreen;

    m_nbRow = nbRow;
    m_nbCol = ceil((float)m_maxItemStack/m_nbRow);
    m_sizeBoxItem = {64,64};

    m_spriteDarkHand.setTexture(*DataLoader::getTextureDarkHand());
    m_spriteDarkHand.setScale(m_sizeBoxItem.x/m_spriteDarkHand.getLocalBounds().width, m_sizeBoxItem.y/m_spriteDarkHand.getLocalBounds().height);
    m_movingItem = false;
    m_saveDoubleItemStack = nullptr;
    m_saveItemStackOnHand = nullptr;
    m_saveIndexClick = 0;

    m_paddding = 10;

    initRectStyle(m_rectBackground);
    m_rectBackground.setSize({m_nbCol*(m_sizeBoxItem.x+m_paddding)+m_paddding, m_nbRow*(m_sizeBoxItem.y+m_paddding)+m_paddding});
    m_rectBackground.setPosition(m_posScreen);


    for (int i = 0; i < m_maxItemStack; ++i) {
        m_listBgItem.push_back(RectangleShape());
        initRectStyleBis(m_listBgItem[i]);
        m_listBgItem[i].setSize({m_sizeBoxItem.x, m_sizeBoxItem.y});
        m_listBgItem[i].setPosition(m_posScreen.x+m_paddding+(i%m_nbCol)*(m_sizeBoxItem.x+m_paddding), m_posScreen.y+m_paddding+(i/m_nbCol)*(m_sizeBoxItem.y+m_paddding));
    }

    //addItem(DataLoader::getItemType(0), 152);
    //addItem(DataLoader::getItemType(1), 241);

    addItem(DataLoader::getItemType(1), 5);
    addItem(DataLoader::getItemType(5), 10);

    //addItem(DataLoader::getItemType(4), 4, {new QualityDataItem(NORMAL)});
}
ItemContainer::~ItemContainer(){
    for(int i=0;i<m_listItemStack.size();i++){
        delete m_listItemStack[i];
    }
}

void ItemContainer::setMaxItemStack(unsigned maxItemStack){
    if(maxItemStack == m_maxItemStack) return;

    if(maxItemStack > m_maxItemStack){
        unsigned toAdd = maxItemStack-m_maxItemStack;
        for(unsigned i=0;i<toAdd;i++){
            m_listItemStack.push_back(nullptr);

            short index = m_listBgItem.size();
            m_listBgItem.push_back(RectangleShape());
            initRectStyleBis(m_listBgItem[index]);
            m_listBgItem[index].setSize({m_sizeBoxItem.x, m_sizeBoxItem.y});
        }
    }else {
        unsigned toRemove = m_maxItemStack-maxItemStack;
        for(unsigned i=m_listItemStack.size()-1;i>=m_maxItemStack-toRemove;i--){
            delete m_listItemStack[i];
            m_listItemStack.pop_back();
            m_listBgItem.pop_back();
        }
    }

    m_maxItemStack = maxItemStack;

    m_nbCol = ceil((float)m_maxItemStack/m_nbRow);
    m_rectBackground.setSize({m_nbCol*(m_sizeBoxItem.x+m_paddding)+m_paddding, m_nbRow*(m_sizeBoxItem.y+m_paddding)+m_paddding});

    for (int i = 0; i < m_maxItemStack; ++i) {
        m_listBgItem[i].setPosition(m_posScreen.x+m_paddding+(i%m_nbCol)*(m_sizeBoxItem.x+m_paddding), m_posScreen.y+m_paddding+(i/m_nbCol)*(m_sizeBoxItem.y+m_paddding));
    }
    
    updatePosItem();
}

void ItemContainer::updatePosItem(){
    for(unsigned i=0;i<m_listItemStack.size();i++){
        Vector2f pos = {m_posScreen.x+m_paddding+(i%m_nbCol)*(m_sizeBoxItem.x+m_paddding), m_posScreen.y+m_paddding+(i/m_nbCol)*(m_sizeBoxItem.y+m_paddding)};
        if(m_listItemStack[i])
            m_listItemStack[i]->setPos(pos);
    }
}

bool ItemContainer::canAddItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem){
    bool ret;
    ItemStack * is = new ItemStack(new ItemTypeWithData(itemType,listDataItem),nb);
    ret = canAddItem(is);
    delete is;
    return ret;
}
bool ItemContainer::canAddItem(ItemStack *is){
    unsigned nb = is->getNbItem();
    unsigned maxItemPerStack = is->getItemTypeWithData()->getItemType()->getMaxStack();
    short i = 0;
    unsigned nbCanAdd=0;

    while(nbCanAdd < nb && i < m_listItemStack.size()){
        if(m_listItemStack[i]){
            if(m_listItemStack[i]->isStackableWith(is)){
                nbCanAdd += maxItemPerStack-m_listItemStack[i]->getNbItem();
            }
        }else{
            nbCanAdd += maxItemPerStack;
        }
        i++;
    }

    if(nbCanAdd >= nb){
        return true;
    }else{
        return false;
    }
}

void ItemContainer::addItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem){
    ItemStack * is = new ItemStack(new ItemTypeWithData(itemType,listDataItem),nb);
    addItem(is);
    delete is;
}
void ItemContainer::addItem(ItemStack *is){
    unsigned maxItemPerStack = is->getItemTypeWithData()->getItemType()->getMaxStack();
    unsigned nb = is->getNbItem();
    short i = 0;

    while(nb > 0 && i < m_listItemStack.size()){
        if(m_listItemStack[i] != nullptr && m_listItemStack[i]->isStackableWith(is)){
            unsigned nbItem = m_listItemStack[i]->getNbItem();
            unsigned place = maxItemPerStack-nbItem;
            if(place >= nb){
                m_listItemStack[i]->setNbItem(nbItem+nb);
                nb = 0;
            }else{
                m_listItemStack[i]->setNbItem(maxItemPerStack);
                nb -= place;
            }
            if(m_listItemStack[i] == m_saveItemStackOnHand){
                m_saveDoubleItemStack->setNbItem(m_saveItemStackOnHand->getNbItem());
            }
        }
        i++;
    }

    i=0;
    while(nb > 0 && i < m_listItemStack.size()){
        if(m_listItemStack[i] == nullptr){
            Vector2f pos = {m_posScreen.x+m_paddding+(i%m_nbCol)*(m_sizeBoxItem.x+m_paddding), m_posScreen.y+m_paddding+(i/m_nbCol)*(m_sizeBoxItem.y+m_paddding)};
            ItemTypeWithData* itemTypeWithData = new ItemTypeWithData(is->getItemTypeWithData());
            if(nb <= maxItemPerStack){
                m_listItemStack[i] = new DrawableItemStack(itemTypeWithData, m_sizeBoxItem, pos, nb);
                nb = 0;
            }else{
                m_listItemStack[i] = new DrawableItemStack(itemTypeWithData, m_sizeBoxItem, pos, maxItemPerStack);
                nb -= maxItemPerStack;
            }
        }
        i++;
    }
}

void ItemContainer::removeItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem){
    ItemStack * is = new ItemStack(new ItemTypeWithData(itemType,listDataItem),nb);
    removeItem(is);
    delete is;
}

void ItemContainer::removeItem(ItemStack *is){

    unsigned nb = is->getNbItem();
    unsigned i=0;
    while(nb > 0 && i<m_listItemStack.size()){
        if(m_listItemStack[i] != nullptr && m_listItemStack[i]->isStackableWith(is)){
            unsigned nbItemStack = m_listItemStack[i]->getNbItem();
            if(nbItemStack >= nb){
                int newNbItem = nbItemStack-nb;
                if(newNbItem == 0){

                    if(m_movingItem && m_listItemStack[i] == m_saveItemStackOnHand){
                        m_movingItem = false;
                        delete m_saveDoubleItemStack;
                        m_saveDoubleItemStack = nullptr;
                        m_saveItemStackOnHand = nullptr;
                    }

                    delete m_listItemStack[i];
                    m_listItemStack[i] = nullptr;

                }else{
                    m_listItemStack[i]->setNbItem(nbItemStack-nb);
                    if(m_listItemStack[i] == m_saveItemStackOnHand){
                        m_saveDoubleItemStack->setNbItem(m_saveItemStackOnHand->getNbItem());
                    }
                }
                nb = 0;
            }else{
                //remove the itemStack
                nb -= nbItemStack;

                if(m_movingItem && m_listItemStack[i] == m_saveItemStackOnHand){
                    m_movingItem = false;
                    delete m_saveDoubleItemStack;
                    m_saveDoubleItemStack = nullptr;
                    m_saveItemStackOnHand = nullptr;
                }

                delete m_listItemStack[i];
                m_listItemStack[i] = nullptr;

            }
        }
        i++;
    }
}

bool ItemContainer::hasItem(ItemType *itemType, unsigned nb, const std::vector<DataItem*> &listDataItem){
    bool ret;
    ItemStack * is = new ItemStack(new ItemTypeWithData(itemType,listDataItem),nb);
    ret = hasItem(is);
    delete is;
    return ret;
}
bool ItemContainer::hasItem(ItemStack *is){
    unsigned nb = is->getNbItem();
    unsigned short nbTot=0;
    for(unsigned i=0;i<m_listItemStack.size();i++){
        if(m_listItemStack[i] != nullptr && m_listItemStack[i]->isStackableWith(is)){
            nbTot += m_listItemStack[i]->getNbItem();
        }
    }
    return (nbTot >= nb);
}

ItemStack* ItemContainer::getFirstItemStack() const{
    ItemStack *ret = nullptr;

    for(unsigned i=0;i<m_listItemStack.size();i++){
        if(m_listItemStack[i]){
            ret = m_listItemStack[i];
            break;
        }
    }

    return ret;
}

void ItemContainer::clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent, const sf::Vector2f &cooMap, const sf::Vector2i &cooScreen){
    if(mouseButtonEvent.button == sf::Mouse::Button::Left){
        bool pressed = Mouse::isButtonPressed(sf::Mouse::Button::Left);
        if(pressed){
            for (unsigned i = 0; i < m_listItemStack.size(); ++i) {
                if(m_listItemStack[i] && m_listItemStack[i]->mouseIsOn(cooScreen)){
                    m_saveDoubleItemStack = new DrawableItemStack(m_listItemStack[i]);
                    m_saveItemStackOnHand = m_listItemStack[i];
                    m_saveDoubleItemStack->setPos({(float)cooScreen.x, (float)cooScreen.y});
                    m_spriteDarkHand.setPosition(m_listItemStack[i]->getPos());
                    m_movingItem = true;
                    m_saveIndexClick = i;
                    break;
                }
            }
        }else{
            if(m_movingItem){
                m_movingItem = false;
                bool onItem = false;
                for (unsigned i = 0; i < m_listItemStack.size(); ++i) {
                    if (m_listItemStack[i] && m_listItemStack[i]->mouseIsOn(cooScreen)) {
                        DrawableItemStack *save = m_listItemStack[m_saveIndexClick];
                        m_listItemStack[m_saveIndexClick] = m_listItemStack[i];
                        m_listItemStack[i] = save;

                        updatePosItem();

                        delete m_saveDoubleItemStack;
                        m_saveDoubleItemStack = nullptr;
                        m_saveItemStackOnHand = nullptr;

                        onItem = true;

                        break;
                    }
                }
                if (!onItem) {
                    delete m_saveDoubleItemStack;
                    m_saveDoubleItemStack = nullptr;
                    m_saveItemStackOnHand = nullptr;
                }
            }
        }
    }
}
void ItemContainer::mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick){
    m_saveCooScreen = cooScreen;

    if(m_saveDoubleItemStack){
        m_saveDoubleItemStack->setPos({(float)cooScreen.x, (float)cooScreen.y});
    }

    for (unsigned i = 0; i < m_listItemStack.size(); ++i) {
        if(m_listItemStack[i] && m_listItemStack[i]->mouseIsOn(cooScreen)){
            m_listItemStack[i]->setPosUI(cooScreen);
        }
    }
}

void ItemContainer::drawInfo(sf::RenderWindow &window){
    window.draw(m_rectBackground);

    for(unsigned i=0;i<m_listBgItem.size();i++){
        window.draw(m_listBgItem[i]);
    }

    for(unsigned i=0;i<m_listItemStack.size();i++){
        if(m_listItemStack[i]){
            m_listItemStack[i]->draw(window);
        }
    }

    if(m_movingItem){
        window.draw(m_spriteDarkHand);
        if(m_saveDoubleItemStack){
            m_saveDoubleItemStack->draw(window);
        }
    }

    for(unsigned i=0;i<m_listItemStack.size();i++){
        if(!m_saveDoubleItemStack && m_listItemStack[i] && m_listItemStack[i]->mouseIsOn(m_saveCooScreen)){
            m_listItemStack[i]->drawScreenPosUI(window);
        }
    }
}
