#include "BuildingType.h"

BuildingType::BuildingType(std::string buildingName, sf::Vector2i tileSizeSheet, sf::Vector2f sizeSprite, sf::Vector2i tileSizeBox, std::string texturePath, float nbCyclePerSec){
    m_buildingName = buildingName;

    m_tileSizeSheet = tileSizeSheet;
    m_sizeSprite = sizeSprite;
    m_tileSizeBox = tileSizeBox;

    m_nbCyclePerSec = nbCyclePerSec;

    m_texture = new sf::Texture;
    m_texture->loadFromFile(texturePath);
}

BuildingType::~BuildingType(){
    delete m_texture;
}
