#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include "../utile.h"

class ItemType{

public:
    ItemType(std::string name, unsigned int maxStack, unsigned int nbSecProductionTime);
    ~ItemType();
private:
    std::string m_name;
    unsigned int m_maxStack;

    unsigned int m_nbSecProductionTime;
    std::vector<std::string> m_listNameItemTypeDependance;
    std::vector<unsigned int> m_listNameNbItemTypeDependance;
};

#endif //ITEMTYPE_H
