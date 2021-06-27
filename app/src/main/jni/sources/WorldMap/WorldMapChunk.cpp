//
// Created by natha on 11/11/2020.
//

#include "WorldMapChunk.h"

using namespace std;
using namespace sf;
using namespace rapidjson;

const sf::Vector2i WorldMapChunk::s_nbTile = {24, 24};
const int WorldMapChunk::s_sizeFactory = 72;
const int WorldMapChunk::s_sizeTileTerrain = 120;
const float WorldMapChunk::s = 61.875;
const float WorldMapChunk::h = (s_sizeTileTerrain - s) / 2.f;
const float WorldMapChunk::r = s_sizeTileTerrain / 2.f;
const sf::Vector2i WorldMapChunk::s_size = {int(s_nbTile.x * s_sizeTileTerrain + r), int((s_nbTile.y) * (h + s) + h)};
const sf::Vector2f WorldMapChunk::s_littleSize = {(WorldMapChunk::s_size.x-WorldMapChunk::r), (WorldMapChunk::s_size.y-WorldMapChunk::h)};

WorldMapChunk::WorldMapChunk(const sf::Vector2i& cooChunk){
    m_cooChunk = cooChunk;

    m_tileset = DataLoader::getTextureWorldMapTileSet();

    load();

    m_pos = getPosWorldByPosTileRelative({0, 0});
    //cout << "m_pos: " << m_pos << endl;

    m_rectArea.setSize({(float)s_size.x, (float)s_size.y});
    m_rectArea.setOutlineThickness(20);
    m_rectArea.setFillColor(Color::Transparent);
    m_rectArea.setOutlineColor(getRandomColor());
    m_rectArea.setPosition(m_pos);
}
WorldMapChunk::~WorldMapChunk(){
    for (int i = 0; i < m_listFactory.size(); ++i) {
        delete m_listFactory[i];
    }
}
const int WorldMapChunk::getSizeTile(){
    return s_sizeTileTerrain;
}
const Vector2i &WorldMapChunk::getNbTile() {
    return s_nbTile;
}
const float WorldMapChunk::getH() {
    return h;
}
const float WorldMapChunk::getR() {
    return r;
}
const float WorldMapChunk::getS() {
    return s;
}

sf::Vector2i WorldMapChunk::getCooChunk() const{
    return m_cooChunk;
}

sf::Vector2f WorldMapChunk::getPosWorldByPosTileRelative(const sf::Vector2i& posTileRelative) const{
    unsigned short impaire = (posTileRelative.y%2==0) ? 0:1;
    Vector2f pos = {posTileRelative.x * 2 * r + impaire * r, posTileRelative.y * (h + s)};
    pos.x += (float)m_cooChunk.x * s_size.x - m_cooChunk.x*r;
    pos.y += (float)m_cooChunk.y * s_size.y - m_cooChunk.y*h;
    return pos;
}
/*sf::Vector2f WorldMapChunk::getPosWorldByPosTileAbsolute(const sf::Vector2i& posTileAbsolute){
    unsigned short impaire = (posTileAbsolute.y%2==0) ? 0:1;
    Vector2f pos = {posTileAbsolute.x * 2 * r + impaire * r, posTileAbsolute.y * (h + s)};
    return pos;
}*/


sf::Vector2i WorldMapChunk::getCooTilePosChunk(const sf::Vector2f& mousePosChunk) const{
    //cout << "2r: " << 2*r << endl;
    //cout << "s + h: " << s + h << endl;

    int SectX = mousePosChunk.x / (2*r);
    int SectY = mousePosChunk.y / (s + h);

    int SectPxlX = mousePosChunk.x - SectX*(2*r);
    int SectPxlY = mousePosChunk.y - SectY*(s + h);

    //cout << "SectX: " << SectX << endl;
    //cout << "SectY: " << SectY << endl;

    //cout << "SectPxlX: " << SectPxlX << endl;
    //cout << "SectPxlY: " << SectPxlY << endl;

    float m = h / r; //around 0.484

    char SectTyp;
    if(SectY%2 == 0)
        SectTyp = 'A';
    else
        SectTyp = 'B';

    //cout << "SectTyp: " << SectTyp << endl;

    Vector2i tilePos = {-1,-1};

    if(SectTyp == 'A')
    {
        // middle
        tilePos.y = SectY;
        tilePos.x = SectX;
        // left Edge
        if(SectPxlY < (h - SectPxlX * m)){
            tilePos.y = SectY - 1;
            tilePos.x = SectX - 1;
        }
        // right Edge
        if(SectPxlY < (-h + SectPxlX * m)){
            tilePos.y = SectY - 1;
            tilePos.x = SectX;
        }
    }
    else
    {
        // right side
        if(SectPxlX >= r) {
            if (SectPxlY < (2 * h - SectPxlX * m))
            {
                tilePos.y = SectY - 1;
                tilePos.x = SectX;
            }else{
                tilePos.y = SectY;
                tilePos.x = SectX;
            }
        }
        // left side
        else if(SectPxlX < r) {
            if (SectPxlY < (SectPxlX * m))
            {
                tilePos.y = SectY - 1;
                tilePos.x = SectX;
            }else{
                tilePos.y = SectY;
                tilePos.x = SectX - 1;
            }
        }

    }
    //cout << tilePos << endl;
    return tilePos;
}

sf::Vector2i WorldMapChunk::getCooTileByPosWorld(sf::Vector2f& mousePosWorld, sf::Vector2i& posTileRelative, sf::Vector2i& posTileAbsolute){
    //cout << "mousePosWorld: " << mousePosWorld << endl;
    //cout << "m_pos: " << m_pos.x << " " << m_pos.y << endl;
    sf::Vector2f mousePosRelative = {mousePosWorld.x-m_pos.x,mousePosWorld.y-m_pos.y};
    //cout << "mousePosRelative: " << mousePosRelative << endl;
    posTileRelative = getCooTilePosChunk(mousePosRelative);
    posTileAbsolute = {posTileRelative.x + m_cooChunk.x * s_nbTile.x, posTileRelative.y + m_cooChunk.y * s_nbTile.y};
}

sf::Vector2i WorldMapChunk::getCooTileAbsoluteByCooRelative(const sf::Vector2i& posTileRelative) const{
    return {posTileRelative.x+m_cooChunk.x*s_nbTile.x,posTileRelative.y+m_cooChunk.y*s_nbTile.y};
}

sf::Vector2i WorldMapChunk::getSize(){
    return s_size;
}

sf::Vector2f WorldMapChunk::getLittleSize() {
    return s_littleSize;
}

FactoryWorldMap *WorldMapChunk::getFactory(const Vector2i &posRelative) {
    return m_doubleArrayFactory[posRelative.x][posRelative.y];
}

MapTile *WorldMapChunk::getMapTile(const sf::Vector2i &posRelative){
    return &m_doubleArrayMapTile[posRelative.x][posRelative.y];
}

bool WorldMapChunk::load() {
    //init
   std::vector<std::vector<unsigned>> listTileTexture;
    for (unsigned int col = 0; col < s_nbTile.x; col++){
        listTileTexture.push_back({});
        m_doubleArrayFactory.push_back({});
        m_doubleArrayMapTile.push_back({});
        for (unsigned int row = 0; row < s_nbTile.y; row++) {
            listTileTexture[col].push_back(0);
            m_doubleArrayFactory[col].push_back(nullptr);
        }
    }

    //reading
    string path = "data/worldMap/Chunks/"+to_string(m_cooChunk.x)+"-"+to_string(m_cooChunk.y)+".json";
    std::string json = Utile::readFile(path);

    Document doc;
    ParseResult ok = doc.Parse(json.c_str());
    if (!ok) {
        cout << "error parsing: " << GetParseError_En(ok.Code()) << endl;
    }

    Value& tiles = doc["tile"];
    unsigned indexRow = 0;
    unsigned indexCol = 0;
    for (auto& row : tiles.GetArray()){
        for (auto& col : row.GetArray()){
            //Value &obj = col;
            MapTile mt = {0,0,0};
            int value = col["textureNumber"].GetInt();
            mt.waterScale = col["waterScale"].GetInt();
            mt.woodScale = col["woodScale"].GetInt();
            mt.coalScale = col["coalScale"].GetInt();
            //cout << value << " " << indexCol << " " << indexRow << endl;
            m_doubleArrayMapTile[indexCol].push_back(mt);
            listTileTexture[indexCol][indexRow] = value;
            indexCol++;
        }
        indexCol = 0;
        indexRow++;
    }

    //cout << "end loading m_listTile ok" << endl;

    Value& factories = doc["factory"];
    for (auto& factory : factories.GetArray()){
        FactoryWorldMap * f = new FactoryWorldMap();
        Value& coo = factory["coo"];
        Vector2i cooTileRelative = {coo.GetArray()[0].GetInt(), coo.GetArray()[1].GetInt()};
        Vector2f posSprite = {0,0};

        Vector2f posTileHexa = getPosWorldByPosTileRelative(cooTileRelative);

        posSprite.x = posTileHexa.x + s_sizeTileTerrain / 2.f - s_sizeFactory / 2.f;
        posSprite.y = posTileHexa.y + s_sizeTileTerrain / 2.f - s_sizeFactory / 2.f;


        unsigned lvl = factory["lvl"].GetInt();
        sf::String owner = factory["owner"].GetString();

        //cout << lvl << " " << posTileHexa << " " << posSprite << endl;

        f->loadInfo(lvl,
                    owner,
                    posSprite,
                    {s_sizeFactory, s_sizeFactory},
                    getCooTileAbsoluteByCooRelative(cooTileRelative));

        m_doubleArrayFactory[cooTileRelative.x][cooTileRelative.y] = f;
        m_listFactory.push_back(f);
    }

    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(s_nbTile.x * s_nbTile.y * 4);

    int textureWidth = m_tileset->getSize().x/10;
    int textureHeight = m_tileset->getSize().y;

    // populate the vertex array, with one quad per tile
    for (unsigned int col = 0; col < listTileTexture.size(); col++)
        for (unsigned int row = 0; row < listTileTexture[col].size(); row++)
        {
            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[(row + col * s_nbTile.y) * 4];

            // define its 4 corners
            Vector2f pos = getPosWorldByPosTileRelative({(int)col,(int)row});
            //cout << pos << endl;
            quad[0].position = sf::Vector2f(pos.x, pos.y);
            quad[1].position = sf::Vector2f(pos.x + s_sizeTileTerrain, pos.y);
            quad[2].position = sf::Vector2f(pos.x + s_sizeTileTerrain, pos.y + s_sizeTileTerrain);
            quad[3].position = sf::Vector2f(pos.x, pos.y + s_sizeTileTerrain);

            // define its 4 texture coordinates
            unsigned num = m_doubleArrayFactory[col][row] ? 0 : listTileTexture[col][row];

            quad[0].texCoords = sf::Vector2f(num * textureWidth, 0);
            quad[1].texCoords = sf::Vector2f((num + 1) * textureWidth, 0);
            quad[2].texCoords = sf::Vector2f((num + 1) * textureWidth, textureHeight);
            quad[3].texCoords = sf::Vector2f(num * textureWidth, textureHeight);
        }
    return true;
}



void WorldMapChunk::draw(sf::RenderWindow& window){
    // draw the vertex array
    window.draw(m_vertices, m_tileset);

    for (int i = 0; i < m_listFactory.size(); ++i) {
        m_listFactory[i]->draw(window);
    }
}

void WorldMapChunk::drawWorldPosUI(sf::RenderWindow &window){
    window.draw(m_rectArea);
}

String WorldMapChunk::getStrInfo(MapTile *pMapTile) {
    String str;
    str += "waterScale: " + to_string(pMapTile->waterScale)+"\n";
    str += "woodScale: " + to_string(pMapTile->woodScale)+"\n";
    str += "coalScale: " + to_string(pMapTile->coalScale);
    return str;
}
