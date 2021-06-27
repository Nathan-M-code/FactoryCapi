#include "RobotType.h"

using namespace std;
using namespace sf;

RobotType::RobotType(unsigned num,
              std::string name,
              std::vector<std::string> listPath,
              std::vector<bool> listIsBoucingSpriteSheet,
              sf::Vector2i tileSizeSheet,
              sf::Vector2f sizeSprite)
{
    m_numType = num;
    m_nameType = name;

    for(unsigned int i=0;i<listPath.size();i++){
        Texture* texture = new Texture;
        texture->loadFromFile(listPath[i]);
        m_listTexture.push_back(texture);
    }

    m_listIsBoucingSpriteSheet = listIsBoucingSpriteSheet;
    m_tileSizeSheet = tileSizeSheet;
    m_sizeSprite = sizeSprite;
}
RobotType::~RobotType(){

}
