#include "DataLoader.h"

using namespace std;
using namespace sf;
using namespace rapidjson;

AAssetManager* DataLoader::m_assetManager = nullptr;

sf::Texture* DataLoader::m_textureTerrainTileSet = nullptr;
sf::Texture* DataLoader::m_textureWorldMapTileSet = nullptr;

std::map<unsigned, ItemType *> DataLoader::m_mapNumItemType = {};

std::map<unsigned, BuildingType *> DataLoader::m_mapNumBuildingType = {};
Document* DataLoader::m_documentBuilding = nullptr;
std::vector<rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>> DataLoader::m_listBuildingTypeValueJSON = {};

sf::Texture* DataLoader::m_textureRobot = nullptr;

std::vector<sf::Texture*> DataLoader::m_listTextureUI = {};
std::vector<sf::Texture*> DataLoader::m_listTextureFactoryWorldMap = {};

sf::Font DataLoader::m_font;

void DataLoader::init(AAssetManager* assetManager){
#if W_LOG
    LoggerFile::write("Init DataLoader");
#endif
    m_assetManager = assetManager;

    m_textureTerrainTileSet = new sf::Texture;
    m_textureTerrainTileSet->loadFromFile("data/map/tileSet.png");
    m_textureWorldMapTileSet = new sf::Texture;
    m_textureWorldMapTileSet->loadFromFile("data/worldMap/tileSet.png");

    DataLoader::addTextureUI("data/ui/SelectedCorner.png");
    DataLoader::addTextureUI("data/wip/wipUI.png");
    DataLoader::addTextureUI("data/wip/scaffolding.png");
    DataLoader::addTextureUI("data/ui/arrowIO.png");
    DataLoader::addTextureUI("data/ui/flush.png");
    DataLoader::addTextureUI("data/ui/darkHand.png");
    DataLoader::addTextureUI("data/wip/deconstruction.png");
    DataLoader::addTextureUI("data/worldMap/selected256x.png");
    DataLoader::addTextureUI("data/item/lightning256x.png");

    DataLoader::addTextureFactory("data/worldMap/factories/factory1-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory2-512x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory3-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory4-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory5-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory6-512x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory7-512x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory8-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory9-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory10-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory11-256x.png");
    DataLoader::addTextureFactory("data/worldMap/factories/factory12-256x.png");

    m_textureRobot = new sf::Texture;
    m_textureRobot->loadFromFile("data/robot/hr-construction-robot.png");

    m_font.loadFromFile("data/fonts/DejaVuSans.ttf");

    loadAllBuildingType();
    loadAllItemType();
    loadAllRecipe();
}

void DataLoader::loadAllBuildingType(){
    // 1. Parse a JSON string into DOM.
    cout << "loading data/building/dataBuildings.json" << endl;
#if W_LOG
    LoggerFile::write("start loading data/building/dataBuildings.json");
#endif

    std::string json = Utile::readFile("data/building/dataBuildings.json");

    Document *doc = new Document;
    ParseResult ok = doc->Parse(json.c_str());
    if (!ok) {
        cout << GetParseError_En(ok.Code()) << endl;
#if W_LOG
        LoggerFile::write(GetParseError_En(ok.Code()));
#endif
    }


    Value& arrayBuildings = (*doc)["buildings"];
    //cout << arrayBuildings.GetType() << endl;

    for (auto& building : arrayBuildings.GetArray()){
        unsigned num = building["num"].GetInt();
        string buildingName = building["nameType"].GetString();


        vector<string> listPath;
        Value&  texturePaths = building["texturePath"];
        for (auto& texturePath : texturePaths.GetArray()){
            listPath.push_back(texturePath.GetString());
        }

        vector<bool> listIsBouncingSprite;
        Value&  areBouncing = building["bouncingSpriteSheet"];
        for (auto& isBouncing : areBouncing.GetArray()){
            listIsBouncingSprite.push_back(isBouncing.GetBool());
        }

        vector<Vector2i> listTileSizeSheet;
        Value& tileSizeSheetCol = building["nbColSheet"];
        Value& tileSizeSheetRow = building["nbRowSheet"];
        for (SizeType i = 0; i < tileSizeSheetCol.Size(); i++){
            listTileSizeSheet.push_back(Vector2i(tileSizeSheetCol[i].GetInt(),tileSizeSheetRow[i].GetInt()));
        }

        vector<Vector2f> listSizeSprite;
        Value& sizeSpriteX = building["sizeX"];
        Value& sizeSpriteY = building["sizeY"];
        for (SizeType i = 0; i < sizeSpriteX.Size(); i++){
            listSizeSprite.push_back(Vector2f(sizeSpriteX[i].GetInt(),sizeSpriteY[i].GetInt()));
        }

        vector<Vector2i> listTileSizeBox;
        Value& listTileSizeBoxCol = building["nbColSizeBox"];
        Value& listTileSizeBoxRow = building["nbRowSizeBox"];
        for (SizeType i = 0; i < listTileSizeBoxCol.Size(); i++){
            listTileSizeBox.push_back(Vector2i(listTileSizeBoxCol[i].GetInt(),listTileSizeBoxRow[i].GetInt()));
        }

        std::string orientationMode = building["orientationMode"].GetString();

        float nbCyclePerSec = building["nbCyclePerSecond"].GetDouble();

        bool hasQuality = building["hasQuality"].GetBool();

        sf::Time constructionTime = sf::seconds(building["constructionTime"].GetInt());
#if W_LOG
        LoggerFile::write("----------------------------------\nbuildingNum: "+to_string(num)+"\n");
        LoggerFile::write("buildingName: "+buildingName+"\n");
#endif
        cout << "listPath: " << endl;
        for(unsigned i=0;i<listPath.size();i++){
            cout << "\t" << listPath[i] << endl;
        }

        cout << "bouncingSpriteSheet: " << endl;
        for(unsigned i=0;i<listIsBouncingSprite.size();i++){
            cout << "\t" << listIsBouncingSprite[i] << endl;
        }
        cout << "nbCyclePerSec: " << nbCyclePerSec << endl;

        cout << "listTileSizeSheet: " << vectorOfVector2iToString(listTileSizeSheet) << endl;
        cout << "listSizeSprite: " << vectorOfVector2fToString(listSizeSprite) << endl;
        cout << "listTileSizeBox: " << vectorOfVector2iToString(listTileSizeBox) << endl;

        cout << "OrientationMode: " << orientationMode << endl;

        cout << "hasQuality: " << hasQuality << endl;
        cout << "constructionTime: " << constructionTime.asSeconds() << endl;
        cout << "----------------------------------" << endl;

        if(m_mapNumBuildingType.count(num) == 0){
            m_mapNumBuildingType[num] = new BuildingType(num,
                                                         buildingName,
                                                         listTileSizeSheet,
                                                         listSizeSprite,
                                                         listTileSizeBox,
                                                         listPath,
                                                         listIsBouncingSprite,
                                                         nbCyclePerSec,
                                                         orientationMode,
                                                         hasQuality,
                                                         constructionTime,
                                                         &building);

        }else{
            cout << "there is more than one BuildingType with the num: " << num << endl;
            throw -1;
        }
    }
#if W_LOG
    LoggerFile::write("end load building");
#endif
}

Document* DataLoader::getDocumentBuilding(){
    return m_documentBuilding;
}

rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>* DataLoader::getBuildingTypeValueJSON(unsigned index){
    return &m_listBuildingTypeValueJSON.at(index);
}

void DataLoader::loadAllItemType(){
    // 1. Parse a JSON string into DOM.
    cout << "loading data/item/dataItems.json" << endl;

    std::string json = Utile::readFile("data/item/dataItems.json");

    Document doc;
    ParseResult ok = doc.Parse(json.c_str());
    if (!ok) {
        cout << GetParseError_En(ok.Code()) << endl;
    }

    Value& arrayBuildings = doc["items"];
    //cout << arrayBuildings.GetType() << endl;
    for (auto& item : arrayBuildings.GetArray()){
        unsigned num = item["num"].GetInt();

        string itemName = item["nameType"].GetString();
        string texturePath = item["texturePath"].GetString();

        unsigned maxStack = item["maxStack"].GetInt();

        float nbSecColl = item["nbSecCollectTime"].GetFloat();
        //float nbSecProd = item["nbSecProductionTime"].GetFloat();


        cout << "----------------------------------" << endl;
        cout << "itemName: " << itemName << endl;
        cout << "texturePath: " << texturePath << endl;

        cout << "maxStack: " << maxStack << endl;

        cout << "nbSecColl: " << nbSecColl << endl;
        //cout << "nbSecProd: " << nbSecProd << endl;
        cout << "----------------------------------" << endl;

        if(m_mapNumItemType.count(num) == 0){
            m_mapNumItemType[num] = new ItemType(num, itemName, texturePath, maxStack, nbSecColl);
        }else{
            cout << "there is more than one itemType with the num: " << num << endl;
            throw -1;
        }
    }
}

void DataLoader::loadAllRecipe() {
    // 1. Parse a JSON string into DOM.
    cout << "loading data/recipe/dataRecipes.json" << endl;

    std::string json = Utile::readFile("data/recipe/dataRecipes.json");

    Document doc;
    ParseResult ok = doc.Parse(json.c_str());
    if (!ok) {
        cout << GetParseError_En(ok.Code()) << endl;
    }

    Value& arrayRecipes = doc["recipes"];
    for (auto& recipe : arrayRecipes.GetArray()){
        Value &recipeArray = recipe;
        for (auto& recipeNIngredient : recipeArray.GetArray()){

            vector<ItemType*> listIngredient;
            Value &listIngredientJson = recipeNIngredient["ingredients"];
            for (auto& ingredient : listIngredientJson.GetArray()){
                listIngredient.push_back(m_mapNumItemType[ingredient.GetInt()]);
            }
            //verif
            /*for(auto const& val : listIngredient){
                cout << val << endl;
            }*/

            vector<unsigned int> listAmountIngre;
            Value &listAmountJson = recipeNIngredient["amount"];
            for (auto& amount : listAmountJson.GetArray()){
                listAmountIngre.push_back(amount.GetInt());
            }
            //verif
            /*for(auto const& val : listAmount){
                cout << val << endl;
            }*/
            if(listIngredient.size() != listAmountIngre.size()){
                cout << "listIngredient.size() != listAmountIngre.size()" << endl;
                throw -1;
            }


            vector<vector<DataItem*>> listDataItemIngre;
            for(unsigned i=0;i<listIngredient.size();i++){
                listDataItemIngre.push_back({});
            }

            Value &listDataIngre = recipeNIngredient["dataIngredient"];
            for (Value& data : listDataIngre.GetArray()){
                unsigned index = data["index"].GetInt();
                //cout << index << endl;
                //cout << data.GetType() << endl;
                try {
                    listDataItemIngre.at(index).push_back(getDataItem(data));
                } catch (std::out_of_range) {
                    cout << "out of range dataItemOutput index" << endl;
                    throw -1;
                }
            }

            if(listDataItemIngre.size() > listIngredient.size()){
                cout << "listDataItemIngre.size() > listIngredient.size()" << endl;
                throw -1;
            }

            //-----------OUTPUT---------------

            bool producesElectricity = false;
            int saveIndexElectricity = -1;
            unsigned amountElectricityProduced = 0;

            vector<ItemType*> listOutput;
            Value &listOutputJson = recipeNIngredient["result"];
            unsigned index = 0;
            for (auto& output : listOutputJson.GetArray()){
                if(output.IsInt()) {
                    listOutput.push_back(m_mapNumItemType[output.GetInt()]);
                }else if(output.IsString()){
                    if(string(output.GetString()).compare("electricity") == 0){
                        producesElectricity = true;
                        saveIndexElectricity = index;
                    }
                }
                index++;
            }
            //verif
            /*for(auto const& val : listOutput){
                cout << val->getItemName() << endl;
            }*/

            vector<unsigned int> listAmountOutput;
            index = 0;
            Value &listAmountOutputJson = recipeNIngredient["amountRes"];
            for (auto& amountRes : listAmountOutputJson.GetArray()){
                if(index == saveIndexElectricity){
                    amountElectricityProduced += amountRes.GetInt();
                }else {
                    listAmountOutput.push_back(amountRes.GetInt());
                }
                index++;
            }
            //verif
            /*for(auto const& val : listAmountOutput){
                cout << val << endl;
            }*/
            if(listOutput.size() != listAmountOutput.size()){
                cout << "listOutput.size() != listAmountOutput.size()" << endl;
                throw -1;
            }

            vector<vector<DataItem*>> listDataItemOutput;
            for(unsigned i=0;i<listOutput.size();i++){
                listDataItemOutput.push_back({});
            }

            Value &listDataOutput = recipeNIngredient["dataRes"];
            for (Value& data : listDataOutput.GetArray()){
                unsigned index = data["index"].GetInt();
                //cout << index << endl;
                //cout << data.GetType() << endl;
                try {
                    listDataItemOutput.at(index).push_back(getDataItem(data));
                } catch (std::out_of_range) {
                    cout << "out of range dataItemOutput index" << endl;
                    throw -1;
                }

            }

            if(listDataItemOutput.size() > listOutput.size()){
                cout << "listDataItemOutput.size() > listOutput.size()" << endl;
                throw -1;
            }

            float timeSec = recipeNIngredient["timeSec"].GetFloat();
            //cout << "timeSec: " << timeSec << endl;

            /*cout << "hasElectricity: " << boolToString(producesElectricity) << endl;
            if(producesElectricity)
                cout << "amountElectricity: " << to_string(amountElectricityProduced) << endl;*/

            RecipeManager::addRecipe(listIngredient,
                                     listAmountIngre,
                                     listDataItemIngre,
                                     listOutput,
                                     listAmountOutput,
                                     listDataItemOutput,
                                     timeSec,
                                     producesElectricity,
                                     amountElectricityProduced);
        }
    }
}

sf::Texture* DataLoader::getTextureRobot(){
    return m_textureRobot;
}

sf::Texture* DataLoader::getTextureLightning(){
    return m_listTextureUI[8];
}

sf::Texture* DataLoader::getTextureTerrainTileSet(){
    return m_textureTerrainTileSet;
}

sf::Texture* DataLoader::getTextureWorldMapTileSet(){
    return m_textureWorldMapTileSet;
}

BuildingType* DataLoader::getBuildingType(unsigned num){
    if(m_mapNumBuildingType.count(num) == 1){
        return m_mapNumBuildingType[num];
    }else{
        cout << "There is no " << num << " in m_mapNumBuildingType ... getBuildingType in DataLoader. We are returning nullptr" << endl;
        throw -1;
    }
    return nullptr;
}

ItemType* DataLoader::getItemType(unsigned num){
    if(m_mapNumItemType.count(num) == 1){
        return m_mapNumItemType[num];
    }else{
        cout << "There is no " << num << " in m_mapNumItemType ... getItemType in DataLoader. We are returning nullptr" << endl;
        throw -1;
    }
    return nullptr;
}

sf::Font* DataLoader::getFont(){
    return &m_font;
}

sf::Texture* DataLoader::getTextureSelectedCorner(){
    return m_listTextureUI[0];
}

sf::Texture* DataLoader::getTextureWipUI(WIPStatus wipStatus){
    if(wipStatus == CONSTRUCTION)
        return m_listTextureUI[1];
    else if(wipStatus == DECONSTRUCTION)
        return m_listTextureUI[6];
    else
        cout << "getTextureWipUI not managed" << endl;
    return nullptr;
}

sf::Texture* DataLoader::getTextureScaffolding(){
    return m_listTextureUI[2];
}

sf::Texture* DataLoader::getTextureArrowUI(){
    return m_listTextureUI[3];
}

sf::Texture* DataLoader::getTextureFlush() {
    return m_listTextureUI[4];
}
sf::Texture* DataLoader::getTextureDarkHand(){
    return m_listTextureUI[5];
}
sf::Texture* DataLoader::getTextureTileWorldMapSelected(){
    return m_listTextureUI[7];
}


sf::Texture* DataLoader::getTextureFactoryByLvl(unsigned lvl){
    try {
        return m_listTextureFactoryWorldMap.at(lvl/10);
    } catch (std::out_of_range) {
        cout << "lvl of factory not handled, returning the first texture" << endl;
        return m_listTextureFactoryWorldMap[0];
    }
}


DataItem* DataLoader::getDataItem(const rapidjson::Value &value){
    for (Value::ConstMemberIterator itr = value.MemberBegin();itr != value.MemberEnd(); ++itr)
    {
        string dateLabel = itr->name.GetString();
        if(dateLabel == "index"){

        }else if(dateLabel == dataItemLabelToString(DATA_QUALITY)){
            return new QualityDataItem(strToQuality(itr->value.GetString()));
        }else{
            cout << "data item in recipe not managed" << endl;
            throw -1;
        }
    }
    return nullptr;
}


void DataLoader::freeRAM(){
    cout << "TODO DataLoader::freeRAM" << endl;
}

void DataLoader::addTextureUI(std::string path){
    sf::Texture *t = new sf::Texture;
    t->loadFromFile(path);
    m_listTextureUI.push_back(t);
}

void DataLoader::addTextureFactory(std::string path){
    sf::Texture *t = new sf::Texture;
    t->loadFromFile(path);
    m_listTextureFactoryWorldMap.push_back(t);
}
