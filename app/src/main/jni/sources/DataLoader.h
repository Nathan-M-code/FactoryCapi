#ifndef TEXTURELOADER_H_INCLUDED
#define TEXTURELOADER_H_INCLUDED

#include "Entity/Building/BuildingType.h"
#include "Recipe/RecipeManager.h"
#include "Entity/Robot/RobotType.h"

#include "rapidjson/error/en.h"

class DataLoader{
public:
    static void init(AAssetManager* assetManager);

    static void loadAllBuildingType();
    static void loadAllItemType();
    static void loadAllRecipe();

    static void addTextureUI(std::string path);
    static void addTextureFactory(std::string path);

    static sf::Texture* getTextureTerrainTileSet();
    static sf::Texture* getTextureWorldMapTileSet();

    static BuildingType* getBuildingType(unsigned num);
    static rapidjson::Document* getDocumentBuilding();
    static rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>* getBuildingTypeValueJSON(unsigned index);

    static ItemType* getItemType(unsigned num);

    static sf::Texture* getTextureRobot();

    static sf::Texture* getTextureLightning();

    static sf::Texture* getTextureSelectedCorner();
    static sf::Texture* getTextureWipUI(WIPStatus wipStatus);
    static sf::Texture* getTextureArrowUI();
    static sf::Texture* getTextureScaffolding();
    static sf::Texture* getTextureFlush();
    static sf::Texture* getTextureDarkHand();
    static sf::Texture* getTextureTileWorldMapSelected();

    static sf::Texture* getTextureFactoryByLvl(unsigned lvl);

    static sf::Font* getFont();

    static DataItem* getDataItem(const rapidjson::Value &value);

    static void freeRAM();

private:
    static AAssetManager* m_assetManager;

    static std::vector<sf::Texture*> m_listTextureUI;
    static std::vector<sf::Texture*> m_listTextureFactoryWorldMap;

    static sf::Texture* m_textureTerrainTileSet;
    static sf::Texture* m_textureWorldMapTileSet;

    static std::map<unsigned, BuildingType *> m_mapNumBuildingType;
    static rapidjson::Document *m_documentBuilding;
    static std::vector<rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>> m_listBuildingTypeValueJSON;

    static std::map<unsigned, ItemType *> m_mapNumItemType;

    static sf::Texture *m_textureRobot;

    static sf::Font m_font;
};

#endif // TEXTURELOADER_H_INCLUDED
