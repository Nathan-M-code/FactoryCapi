#include "MusicManager.h"
#include "SoundLoader.h"

#include "Map/Map.h"

#include "Entity/Building/Chest1.h"

using namespace std;
using namespace sf;


void verifViewPos(sf::View &view, float SPEEDCAM, Map &myMap){
    if(view.getCenter().y<= view.getSize().y/2.0){
        view.setCenter(view.getCenter().x,view.getSize().y/2.0);
    }
    if(view.getCenter().x<= view.getSize().x/2.0){
        view.setCenter(view.getSize().x/2.0,view.getCenter().y);
    }
    if(myMap.getSize().y - view.getCenter().y <= view.getSize().y/2.0){
        view.setCenter(view.getCenter().x,myMap.getSize().y-view.getSize().y/2.0);
    }
    if(myMap.getSize().x - view.getCenter().x <= view.getSize().x/2.0){
        view.setCenter(myMap.getSize().x-view.getSize().x/2.0,view.getCenter().y);
    }
}

void moveView(bool z, bool q, bool s, bool d, View &view, float SPEEDCAM, Map &myMap){
    if(z||q||s||d){
        if(z){
            view.move(0,-SPEEDCAM);
        }
        if(q){
            view.move(-SPEEDCAM,0);
        }
        if(s){
            view.move(0,SPEEDCAM);
        }
        if(d){
            view.move(SPEEDCAM,0);
        }
        verifViewPos(view,SPEEDCAM,myMap);
    }
}

void zoomView(int delta, View &view, float SPEEDCAM, Map &myMap){
    if(delta>0){ //zoom in delta>0
        view.zoom(0.7);
        if(view.getSize().x <= 500){
            view.setSize(500,281.25);
        }
    }else if(delta<0){ //zoom out delat<0
        view.zoom(1.3);
        if(view.getSize().x >= 3500){
            view.setSize(3500,1968.75);
        }
    }
    verifViewPos(view,SPEEDCAM,myMap);
}


int main()
{
    srand(time(NULL));
    TextureLoader::init();
    SoundLoader::init();
    OptionManager::loadOption();
    MusicManager musicManager;
    musicManager.loadPaths();
    musicManager.switchMusic(musicManager.day1);

    Vector2i tailleEcran = OptionManager::getScreenSize();

    Text text_camera;
    text_camera.setFont(*TextureLoader::getFont());
    text_camera.setFillColor(Color(255,255,255));
    text_camera.setCharacterSize(16);

    //option
    Vector2i mousePos;
    Vector2f cooMap;
    View view(FloatRect(0,0,tailleEcran.x,tailleEcran.y));

    RenderWindow window(VideoMode(tailleEcran.x,tailleEcran.y), "FactoryCapi", Style::Fullscreen);

    //window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    //constante
    const float SPEEDCAM = OptionManager::getSpeedCam();

    //variable classique
    bool z=false,q=false,s=false,d=false;

    //objet
    Map myMap;

    //clock manager
    unsigned int UPS = 60; //update per second -> delimit the gameplau clock
    //unsigned int RPS = 144; //render per second -> limit hardware calcul
    double MS_PER_UPDATE = 1000.0/UPS; //10ms
    //double MS_PER_RENDER = 1000.0/RPS;
    Clock clock;
    Time previousTime = clock.getElapsedTime();
    Time lagTimeUpdate;
    //Time lagTimeRender;

    Clock clockCalculUPSRPS;
    unsigned countRender = 0;
    unsigned countUpdate = 0;
    Time lagTimeForUPSRPS;
    float updatePerSec = 0;
    float renderPerSec = 0;

    while (window.isOpen())
    {
        Time currentTime = clock.getElapsedTime();
        lagTimeUpdate += currentTime - previousTime;

        previousTime = currentTime;

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    window.close();
                case Event::KeyPressed:
                    if (event.key.code == Keyboard::Escape){
                        window.close();
                    }
                    //-----------------------------------
                    if (event.key.code == Keyboard::Z){
                        z = true;
                    }
                    if (event.key.code == Keyboard::Q){
                        q = true;
                    }
                    if (event.key.code == Keyboard::S){
                        s = true;
                    }
                    if (event.key.code == Keyboard::D){
                        d = true;
                    }
                    //----------------gameplay----------------
                    //inv.keyEventHandler(event.key, myMap.getClock());
                    //------------------test-----------------
                    if (event.key.code == Keyboard::T){

                    }
                    break;
                case Event::KeyReleased:
                    if (event.key.code == Keyboard::Z){
                        z = false;
                    }
                    if (event.key.code == Keyboard::Q){
                        q = false;
                    }
                    if (event.key.code == Keyboard::S){
                        s = false;
                    }
                    if (event.key.code == Keyboard::D){
                        d = false;
                    }
                    break;

                case Event::MouseMoved:
                    mousePos = Mouse::getPosition(window);
                    cooMap = window.mapPixelToCoords(mousePos,view);
                    //myMap.mouseMoveHandler(cooMap);
                    break;

                case Event::MouseButtonPressed:
                    mousePos = Mouse::getPosition(window);
                    cooMap = window.mapPixelToCoords(mousePos,view);
                    myMap.clickHandler(event.mouseButton, true, cooMap);
                    break;

                case Event::MouseButtonReleased:
                    myMap.clickHandler(event.mouseButton, false, cooMap);
                    break;

                case Event::MouseWheelMoved:
                    zoomView(event.mouseWheel.delta, view, SPEEDCAM, myMap);
                    break;
            }
        }
        //logique
        while (lagTimeUpdate.asMilliseconds() >= MS_PER_UPDATE)
        {
            //ATTENTION MAKE SURE MS_PER_UPDATE IS HIGHER THAN THE CALCUL TIME
            myMap.update();

            //view
            moveView(z,q,s,d,view, SPEEDCAM, myMap);
            cooMap = window.mapPixelToCoords(mousePos,view);


            countUpdate++;
            lagTimeUpdate = milliseconds(lagTimeUpdate.asMilliseconds()-MS_PER_UPDATE);
        }



        //clear
        window.clear();

        //draw
        window.setView(view);

        myMap.draw(window);

        countRender++;


        window.setView(window.getDefaultView());

        lagTimeForUPSRPS += clockCalculUPSRPS.getElapsedTime();
        if(lagTimeForUPSRPS.asMilliseconds() >= 1000){
            lagTimeForUPSRPS -= milliseconds(1000);
            updatePerSec = countUpdate;
            countUpdate = 0;
            renderPerSec = countRender;
            countRender = 0;
        }
        clockCalculUPSRPS.restart();

        //Text
        text_camera.setString("view: center x: "+to_string(view.getCenter().x)+" center y: "+to_string(view.getCenter().y)+" size: "+to_string(view.getSize().x)+" "+to_string(view.getSize().y));
        text_camera.setPosition(0,0);
        window.draw(text_camera);
        text_camera.setString("mouse: x: "+to_string(mousePos.x)+" y: "+to_string(mousePos.y));
        text_camera.setPosition(0,40);
        window.draw(text_camera);
        text_camera.setString("mouseCooMap: x: "+to_string(cooMap.x)+" y: "+to_string(cooMap.y));
        text_camera.setPosition(0,60);
        window.draw(text_camera);

        text_camera.setString("UPS: "+to_string(updatePerSec)+" RPS: "+to_string(renderPerSec));
        text_camera.setPosition(0,120);
        window.draw(text_camera);
        text_camera.setString(to_string(lagTimeForUPSRPS.asMilliseconds()));
        text_camera.setPosition(0,140);
        window.draw(text_camera);

        window.display();
    }

    //OptionManager::saveOption();
    TextureLoader::freeRAM();
    SoundLoader::freeRAM();
    return 0;
}
