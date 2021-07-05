#include "utile.h"

using namespace std;
using namespace sf;

AAssetManager* Utile::m_assetManager = nullptr;

void Utile::init(AAssetManager* assetManager){
#if W_LOG
    LoggerFile::write("Init Utile");
#endif
    m_assetManager = assetManager;
}

std::string Utile::readFile(std::string path){
    AAsset* mAsset;
    mAsset = AAssetManager_open(m_assetManager, path.c_str(), AASSET_MODE_BUFFER);
    int len = AAsset_getLength(mAsset);
    char buffer[len];
    int lenRead = AAsset_read(mAsset,buffer,len);
#if W_LOG
    LoggerFile::write( "This is a number: "+to_string(len));
    LoggerFile::write( "lenRead : "+to_string(lenRead));
    std::string str(buffer, len);
    //LoggerFile::write(json);
#endif
    AAsset_close(mAsset);

    return str;
}

std::vector<std::string> Utile::getLinesFromFile(std::string path){
    std::string str = readFile(path);
    std::vector<std::string> ret = split(str, "\n");
    return ret;
}

std::vector<std::string> split(std::string s, std::string delimiter){
    size_t last = 0;
    size_t next = 0;
    std::vector<std::string> ret;
    while ((next = s.find(delimiter, last)) != string::npos) {
        ret.push_back(s.substr(last, next-last));
        last = next + 1;
    }
    ret.push_back(s.substr(last));
    return ret;
}

int rand_a_b(int a,int b){ //[a,b[
    return rand()%(b-a)+a;
}

sf::Color getRandomColor(){
    int num = rand_a_b(0,7);
    switch(num){
        case 0:
            return sf::Color::Red;
        case 1:
            return sf::Color::Blue;
        case 2:
            return sf::Color::Green;
        case 3:
            return sf::Color::Cyan;
        case 4:
            return sf::Color::Magenta;
        case 5:
            return sf::Color::White;
        case 6:
            return sf::Color::Yellow;
    }
}

float qualityToMult(QualityEnum quality){
    switch(quality){
    case MEDIOCRE:
        return 0.5;
    case NORMAL:
        return 1;
    case GOOD:
        return 1.5;
    case EXCELLENT:
        return 2;
    default:
        cout << "default in qualityToMult utils.cpp" << endl;
        return 1;
    }
}

std::string timeToStr(const sf::Time &time){
    if(time<=seconds(0)){
        return "impossible";
    }

    unsigned hour = (int)(time.asSeconds())/3600;
    unsigned min = (int)(time.asSeconds())%3600/60;
    unsigned sec = (int)(time.asSeconds())%3600%60;

    if(hour != 0)
        return to_string(hour)+" h "+to_string(min)+" m "+to_string(sec)+" s";

    if(min != 0)
        return to_string(min)+" m "+to_string(sec)+" s";

    if(sec != 0)
        return to_string(sec)+" s";


    return to_string(time.asMilliseconds())+" ms";

}

std::ostream& operator<<(std::ostream& out, DataItemLabel &dataItemLabel){
    string ret;
    if(dataItemLabel == DATA_QUALITY){
        ret = "DATA_QUALITY";
    }
    return out << ret;
}

std::string dataItemLabelToString(DataItemLabel dataItemLabel){
    ostringstream oss;
    oss << dataItemLabel;
    return oss.str();
}

//--------------------------------------------------------------

std::ostream& operator<<(std::ostream& out, QualityEnum &quality){
    return out << qualityToString(quality);
}

std::string qualityToString(QualityEnum quality){
    string ret;
    if(quality == MEDIOCRE){
        ret = "MEDIOCRE";
    }else if(quality == NORMAL){
        ret = "NORMAL";
    }else if(quality == GOOD){
        ret = "GOOD";
    }else if(quality == EXCELLENT){
        ret = "EXCELLENT";
    }
    return ret;
}
QualityEnum strToQuality(std::string str){
    QualityEnum ret;
    if(str == "MEDIOCRE"){
        ret = MEDIOCRE;
    }else if(str == "NORMAL"){
        ret = NORMAL;
    }else if(str == "GOOD"){
        ret = GOOD;
    }else if(str == "EXCELLENT"){
        ret = EXCELLENT;
    }
    return ret;
}

//--------------------------------------------------------------

std::string directionToString(Direction dir){
    ostringstream oss;
    oss << dir;
    return oss.str();
}
std::ostream& operator<<(std::ostream& out, Direction &dir){
    string ret;
    if(dir == NORD){
        ret = "NORD";
    }else if(dir == SOUTH){
        ret = "SOUTH";
    }else if(dir == EAST){
        ret = "EAST";
    }else if(dir == WEST){
        ret = "WEST";
    }
    return out << ret;
}





std::string boolToString(bool b){
    return b ? "Yes" : "No";
}

std::ostream& operator<<(std::ostream& out, sf::Vector2f &vec){
    return out << "Vec: x: " << vec.x << " y: " << vec.y;
}

std::string vector2iToString(const sf::Vector2i &vec){
    return "["+to_string(vec.x)+", "+to_string(vec.y)+"]";
}

std::string vector2fToString(const sf::Vector2f &vec){
    return "["+to_string(vec.x)+", "+to_string(vec.y)+"]";
}

std::ostream& operator<<(std::ostream& out, sf::Vector2i &vec){
    return out << vector2iToString(vec);
}

std::string vectorOfVector2iToString(const std::vector<sf::Vector2i> &vec){
    string ret = "[";
    for(const auto &val: vec){
        ret += vector2iToString(val);
        ret += ", ";
    }
    ret += "]";
    return ret;
}

std::string vectorOfVector2fToString(const std::vector<sf::Vector2f> &vec){
    string ret = "[";
    for(const auto &val: vec){
        ret += vector2fToString(val);
        ret += ", ";
    }
    ret += "]";
    return ret;
}


bool approximatelyEqual(float a, float b)
{
    return fabs(a - b) <= ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

bool essentiallyEqual(float a, float b)
{
    return fabs(a - b) <= ( (fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

bool definitelyGreaterThan(float a, float b)
{
    return (a - b) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

bool definitelyLessThan(float a, float b)
{
    return (b - a) > ( (fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * EPSILON);
}

double distance(sf::Vector2f coo1, sf::Vector2f coo2){
    return sqrt(pow(coo2.x-coo1.x,2)+pow(coo2.y-coo1.y,2));
}

double distance(sf::Vector2i coo1, sf::Vector2i coo2){
    return sqrt(pow(coo2.x-coo1.x,2)+pow(coo2.y-coo1.y,2));
}

Vector2f getCenter(sf::Vector2f coo1, sf::Vector2f coo2){
    return Vector2f(round((coo2.x+coo1.x)/2.0),round((coo2.y+coo1.y)/2.0));
}

void initTextStyle(sf::Text &text, sf::Font *font, unsigned size){
    text.setFont(*font);
    text.setCharacterSize(size);
    text.setOutlineThickness((size/25)+1);
    text.setOutlineColor(sf::Color::Black);
    text.setFillColor(sf::Color::White);
}

void initRectStyle(sf::RectangleShape &rect){
    rect.setFillColor(sf::Color(100,100,100));
    rect.setOutlineColor(sf::Color(50,50,50));
    rect.setOutlineThickness(5);
}

void initRectStyleBis(sf::RectangleShape &rect){
    rect.setFillColor(sf::Color(200,200,200));
    rect.setOutlineColor(sf::Color(150,150,150));
    rect.setOutlineThickness(2);
}
