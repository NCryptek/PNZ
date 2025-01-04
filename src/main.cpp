#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


const int MaxWidth = 10, MaxHeight = 10;
class Tile {
    sf::Sprite sprite;


    int tileWidth;
    int tileHeight;

    int totalTilesX;
    int totalTilesY;

    float X;
    float Y;

public:
    Tile(int typeOfTile, int a, int b, sf::Texture &tileSheetTexture): sprite(tileSheetTexture)
    {
        if (tileSheetTexture.loadFromFile("assets/ground.png"))
        {
            tileWidth = 64;
            tileHeight = 64;
            X = 0+float(a*tileWidth);
            Y = 0+float(b*tileHeight);

            totalTilesX = tileSheetTexture.getSize().x / tileWidth;
            totalTilesY = tileSheetTexture.getSize().y / tileHeight;
            
            sprite.setTextureRect(sf::IntRect({0, 0},{tileWidth, tileHeight}));
            sprite.setPosition({X, Y});
            
            if (typeOfTile == 1)
            {
                sprite.setTextureRect(sf::IntRect({tileWidth, 0},{tileWidth, tileHeight}));
                sprite.setPosition({X, Y});
            }
        }
    }
    sf::Sprite Draw() {
        return sprite;
    }
};


class Map {
private:
    int MaxWidth;
    int MaxHeight;
    std::vector<Tile> MapOfTheLevel;
public:
    Map(int Width, int Height, int *map, sf::Texture &texture)
    {
        MaxWidth = Width;
        MaxHeight = Height;
        for (int i = 0; i < MaxWidth; i++)
        {
            for (int j = 0; j < MaxHeight; j++)
            {
                Tile tmp(*((map + i*MaxHeight) + j), i, j, texture);
                MapOfTheLevel.push_back(tmp);
            }
        }
    }
    void DrawTheLevel(sf::RenderWindow &window) {
        for (int i = 0; i < MaxWidth; i++) 
        {
            for (int j = 0; j < MaxHeight; j++)
            {
                window.draw(MapOfTheLevel[i*MaxWidth+j].Draw());
            }
        }
    }

};



int main()
{
    sf::Texture TileSheet;
    int map[MaxWidth][MaxHeight] = {0};
    map[2][3] = 1;
    Map testowa(MaxWidth, MaxHeight, *map, TileSheet);

    // NOTE: Creating map, (make it in other file later) of size of MaxWidth and MaxHeight

    // NOTE: config.json is a config file (wow) that contains width, height and framerate 
    std::fstream f("./assets/config.json");
    json data = json::parse(f);

    // NOTE: creating and setting up window 
    auto window = sf::RenderWindow(sf::VideoMode({(data["width"]), (data["height"])}), "CMake SFML Project");
    window.setFramerateLimit(int(data["framerate"]));

    // NOTE: Creating view 
    sf::View view1(sf::FloatRect({2.f, 2.f}, {(data["width"]), (data["height"])}));

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
        testowa.DrawTheLevel(window);
        // DrawMapSprite(map, window);
        window.display();
    }
}
