#include "BuildingType.h"

using namespace std;
using namespace sf;

BuildingType::BuildingType(unsigned numType,
                           std::string nameType,
                           std::vector<sf::Vector2i> listTileSizeSheet,
                           std::vector<sf::Vector2f> listSizeSprite,
                           std::vector<sf::Vector2i> listTileSizeBox,
                           vector<string> listPath,
                           vector<bool> listIsBoucingSpriteSheet,
                           float nbCyclePerSec,
                           std::string orientationMode,
                           bool hasQuality,
                           sf::Time constructionTime,
                           rapidjson::Value* valueJSON)
{
    m_numType = numType;
    m_nameType = nameType;

    for(unsigned int i=0;i<listPath.size();i++){
        Texture* texture = new Texture;
        if(!texture->loadFromFile(listPath[i])){
            cout << "impossible to load this image: " << listPath[i] << endl;
        }
        m_listTexture.push_back(texture);
    }

    m_listIsBoucingSpriteSheet = listIsBoucingSpriteSheet;
    m_listTileSizeSheet = listTileSizeSheet;
    m_listSizeSprite = listSizeSprite;
    m_listTileSizeBox = listTileSizeBox;

    m_orientationMode = orientationMode;

    m_constructionTime = constructionTime;

    m_nbCyclePerSec = nbCyclePerSec;

    m_hasQuality = hasQuality;

    m_valueJSON = valueJSON;
}

BuildingType::~BuildingType(){
    for(unsigned int i=0;i<m_listTexture.size();i++){
        delete m_listTexture[i];
        m_listTexture[i] = 0;
    }
}
