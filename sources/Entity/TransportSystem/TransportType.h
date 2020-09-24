#ifndef TRANSPORTTYPE_H
#define TRANSPORTTYPE_H

#include "../../utile.h"

class TransportType{

public:
    TransportType(std::string transportName, sf::Vector2i tileSizeSheet, sf::Vector2f sizeOneSheet, std::string texturePath, float itemPerSec);
    ~TransportType();

    inline std::string getTransportName() const{
        return m_transportName;
    }


    inline sf::Vector2i getTileSizeSheet() const{
        return m_tileSizeSheet;
    }
    inline int getNbSheet() const{
        return m_tileSizeSheet.x*m_tileSizeSheet.y;
    }
    inline sf::Vector2f getSizeOneSheet() const{
        return m_sizeOneSheet;
    }

    inline float getItemPerSecond() const{
        return m_itemPerSec;
    }
    inline sf::Time getMicroSEachTileSheet() const{
        sf::Time t = sf::microseconds(1000*(1000.0/m_itemPerSec)/m_tileSizeSheet.x);
        //std::cout << m_tileSizeSheet.x << " " << m_itemPerSec << " " << (1000.0/m_itemPerSec)/m_tileSizeSheet.x << std::endl;
        return t;
    }

    inline sf::Texture* getTexture() const{
        return m_texture;
    }

protected:
    std::string m_transportName;

    sf::Vector2i m_tileSizeSheet;
    sf::Vector2f m_sizeOneSheet;

    float m_itemPerSec;

    sf::Texture *m_texture;
};

#endif //TRANSPORTTYPE_H


