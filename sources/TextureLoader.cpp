#include "TextureLoader.h"

using namespace std;
using namespace sf;
using namespace rapidjson;

std::vector<sf::Texture*> TextureLoader::m_listTextureTerrain = {};
std::vector<BuildingType *> TextureLoader::m_listBuildingType = {};
std::vector<TransportType *> TextureLoader::m_listTransportType = {};
//std::vector<sf::Texture*> TextureLoader::m_listeTextureUI = {};

sf::Font TextureLoader::m_font;

void TextureLoader::init(){
    TextureLoader::addTextureTerrain("data/map/dirt256x.png");
    TextureLoader::addTextureTerrain("data/map/mud256x.png");
    TextureLoader::addTextureTerrain("data/map/grass256x.png");
    TextureLoader::addTextureTerrain("data/map/stone256x.png");
    TextureLoader::addTextureTerrain("data/map/water256x.png");

    m_font.loadFromFile("data/fonts/DejaVuSans.ttf");

    loadAllBuildingType();
    loadAllTransportType();
}

void TextureLoader::loadAllBuildingType(){
    // 1. Parse a JSON string into DOM.
    cout << "loading data/building/dataBuildings.json" << endl;
    std::ifstream inputFile("data/building/dataBuildings.json", std::ifstream::binary);
    if (inputFile) {
        // get length of file:
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        std::string json = buffer.str();

        Document doc;
        //doc.Parse(buffer);

        ParseResult ok = doc.Parse(json.c_str());
        if (!ok) {
            cout << GetParseError_En(ok.Code()) << endl;
        }

        Value& arrayBuildings = doc["buildings"];
        //cout << arrayBuildings.GetType() << endl;
        for (auto& building : arrayBuildings.GetArray()){
            string buildingName = building["nameType"].GetString();
            string texturePath = building["texturePath"].GetString();

            Vector2i tileSizeSheet(building["nbColSheet"].GetInt(),building["nbRowSheet"].GetInt());
            Vector2f sizeSprite(building["sizeX"].GetInt(),building["sizeY"].GetInt());
            Vector2i tileSizeBox(building["nbColSizeBox"].GetInt(),building["nbRowSizeBox"].GetInt());
            float nbCyclePerSec = building["nbCyclePerSeconde"].GetDouble();

            cout << "buildingName: " << buildingName << endl;
            cout << "texturePath: " << texturePath << endl;
            cout << "nbCyclePerSec: " << nbCyclePerSec << endl;

            cout << "tileSizeSheet: " << tileSizeSheet.x << " " << tileSizeSheet.y << endl;
            cout << "sizeSprite: " << sizeSprite.x << " " << sizeSprite.y << endl;
            cout << "tileSizeBox: " << tileSizeBox.x << " " << tileSizeBox.y << endl;

            m_listBuildingType.push_back(new BuildingType(buildingName,tileSizeSheet,sizeSprite,tileSizeBox,texturePath,nbCyclePerSec));
        }

        inputFile.close();
    }
}

void TextureLoader::loadAllTransportType(){
    // 1. Parse a JSON string into DOM.
    cout << "loading data/transportSystem/dataTransportSystem.json" << endl;
    std::ifstream inputFile("data/transportSystem/dataTransportSystem.json", std::ifstream::binary);
    if (inputFile) {
        // get length of file:
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        std::string json = buffer.str();

        Document doc;

        //cout << json << endl;

        ParseResult ok = doc.Parse(json.c_str());
        if (!ok) {
            cout << GetParseError_En(ok.Code()) << endl;
        }

        Value& arrayBuildings = doc["transports"];
        //cout << arrayBuildings.GetType() << endl;
        for (auto& transport : arrayBuildings.GetArray()){
            string transportName = transport["nameType"].GetString();
            string texturePath = transport["texturePath"].GetString();

            Vector2i tileSizeSheet(transport["nbColSheet"].GetInt(),transport["nbRowSheet"].GetInt());
            Vector2f sizeSprite(transport["sizeX"].GetInt(),transport["sizeY"].GetInt());

            float itemPerSec = transport["itemPerSec"].GetDouble();

            cout << "transportName: " << transportName << endl;
            cout << "texturePath: " << texturePath << endl;
            cout << "itemPerSec: " << itemPerSec << endl;

            cout << "tileSizeSheet: " << tileSizeSheet.x << " " << tileSizeSheet.y << endl;
            cout << "sizeSprite: " << sizeSprite.x << " " << sizeSprite.y << endl;

            m_listTransportType.push_back(new TransportType(transportName,tileSizeSheet,sizeSprite,texturePath,itemPerSec));
        }

        inputFile.close();
    }
}

sf::Texture* TextureLoader::getTextureTerrain(TerrainEnum terrainEnum){
    if(terrainEnum == dirt)
        return m_listTextureTerrain[0];
    else if(terrainEnum == mud)
        return m_listTextureTerrain[1];
    else if(terrainEnum == grass)
        return m_listTextureTerrain[2];
    else if(terrainEnum == stone)
        return m_listTextureTerrain[3];
    else if(terrainEnum == water)
        return m_listTextureTerrain[4];

    return 0;
}

BuildingType* TextureLoader::getBuildingType(std::string buildingName){
    for(unsigned int i=0;i<m_listBuildingType.size();i++){
        if(m_listBuildingType[i]->getBuildingName() == buildingName){
            return m_listBuildingType[i];
        }
    }
    cout << "There is no " << buildingName << " in m_listBuildingType ... getBuildingType in TextureLoader. We are returning nullptr" << endl;
    return nullptr;
}

TransportType* TextureLoader::getTransportType(std::string transportName){
    for(unsigned int i=0;i<m_listTransportType.size();i++){
        if(m_listTransportType[i]->getTransportName() == transportName){
            return m_listTransportType[i];
        }
    }
    cout << "There is no " << transportName << " in m_listTransportType ... getTransportType in TextureLoader. We are returning nullptr" << endl;
    return nullptr;
}

sf::Font* TextureLoader::getFont(){
    return &m_font;
}


void TextureLoader::freeRAM(){
    for(unsigned int i=0;i<m_listTextureTerrain.size();i++){
        delete m_listTextureTerrain[i];
        m_listTextureTerrain[i] = 0;
    }
    for(unsigned int i=0;i<m_listBuildingType.size();i++){
        delete m_listBuildingType[i];
        m_listBuildingType[i] = 0;
    }
}


void TextureLoader::addTextureTerrain(std::string path){
    sf::Texture *t = new sf::Texture;
    t->loadFromFile(path);
    m_listTextureTerrain.push_back(t);
}
