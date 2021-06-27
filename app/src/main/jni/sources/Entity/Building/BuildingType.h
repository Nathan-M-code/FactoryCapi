#ifndef BUILDINGTYPE_H
#define BUILDINGTYPE_H

#include "../../utile.h"

class BuildingType{

public:
    BuildingType(unsigned numType,
                 std::string nameType,
                 std::vector<sf::Vector2i> listTileSizeSheet,
                 std::vector<sf::Vector2f> listSizeSprite,
                 std::vector<sf::Vector2i> listTileSizeBox,
                 std::vector<std::string> listPath,
                 std::vector<bool> listIsBoucingSpriteSheet,
                 float nbCyclePerSec,
                 std::string orientationMode,
                 bool hasQuality,
                 sf::Time constructionTime,
                 rapidjson::Value* valueJSON);
    ~BuildingType();

    inline unsigned getNum() const{
        return m_numType;
    }

    inline std::string getName() const{
        return m_nameType;
    }

    inline const std::vector<sf::Texture*> &getListTexture() const{
        return m_listTexture;
    }

    inline sf::Texture* getTexture(unsigned index) const{
        return m_listTexture.at(index);
    }

    inline unsigned getNbTexture() const{
        return m_listTexture.size();
    }

    inline const std::vector<sf::Vector2i> &getTileSizeSheet() const{
        return m_listTileSizeSheet;
    }

    inline unsigned getNbSheet(unsigned index) const{
        return m_listTileSizeSheet.at(index).x*m_listTileSizeSheet.at(index).y;
    }

    inline const std::vector<sf::Vector2f> &getSizeSprite() const{
        return m_listSizeSprite;
    }

    inline const std::vector<sf::Vector2i> &getTileSizeBox() const{
        return m_listTileSizeBox;
    }

    inline bool isBoucingSpriteSheet(unsigned index) const{
        return m_listIsBoucingSpriteSheet.at(index);
    }

    inline sf::Time getConstructionTime() const{
        return m_constructionTime;
    }

    inline float getNbCyclePerSecond() const{
        return m_nbCyclePerSec;
    }

    inline sf::Time getMSEachTileSheet(unsigned index) const{
        sf::Time t = sf::milliseconds(1000.0/m_nbCyclePerSec/getNbSheet(index));
        //std::cout << m_nbCyclePerSec << " " << getNbSheet() << " " << t.asMilliseconds() << std::endl;
        return t;
    }

    inline std::string getOrientationMode() const{
        return m_orientationMode;
    }

    inline bool hasQuality() const{
        return m_hasQuality;
    }

    inline rapidjson::Value *getValueJSON() {
        return m_valueJSON;
    }

protected:
    unsigned m_numType;
    std::string m_nameType;

    std::vector<sf::Texture*> m_listTexture;
    std::vector<bool> m_listIsBoucingSpriteSheet; //if the texture 1 is boucing, ect...

    std::vector<sf::Vector2i> m_listTileSizeSheet;
    std::vector<sf::Vector2f> m_listSizeSprite;
    std::vector<sf::Vector2i> m_listTileSizeBox;

    std::string m_orientationMode;

    sf::Time m_constructionTime;

    float m_nbCyclePerSec;

    bool m_hasQuality;

    rapidjson::Value* m_valueJSON;
};

#endif //BUILDINGTYPE_H

