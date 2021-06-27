//
// Created by natha on 31/10/2020.
//

#include "RecipeProcessing.h"

using namespace std;
using namespace sf;

RecipeProcessing::RecipeProcessing(sf::Clock* clock, bool canProduceElectricity){
    m_clock = clock;

    m_canProduceElectricity = canProduceElectricity;
}
RecipeProcessing::~RecipeProcessing(){

}
void RecipeProcessing::initLate(unsigned nbInputIngre, float divTime){
    m_divTime = divTime;

    m_nbInputIngre = nbInputIngre;
    m_nbOutputIngre = RecipeManager::getMaxOuput(m_nbInputIngre);
    //cout << "nbOutputIngre: " << m_nbOutputIngre << endl;

    for(unsigned i = 0; i < m_nbInputIngre; i++){
        m_listItemIngredient.push_back(nullptr);
    }

    m_isProducingElectricity = false;
    m_powerProduced = Power();

    m_flushing = false;

    m_targetRecipe = nullptr;
    m_targetRecipeSave = nullptr;

    for(unsigned i = 0; i < m_nbOutputIngre; i++){
        m_listOutput.push_back(nullptr);
    }

    //--------------GRAPHICS------------------------

    for(int i=0; i < m_nbInputIngre; i++){
        m_listBgInput.push_back(RectangleShape({64,64}));
        initRectStyleBis(m_listBgInput[i]);
    }

    initRectStyle(m_bgInfo);
    m_bgInfo.setSize({500,std::max(std::max(m_nbInputIngre,m_nbOutputIngre)*(m_listBgInput[0].getSize().y+10),230.f)+10});
    m_bgInfo.setPosition(500,200);

    for(int i=0; i < m_nbInputIngre; i++){
        m_listBgInput[i].setPosition(m_bgInfo.getPosition().x+10, m_bgInfo.getPosition().y+10+i*(m_listBgInput[i].getSize().y+10));
    }

    for(int i=0; i < m_nbOutputIngre; i++){
        m_listBgSprite.push_back(RectangleShape(m_listBgInput[0].getSize()));
        initRectStyleBis(m_listBgSprite[i]);
        m_listBgSprite[i].setPosition(m_bgInfo.getPosition().x+m_bgInfo.getSize().x-m_listBgSprite[i].getSize().x*2-20, m_bgInfo.getPosition().y+10+i*(m_listBgSprite[i].getSize().y+10));
    }

    for(int i=0; i < m_nbOutputIngre; i++){
        m_listSpriteItemProduced.push_back(Sprite());
        m_listSpriteItemProduced[i].setPosition(m_listBgSprite[i].getPosition());
        m_listSpriteItemProduced[i].setColor(sf::Color(255,255,255,150));
    }

    initRectStyleBis(m_bgOutput);
    m_bgOutput.setSize(m_listBgInput[0].getSize());
    m_bgOutput.setPosition(m_bgInfo.getPosition().x+m_bgInfo.getSize().x-m_bgOutput.getSize().x-10, m_bgInfo.getPosition().y+m_bgInfo.getSize().y-m_bgOutput.getSize().y-10);

    m_bgBarre.setFillColor(sf::Color(50,50,50));
    m_bgBarre.setSize({(m_bgInfo.getPosition().x+m_bgInfo.getSize().x-m_listBgInput[0].getSize().x*2-30)-(m_listBgInput[0].getPosition().x+m_listBgInput[0].getSize().x+10), 20});
    m_bgBarre.setPosition(m_listBgInput[0].getPosition().x+m_listBgInput[0].getSize().x+10, m_bgInfo.getPosition().y+m_bgInfo.getSize().y/2-m_bgBarre.getSize().y/2);

    m_barre.setFillColor(sf::Color(255,0,0));
    m_barre.setSize({0,m_bgBarre.getSize().y});
    m_barre.setPosition(m_bgBarre.getPosition());

    m_flush.setPosition(m_barre.getPosition().x, m_barre.getPosition().y+m_barre.getSize().y+10);
    m_flush.setTexture(*DataLoader::getTextureFlush());
    m_flush.setScale(m_bgOutput.getSize().x/m_flush.getLocalBounds().width, m_bgOutput.getSize().y/m_flush.getLocalBounds().height);

    m_indicFlush.setPosition(m_flush.getPosition().x, m_flush.getPosition().y+m_flush.getGlobalBounds().height+10);
    m_indicFlush.setSize({10,10});
    m_indicFlush.setFillColor(sf::Color::Red);

    initTextStyle(m_text, DataLoader::getFont(), 22);

    //--------------END GRAPHICS------------------
    stopProducing();
}


bool RecipeProcessing::mouseIsOnBarre() {
    return (m_savePosScreen.x >= m_bgBarre.getPosition().x
            && m_savePosScreen.x <= m_bgBarre.getPosition().x+m_bgBarre.getGlobalBounds().width
            && m_savePosScreen.y >= m_bgBarre.getPosition().y
            && m_savePosScreen.y <= m_bgBarre.getPosition().y+m_bgBarre.getGlobalBounds().height);
}


bool RecipeProcessing::canAcceptItem(ItemStack *is){
    bool ok = true;
    unsigned nbNotSameItemType = 0;
    for(unsigned i = 0; i < m_nbInputIngre; i++){
        if(m_listItemIngredient[i]){
            if(m_listItemIngredient[i]->isStackableWith(is)){
                if(is->getNbItem()+m_listItemIngredient[i]->getNbItem() <= is->getItemTypeWithData()->getItemType()->getMaxStack()) {
                    ok = true;
                }else{
                    ok = false;
                }
                break;
            }else{
                nbNotSameItemType++;
            }
        }
    }
    if(nbNotSameItemType == m_nbInputIngre){
        ok = false;
    }
    return ok;
}

void RecipeProcessing::addIngredient(ItemStack *is) {
    int index = -1; //where to put the is
    for(unsigned i = 0; i < m_nbInputIngre; i++){
        if(m_listItemIngredient[i]){
            if(m_listItemIngredient[i]->isStackableWith(is)){
                index = i;
                break;
            }
        }
    }
    if(index == -1){
        //create new DrawableItemStack
        for(unsigned i = 0; i < m_nbInputIngre; i++){
            if(!m_listItemIngredient[i]){
                m_listItemIngredient[i] = new DrawableItemStack(is, m_listBgInput[i].getSize(), m_listBgInput[i].getPosition());
                break;
            }
        }
        //cout << "nbIngredient: " << getNbIngredient() << endl;
        if(getNbIngredient() == m_nbInputIngre){
            RecipeManager::clear();
            for (unsigned i = 0; i < m_listItemIngredient.size(); ++i) {
                RecipeManager::addItemTypeWithDataForLate(m_listItemIngredient[i]->getItemTypeWithData());
            }
            Recipe *r = RecipeManager::getRecipeLate();
            bool producesElec = r->producesElectricity();
            if(r) {
                if(producesElec == m_canProduceElectricity) {
                    //cout << "new recipe detected: " << *m_targetRecipe << endl;
                    m_targetRecipe = r;

                    vector<ItemStack *> listResult = m_targetRecipe->getListResult();

                    unsigned i;
                    for (i = 0; i < listResult.size(); ++i) {
                        m_listSpriteItemProduced[i].setTexture(
                                *listResult[i]->getItemTypeWithData()->getItemType()->getTexture());
                        m_listSpriteItemProduced[i].setScale(
                                m_listBgSprite[i].getSize().x / m_listSpriteItemProduced[i].getLocalBounds().width,
                                m_listBgSprite[i].getSize().y / m_listSpriteItemProduced[i].getLocalBounds().height);
                    }

                    if (producesElec) {
                        m_isProducingElectricity = true;
                        unsigned amountElectricityProduced = m_targetRecipe->getAmountElectricity();
                        m_powerProduced = Power(amountElectricityProduced);

                        m_listSpriteItemProduced[i].setTexture(*DataLoader::getTextureLightning());
                        m_listSpriteItemProduced[i].setScale(
                                m_listBgSprite[i].getSize().x / m_listSpriteItemProduced[i].getLocalBounds().width,
                                m_listBgSprite[i].getSize().y / m_listSpriteItemProduced[i].getLocalBounds().height);
                    }

                    m_msItemProduct = seconds(m_targetRecipe->getProductionTime() / m_divTime);
                }
            }
        }
    }else{
        //add number
        m_listItemIngredient[index]->setNbItem(m_listItemIngredient[index]->getNbItem()+is->getNbItem());
    }
}

void RecipeProcessing::setOutput() {
    vector<ItemStack*> listResult = m_targetRecipeSave->getListResult();
    for (unsigned i = 0; i < listResult.size(); ++i) {
        m_listOutput[i] = new DrawableItemStack(listResult[i], {64, 64}, m_listBgSprite[i].getPosition());
    }
}

bool RecipeProcessing::hasAnOutput() {
    for(int i=0; i < m_nbOutputIngre; i++){
        if(m_listOutput[i]){
            return true;
        }
    }
    return false;
}

bool RecipeProcessing::isAnIngredient(ItemStack *is) {
    for (unsigned i = 0; i < m_listItemIngredient.size(); ++i) {
        if(m_listItemIngredient[i] && m_listItemIngredient[i]->isStackableWith(is))
            return true;
    }
    return false;
}

bool RecipeProcessing::isAnOutput(ItemStack *is) {
    for (unsigned i = 0; i < m_listOutput.size(); ++i) {
        if(m_listOutput[i] && m_listOutput[i]->isStackableWith(is))
            return true;
    }
    return false;
}

unsigned int RecipeProcessing::getNbIngredient() {
    unsigned nb = 0;
    for(unsigned i=0;i<m_listItemIngredient.size();i++){
        if(m_listItemIngredient[i]){
            nb++;
        }
    }
    return nb;
}

ItemStack *RecipeProcessing::getFirstItemStackIngredient() {
    for(unsigned i=0;i<m_listItemIngredient.size();i++){
        if(m_listItemIngredient[i]){
            return m_listItemIngredient[i];
        }
    }
    return nullptr;
}
ItemStack* RecipeProcessing::getFirstItemStackResult(){
    for(unsigned i=0;i<m_listOutput.size();i++){
        if(m_listOutput[i]){
            return m_listOutput[i];
        }
    }
    return nullptr;
}

void RecipeProcessing::removeItemStackIngredient(ItemStack *itemStack) {
    int nbToRemove = itemStack->getNbItem();
    unsigned i = 0;
    while(nbToRemove > 0 && i < m_listItemIngredient.size()){
        if(m_listItemIngredient[i] && m_listItemIngredient[i]->isStackableWith(itemStack)){
            if(m_listItemIngredient[i]->getNbItem() > nbToRemove){
                m_listItemIngredient[i]->setNbItem(m_listItemIngredient[i]->getNbItem()-nbToRemove);
                nbToRemove = 0;
            }else{ // m_listItemIngredient[i]->getNbItem() <= nbToRemove
                nbToRemove -= m_listItemIngredient[i]->getNbItem();
                delete m_listItemIngredient[i];
                m_listItemIngredient[i] = nullptr;

                //cout << "targetRecipe removed" << endl;
                m_targetRecipe = nullptr;
            }
        }
        i++;
    }
}

void RecipeProcessing::removeItemStackOutput(ItemStack *itemStack) {
    int nbToRemove = itemStack->getNbItem();
    unsigned i = 0;
    while(nbToRemove > 0 && i < m_listOutput.size()){
        if(m_listOutput[i] && m_listOutput[i]->isStackableWith(itemStack)){
            if(m_listOutput[i]->getNbItem() > nbToRemove){
                m_listOutput[i]->setNbItem(m_listOutput[i]->getNbItem()-nbToRemove);
                nbToRemove = 0;
            }else{ // m_listItemIngredient[i]->getNbItem() <= nbToRemove
                nbToRemove -= m_listOutput[i]->getNbItem();
                //cout << "Delete an item output: " << i << endl;
                delete m_listOutput[i];
                m_listOutput[i] = nullptr;
            }
        }
        i++;
    }
}

void RecipeProcessing::startProducing(){
    //cout << "start produing" << endl;
    m_producing = true;
    m_previousTimeProd = m_clock->getElapsedTime();

    m_targetRecipeSave = m_targetRecipe;

    if(m_targetRecipeSave->producesElectricity()){
        m_isProducingElectricity = true;
    }

    //remove ingre
    //cout << "remove ingre" << endl;
    vector<ItemStack*> listNeededIngre = m_targetRecipeSave->getListIngre();
    for (unsigned i = 0; i < listNeededIngre.size(); ++i) {
        removeItemStackIngredient(listNeededIngre[i]);
    }

}

void RecipeProcessing::stopProducing(){
    //cout << "stop producing" << endl;
    m_producing = false;

    m_isProducingElectricity = false;

    m_barre.setSize({0,m_barre.getSize().y});

    m_text.setString("recipe time");
    m_text.setPosition(m_bgBarre.getPosition().x+m_bgBarre.getSize().x-m_text.getGlobalBounds().width, m_bgBarre.getPosition().y+m_bgBarre.getSize().y+5);
}

bool RecipeProcessing::mouseOnFlushButton(const Vector2i &cooScreen) {
    return (cooScreen.x >= m_flush.getPosition().x &&
            cooScreen.y >= m_flush.getPosition().y &&
            cooScreen.x <= m_flush.getPosition().x+m_flush.getGlobalBounds().width &&
            cooScreen.y <= m_flush.getPosition().y+m_flush.getGlobalBounds().height);
}

void RecipeProcessing::clickFlush(){
    if(m_flushing){
        m_flushing = false;
        m_indicFlush.setFillColor(sf::Color::Red);
    }else{
        m_flushing = true;
        m_indicFlush.setFillColor(sf::Color::Green);
    }
}

void RecipeProcessing::clickHandler(const Event::MouseButtonEvent &mouseButtonEvent, const Vector2f &cooMap, const Vector2i &cooScreen) {
    if(mouseButtonEvent.button == sf::Mouse::Button::Left){
        bool pressed = Mouse::isButtonPressed(sf::Mouse::Button::Left);
        if(pressed){
            if(mouseOnFlushButton(cooScreen)){
                clickFlush();
            }
        }
    }
}

void RecipeProcessing::mouseMoveHandler(const sf::Vector2i &cooScreen, const sf::Vector2f &cooMap, bool leftClick, bool rightClick){
    m_savePosScreen = cooScreen;
    for (unsigned i = 0; i < m_listItemIngredient.size(); ++i) {
        if(m_listItemIngredient[i] && m_listItemIngredient[i]->mouseIsOn(cooScreen)){
            m_listItemIngredient[i]->setPosUI(cooScreen);
        }
    }
    for (unsigned i = 0; i < m_listOutput.size(); ++i) {
        if(m_listOutput[i] && m_listOutput[i]->mouseIsOn(cooScreen)){
            m_listOutput[i]->setPosUI(cooScreen);
        }
    }

    if(mouseIsOnBarre()) {
        if (m_targetRecipeSave) {
            m_targetRecipeSave->setPosUI(cooScreen);
        }
        if (m_targetRecipe) {
            m_targetRecipe->setPosUI(cooScreen);
        }
    }
}

void RecipeProcessing::update(){
    if(m_producing){
        m_actualTimeProd = m_clock->getElapsedTime();
        m_elapsedTimeProd += m_actualTimeProd - m_previousTimeProd;
        m_previousTimeProd = m_actualTimeProd;

        m_text.setString(timeToStr(m_msItemProduct-m_elapsedTimeProd));
        m_text.setPosition(m_bgBarre.getPosition().x+m_bgBarre.getSize().x-m_text.getGlobalBounds().width, m_bgBarre.getPosition().y+m_bgBarre.getSize().y+5);

        m_barre.setSize({m_elapsedTimeProd.asMilliseconds()*m_bgBarre.getSize().x/m_msItemProduct.asMilliseconds(),m_bgBarre.getSize().y});

        if (m_elapsedTimeProd >= m_msItemProduct) {
            m_elapsedTimeProd = seconds(0);

            if (!hasAnOutput()) {
                setOutput();
            }

            stopProducing();
        }
    }

    if(!m_flushing){
        if(!m_producing){
            if(m_targetRecipe){
                //test nb ingre ok
                bool ok = true;
                for(unsigned i=0;i<m_listItemIngredient.size();i++){
                    if(m_listItemIngredient[i]->getNbItem() < m_targetRecipe->getNbIngreForType(m_listItemIngredient[i]->getItemTypeWithData()->getItemType())){
                        ok = false;
                        break;
                    }
                }
                if(ok){ //nb ingre ok
                    if(!hasAnOutput()) { //ouput empty
                        startProducing();
                    }
                }
            }
        }
    }
}

void RecipeProcessing::drawScreenPosUI(sf::RenderWindow &window){
    window.draw(m_bgInfo);

    for(int i=0; i < m_nbInputIngre; i++){
        window.draw(m_listBgInput[i]);
    }

    for(int i=0; i < m_nbOutputIngre; i++){
        window.draw(m_listBgSprite[i]);
    }

    if(m_targetRecipe && !hasAnOutput()){
        for(int i=0; i < m_nbOutputIngre; i++){
            window.draw(m_listSpriteItemProduced[i]);
        }
    }else if(hasAnOutput()){
        for(int i=0; i < m_nbOutputIngre; i++){
            if(m_listOutput[i]){
                m_listOutput[i]->draw(window);
            }
        }
    }

    window.draw(m_bgOutput);

    window.draw(m_bgBarre);
    window.draw(m_barre);

    window.draw(m_text);

    window.draw(m_flush);
    window.draw(m_indicFlush);

    for(unsigned i = 0; i < m_nbInputIngre; i++){
        if(m_listItemIngredient[i]){
            m_listItemIngredient[i]->draw(window);
        }
    }

    //-----------------------------------------------

    for (unsigned i = 0; i < m_listItemIngredient.size(); ++i) {
        if(m_listItemIngredient[i] && m_listItemIngredient[i]->mouseIsOn(m_savePosScreen)){
            m_listItemIngredient[i]->drawScreenPosUI(window);
        }
    }
    for (unsigned i = 0; i < m_listOutput.size(); ++i) {
        if(m_listOutput[i] && m_listOutput[i]->mouseIsOn(m_savePosScreen)){
            m_listOutput[i]->drawScreenPosUI(window);
        }
    }

    if(mouseIsOnBarre()) {
        if (m_producing) {
            if (m_targetRecipeSave) {
                m_targetRecipeSave->drawScreenPosUI(window);
            }
        }else{
            if (m_targetRecipe) {
                m_targetRecipe->drawScreenPosUI(window);
            }
        }
    }
}
