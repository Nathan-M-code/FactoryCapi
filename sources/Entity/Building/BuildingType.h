#ifndef BUILDINGTYPE_H
#define BUILDINGTYPE_H

#include "../../utile.h"

class BuildingType{

public:
    BuildingType(std::string buildingName, sf::Vector2i tileSizeSheet, sf::Vector2f sizeSprite, sf::Vector2i tileSizeBox, std::string texturePath, float nbCyclePerSec);
    ~BuildingType();

    inline std::string getBuildingName() const{
        return m_buildingName;
    }


    inline sf::Vector2i getTileSizeSheet() const{
        return m_tileSizeSheet;
    }
    inline int getNbSheet() const{
        return m_tileSizeSheet.x*m_tileSizeSheet.y;
    }
    inline sf::Vector2f getSizeSprite() const{
        return m_sizeSprite;
    }
    inline sf::Vector2i getTileSizeBox() const{
        return m_tileSizeBox;
    }
    inline int getNbCyclePerSecond() const{
        return m_nbCyclePerSec;
    }
    inline sf::Time getMSEachTileSheet() const{
        sf::Time t = sf::milliseconds(1000.0/m_nbCyclePerSec/getNbSheet());
        //std::cout << m_nbCyclePerSec << " " << getNbSheet() << " " << t.asMilliseconds() << std::endl;
        return t;
    }


    inline sf::Texture* getTexture() const{
        return m_texture;
    }

protected:
    std::string m_buildingName;

    sf::Vector2i m_tileSizeSheet;
    sf::Vector2f m_sizeSprite;
    sf::Vector2i m_tileSizeBox;

    float m_nbCyclePerSec;

    sf::Texture *m_texture;
};

#endif //BUILDINGTYPE_H

