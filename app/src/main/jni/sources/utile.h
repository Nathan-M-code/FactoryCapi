#ifndef UTILE_H
#define UTILE_H

#define W_LOG 1

#define UPS 60
#define EPSILON 0.001
#define PI 3.14159265

#include <SFML/graphics.hpp>

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <random>
#include <iomanip>
#include <sstream>
#include "rapidjson/document.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_activity.h>

#include <SFML/System/NativeActivity.hpp>

#if W_LOG
#include "LoggerFile.h"
#else
#include <string>
#include <fstream>
#include <ctime>
#endif

enum QualityEnum{MEDIOCRE=0, NORMAL, GOOD, EXCELLENT};
enum TerrainEnum{dirt, mud, grass, stone, water, wood, coal, limestone, iron, copper, gold, oil};

enum Direction{
    EAST,
    WEST,
    NORD,
    SOUTH,
};

enum DataItemLabel{
    DATA_QUALITY
};

enum WIPStatus{
    CONSTRUCTION,
    DECONSTRUCTION
};

enum PowerUnit{
    kW,
    mW,
    gW
};

std::vector<std::string> split(std::string s, std::string delimiter);

sf::Color getRandomColor();

std::string dataItemLabelToString(DataItemLabel dataItemLabel);
std::string qualityToString(QualityEnum quality);
QualityEnum strToQuality(std::string str);
std::string directionToString(Direction dir);
std::string boolToString(bool b);
std::string vectorOfVector2iToString(const std::vector<sf::Vector2i> &vec);
std::string vectorOfVector2fToString(const std::vector<sf::Vector2f> &vec);
std::string vector2iToString(const sf::Vector2i &vec);
std::string vector2fToString(const sf::Vector2f &vec);


std::ostream& operator<<(std::ostream& out, Direction &dir);
std::ostream& operator<<(std::ostream& out, QualityEnum &quality);
std::ostream& operator<<(std::ostream& out, DataItemLabel &dataItemLabel);
std::ostream& operator<<(std::ostream& out, sf::Vector2i &vec);
std::ostream& operator<<(std::ostream& out, sf::Vector2f &vec);

bool approximatelyEqual(float a, float b);
bool essentiallyEqual(float a, float b);
bool definitelyGreaterThan(float a, float b);
bool definitelyLessThan(float a, float b);
double distance(sf::Vector2f coo1, sf::Vector2f coo2);
double distance(sf::Vector2i coo1, sf::Vector2i coo2);
sf::Vector2f getCenter(sf::Vector2f coo1, sf::Vector2f coo2);

int rand_a_b(int a,int b); //[a,b[

float qualityToMult(QualityEnum quality);
std::string timeToStr(const sf::Time &time);

void initTextStyle(sf::Text &text, sf::Font *font, unsigned size);
void initRectStyle(sf::RectangleShape &rect);
void initRectStyleBis(sf::RectangleShape &rect);

class Utile {
public:
    static void init(AAssetManager* assetManager);
    static std::string readFile(std::string path);
    static std::vector <std::string> getLinesFromFile(std::string path);

private:
    static AAssetManager* m_assetManager;
};



#endif //UTILE_H
