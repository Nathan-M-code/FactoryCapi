#ifndef ROBOTTYPE_H
#define ROBOTTYPE_H

#include "../../utile.h"

class RobotType{

public:
    RobotType(unsigned num,
              std::string name,
              std::vector<std::string> listPath,
              std::vector<bool> listIsBoucingSpriteSheet,
              sf::Vector2i tileSizeSheet,
              sf::Vector2f sizeSprite);
    ~RobotType();

    inline unsigned getNum() const{
        return m_numType;
    }
    inline std::string getRobotName() const{
        return m_nameType;
    }

    inline std::vector<sf::Texture*> getListTexture() const{
        return m_listTexture;
    }

    inline sf::Texture* getTexture(unsigned index){
        return m_listTexture.at(index);
    }

    inline sf::Vector2i getTileSizeSheet() const{
        return m_tileSizeSheet;
    }

    inline unsigned getNbSheet() const{
        return m_tileSizeSheet.x*m_tileSizeSheet.y;
    }

    inline sf::Vector2f getSizeSprite() const{
        return m_sizeSprite;
    }

    inline bool isBoucingSpriteSheet(unsigned index) const{
        return m_listIsBoucingSpriteSheet.at(index);
    }

    inline sf::Time getMSEachTileSheet() const{
        sf::Time t = sf::milliseconds(1000.0/getNbSheet());
        //std::cout << m_nbCyclePerSec << " " << getNbSheet() << " " << t.asMilliseconds() << std::endl;
        return t;
    }

private:
    unsigned m_numType;
    std::string m_nameType;

    std::vector<sf::Texture*> m_listTexture;
    std::vector<bool> m_listIsBoucingSpriteSheet; //if the texture 0 is boucing, ect...

    sf::Vector2i m_tileSizeSheet;
    sf::Vector2f m_sizeSprite;
};

#endif //ROBOTTYPE_H

