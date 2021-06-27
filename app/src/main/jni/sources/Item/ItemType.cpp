#include "ItemType.h"

ItemType::ItemType(unsigned int num, std::string name, std::string texturePath, unsigned int maxStack,
                   float nbSecCollectTime) {
    m_num = num;
    m_name = name;

    m_maxStack = maxStack;
    m_nbSecCollectTime = nbSecCollectTime;
    //m_nbSecProductionTime = nbSecProductionTime;

    m_texture = new sf::Texture;
    m_texture->loadFromFile(texturePath);
}

float ItemType::getNbSecCollectTime() const {
    return m_nbSecCollectTime;
}

/*float ItemType::getNbSecProductionTime() const {
    return m_nbSecProductionTime;
}*/
