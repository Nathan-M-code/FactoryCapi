#include "TransportType.h"

TransportType::TransportType(std::string transportName, sf::Vector2i tileSizeSheet, sf::Vector2f sizeOneSheet, std::string texturePath, float itemPerSec){
    m_transportName = transportName;

    m_tileSizeSheet = tileSizeSheet;
    m_sizeOneSheet = sizeOneSheet;

    m_itemPerSec = itemPerSec;

    m_texture = new sf::Texture;
    m_texture->loadFromFile(texturePath);
}

TransportType::~TransportType(){
    delete m_texture;
}

