#include "MusicManager.h"
#include "SoundLoader.h"

#include "Map/Map.h"
#include "WorldMap/WorldMap.h"

using namespace std;
using namespace sf;


int main()
{

#if W_LOG
    if(!LoggerFile::init("/sdcard/Download/output.txt")){
        cout << "can't init logger" << endl;
        return -1;
    }
#endif

    ANativeActivity* nativeActivity = sf::getNativeActivity();
    AAssetManager* assetManager = nativeActivity->assetManager;

    Utile::init(assetManager);

    //cout << std::filesystem::current_path() << endl;

    srand(time(nullptr));
    try{
        DataLoader::init(assetManager);
    }catch(int code){
        cout << "can't init DataLoader. Code: " << code << endl;
        return -1;
    }
    SoundLoader::init();
    OptionManager::loadOption(assetManager);
    /*MusicManager musicManager;
    musicManager.loadPaths();
    musicManager.switchMusic(musicManager.day1);*/

    Broadcast::init();

    Text text_camera;
    text_camera.setFont(*DataLoader::getFont());
    text_camera.setFillColor(Color(255,255,255));
    text_camera.setCharacterSize(16);

    //Vector2i mousePos;

    Vector2i screenSize = OptionManager::getScreenSize();
    RenderWindow window(VideoMode(screenSize.x,screenSize.y), "FactoryCapi", Style::Fullscreen);
    window.setVerticalSyncEnabled(true);

    //window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);

    //objet
    Map myMap(&window);
    //WorldMap worldMap(&window);

    //clock manager

    //unsigned int RPS = 144; //render per second -> limit hardware calcul
    double MS_PER_UPDATE = 1000.0/UPS;
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

    Sprite sprite;
    Texture* text;
    text = DataLoader::getTextureScaffolding();
    sprite.setTexture(*text);
    sprite.setPosition(0,0);

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
                    //------------------test-----------------
                    if (event.key.code == Keyboard::T){

                    }
                    //----------------gameplay----------------
                    myMap.keyPressedEventHandler(event.key);
                    //worldMap.keyPressedEventHandler(event.key);
                    break;
                case Event::KeyReleased:
                    myMap.keyReleasedEventHandler(event.key);
                    break;

                case Event::MouseMoved:
                    //mousePos = Mouse::getPosition(window);
                    myMap.mouseMoveHandler();
                    //worldMap.mouseMoveHandler();
                    break;

                case sf::Event::TouchBegan:
                    if (event.touch.finger == 0){

                    }
                    break;

                case Event::MouseButtonPressed:
                    myMap.clickHandler(event.mouseButton);
                    //worldMap.clickHandler(event.mouseButton);
                    break;

                case Event::MouseButtonReleased:
                    myMap.clickHandler(event.mouseButton);
                    break;

                case Event::MouseWheelMoved:
                    myMap.wheelEventHandler(event.mouseWheel);
                    //worldMap.wheelEventHandler(event.mouseWheel);
                    break;
            }
        }
        //logique
        while (lagTimeUpdate.asMilliseconds() >= MS_PER_UPDATE)
        {
            //ATTENTION MAKE SURE MS_PER_UPDATE IS HIGHER THAN THE CALCUL TIME
            myMap.update();
            //worldMap.update();
            Broadcast::update();

            countUpdate++;
            lagTimeUpdate = milliseconds(lagTimeUpdate.asMilliseconds()-MS_PER_UPDATE);
        }



        //clear
        window.clear();

        //draw
        //worldMap.draw(window);
        myMap.draw(window);
        myMap.drawWorldPosUI(window);

        window.draw(sprite);

        countRender++;

        window.setView(window.getDefaultView());
        myMap.drawScreenPosUI(window);
        Broadcast::draw(window);

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
        text_camera.setString("UPS: "+to_string(updatePerSec)+" RPS: "+to_string(renderPerSec));
        text_camera.setPosition(0,120);
        window.draw(text_camera);
        text_camera.setString(to_string(lagTimeForUPSRPS.asMilliseconds()));
        text_camera.setPosition(0,140);
        window.draw(text_camera);

        window.display();
    }

    //OptionManager::saveOption();
    DataLoader::freeRAM();
    SoundLoader::freeRAM();

#if W_LOG
    if(!LoggerFile::quit()){
        cout << "can't quit logger" << endl;
        return -1;
    }
#endif

    return 0;
}
