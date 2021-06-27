#include "Broadcast.h"

using namespace std;

sf::Clock Broadcast::s_clock;
sf::Text Broadcast::s_text;
sf::Time Broadcast::s_startTime;
sf::Time Broadcast::s_timeDraw;
bool Broadcast::s_drawn = false;

void Broadcast::init(){
    s_clock = sf::Clock();
    initTextStyle(s_text, DataLoader::getFont(), 48);
}

void Broadcast::startAnim(std::string str, unsigned nbSec, sf::Vector2f posScreen){
    s_startTime = s_clock.getElapsedTime();
    s_timeDraw = sf::seconds(nbSec);
    s_drawn = true;

    s_text.setString(str);
    s_text.setPosition(posScreen);
}

void Broadcast::announce(std::string str, unsigned nbSec, sf::Vector2f posScreen){
    s_text.setFillColor(sf::Color::Green);
    startAnim(str, nbSec, posScreen);
}

void Broadcast::error(std::string str, unsigned nbSec, sf::Vector2f posScreen){
    s_text.setFillColor(sf::Color::Red);
    startAnim(str, nbSec, posScreen);
}

void Broadcast::update(){
    if(s_clock.getElapsedTime() - s_startTime >= s_timeDraw){
        s_drawn = false;
    }
}

void Broadcast::draw(sf::RenderWindow &window){
    if(s_drawn){
        window.setView(window.getDefaultView());
        window.draw(s_text);
    }
}
