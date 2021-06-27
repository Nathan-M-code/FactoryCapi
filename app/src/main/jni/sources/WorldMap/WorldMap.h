#ifndef WORLDMAP_H
#define WORLDMAP_H

#include "../utile.h"
#include "WorldMapChunk.h"
#include "../OptionManager.h"
#include "../DataLoader.h"
#include "FactoryWorldMap.h"

class WorldMap {
public:
    WorldMap(sf::RenderWindow* renderWindow);
    ~WorldMap();

    std::vector<sf::Vector2i> getCooChunkByCooMap(const sf::Vector2f& cooMap) const;

    void selectedTile(sf::Vector2i cooChunk, sf::Vector2i posTileRelative, const sf::Vector2i& posTileAbsolute);

    void setCooChunkCenter(const sf::Vector2i &cooChunk, bool fullLoad = false);

    void clickHandler(const sf::Event::MouseButtonEvent &mouseButtonEvent);
    void keyPressedEventHandler(sf::Event::KeyEvent &keyEvent);
    void keyReleasedEventHandler(sf::Event::KeyEvent &keyEvent);
    void mouseMoveHandler();
    void wheelEventHandler(sf::Event::MouseWheelEvent &mouseWheelEvent);

    void goToPos(const sf::Vector2f &worldPos);

    void update();

    void draw(sf::RenderWindow &window);

private:
    void deleteFromlistChunk(WorldMapChunk *chunk);

    WorldMapChunk* loadChunk(sf::Vector2i posChunk);

    void checkViewPos();
    void moveView();
    void zoomView(int delta);

    void updatePosVariables();
    void updateChunks();

private:
    sf::RenderWindow *m_renderWindow;

    sf::View m_view;
    float m_speedView;
    sf::Vector2i m_cooScreen;
    sf::Vector2f m_cooMap;

    sf::Vector2i m_cooChunkCenter;
    sf::Vector2i m_sizeChunk;
    sf::Vector2i m_sizeChunkLoaded;
    sf::Vector2i m_sizeChunkFree;
    sf::Vector2i m_saveCooChunk;

    std::vector<WorldMapChunk*> m_listChunk;
    std::vector<std::vector<WorldMapChunk*>> m_doubleArrayChunk;

    FactoryWorldMap * m_factorySelected;
    MapTile * m_mapTileSelected;
    sf::Sprite m_spriteSelected;
    sf::RectangleShape m_backgroundInfoMapTile;
    sf::Text m_textInfoMapTile;

    sf::Text m_textInfo;
};


#endif //WORLDMAP_H
