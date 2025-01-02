#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// NOTE: Some constants, size of each tile on map and the size of maps (in tiles) 
// WARNING!: For some strange reasons (and no longer existing default sf::Sprite constructor) I had to do it with classes
int tileWidth = 64, tileHeight = 64;
const int MaxWidth = 4, MaxHeight = 6;

// NOTE: a draw Sprite function (will be updated)
void DrawMapSprite(int x, int y, int tile, sf::RenderWindow &window) {
    int a = 0+x;
    int b = 0+y;
    sf::Texture textu("assets/ground.png");
    sf::Sprite sprite(textu);
    
    sprite.setTextureRect(sf::IntRect({tileWidth, 0}, {tileWidth, tileHeight}));
    if (tile == 0)
        sprite.setTextureRect(sf::IntRect({0, 0}, {tileWidth, tileHeight}));
    sprite.setPosition({float(a), float(b)});
    window.draw(sprite); 
}

int main()
{
    int map[MaxWidth][MaxHeight] = {0};
    map[2][3] = 1;

    // NOTE: Creating map, (make it in other file later) of size of MaxWidth and MaxHeight

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
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && positionY < MaxHeight*64)
            view1.move({0, MovingValue});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && positionX < MaxWidth*64)
            view1.move({MovingValue, 0});

        // NOTE: window.clear() will clear the screen, so if you want to draw somethig, it should be after that line, but before display()
        window.clear();
        window.setView(view1);
        for (int i = 0; i < MaxWidth; i++)
        {
            for (int j = 0; j < MaxHeight; j++)
                DrawMapSprite(i*tileWidth, j*tileHeight, map[i][j], window);
        }

        window.display();
    }
}
