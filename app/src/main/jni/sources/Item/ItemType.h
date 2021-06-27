#ifndef ITEMTYPE_H
#define ITEMTYPE_H

#include "../utile.h"

class ItemType{

public:
    ItemType(unsigned int num, std::string name, std::string texturePath, unsigned int maxStack,
             float nbSecCollectTime);
    ~ItemType();

    inline std::string getItemName() const{
        return m_name;
    }

    inline sf::Texture* getTexture() const{
        return m_texture;
    }

    inline unsigned getMaxStack() const{
        return m_maxStack;
    }

    float getNbSecCollectTime() const;
    //float getNbSecProductionTime() const;

private:
    unsigned m_num;
    std::string m_name;

    sf::Texture* m_texture;

    unsigned int m_maxStack;

    float m_nbSecCollectTime;
    //float m_nbSecProductionTime;
};

#endif //ITEMTYPE_H
