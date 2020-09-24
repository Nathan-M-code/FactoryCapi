#ifndef TEXTURELOADER_H_INCLUDED
#define TEXTURELOADER_H_INCLUDED

#include "Entity/Building/BuildingType.h"
#include "Entity/TransportSystem/TransportType.h"
#include "rapidjson/error/en.h"

class TextureLoader{
public:
    static void init();
    static void loadAllBuildingType();
    static void loadAllTransportType();

    static void addTextureTerrain(std::string path);

    static sf::Texture* getTextureTerrain(TerrainEnum terrainEnum);
    static BuildingType* getBuildingType(std::string buildingName);
    static TransportType* getTransportType(std::string buildingName);

    static sf::Font* getFont();

    static void freeRAM();

private:
    static std::vector<sf::Texture*> m_listTextureTerrain;
    static std::vector<BuildingType *> m_listBuildingType;
    static std::vector<TransportType *> m_listTransportType;
    static sf::Font m_font;
};

#endif // TEXTURELOADER_H_INCLUDED
