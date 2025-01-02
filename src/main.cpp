#include <iostream>
#include <test.h>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
using json = nlohmann::json;

sf::Sprite MapSprite(int x, int y, int tile) {
    int a = 0+x;
    int b = 0+y;
    sf::Texture textu("assets/ground.png");

    sf::Sprite sprite(textu);
    sprite.setPosition({float(a), float(b)});
    return sprite;
}

std::vector<sf::Sprite> test (int x);
int main()
{
    testowo sprawdzenie;
    sprawdzenie.a = 4;
    std::cout << sprawdzenie.a << std::endl;
    sprawdzenie.zmianaA();
   // std::cout << sprawdzenie.a << std::endl;





    // NOTE: Creating map, (make it in other file later) of size of MaxWidth and MaxHeight
    const int MaxWidth = 10, MaxHeight = 20;
    int map [MaxWidth][MaxHeight] = {0};

    // NOTE: config.json is a config file (wow) that contains width, height and framerate 
    std::fstream f("./assets/config.json");
    json data = json::parse(f);

    // NOTE: creating and setting up window 
    auto window = sf::RenderWindow(sf::VideoMode({(data["width"]), (data["height"])}), "CMake SFML Project");
    window.setFramerateLimit(int(data["framerate"]));

    // NOTE: Creating view 
    sf::View view1(sf::FloatRect({0.f, 0.f}, {(data["width"]), (data["height"])}));

    while (window.isOpen())
    {
        // NOTE: Its a event checker, don't move without a reson
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        // NOTE: Here is main loop. Here are should be all code that need to be run every frame

        int positionX = view1.getCenter().x;
        int positionY = view1.getCenter().y;
        float MovingValue = 5.f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && positionX > 0)
            view1.move({-MovingValue, 0});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && positionY > 0)
            view1.move({0, -MovingValue});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && positionY < MaxHeight*200)
            view1.move({0, MovingValue});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && positionX < MaxWidth*200)
            view1.move({MovingValue, 0});

        // NOTE: window.clear() will clear the screen, so if you want to draw somethig, it should be after that line, but before display()
        window.clear();
        window.setView(view1);

        window.draw(MapSprite(0, 0, 0));



        window.display();
    }
}
