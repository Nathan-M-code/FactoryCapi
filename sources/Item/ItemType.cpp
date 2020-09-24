#include "ItemType.h"

ItemType::ItemType(std::string name, unsigned int maxStack, unsigned int nbSecProductionTime){
    m_name = name;
    m_maxStack = maxStack;
    m_nbSecProductionTime = nbSecProductionTime;
}
