#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../include/menu.h"
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
    std::fstream f("./config/settings.json");
    json settings = json::parse(f);
    std::cout << "[PNZ] Config succesful loaded!" << std::endl;

    // NOTE: creating and setting up window 
    auto window = sf::RenderWindow(sf::VideoMode({(settings["Screen_Width"]), (settings["Screen_Height"])}), "PNZ");
    window.setFramerateLimit(int(settings["Framerate"]));
    std::cout << "[PNZ] Window succesful created and set!" << std::endl;

    // NOTE: Creating view 
    sf::View view1(sf::FloatRect({2.f, 2.f}, {(settings["Screen_Width"]), (settings["Screen_Height"])}));
    std::cout << "[PNZ] View succesful created!" << std::endl;


    sf::Font font;
    if (!font.openFromFile("./assets/arial.ttf")) { 
        std::cerr << "Failed to load font in main!" << std::endl;
        return -1;  
    }
    std::cout << "[PNZ] Font succesful loaded!" << std::endl;

    // NOTE: Creating menu
    Menu menu(window.getSize().x, window.getSize().y);
    std::cout << "[PNZ] Menu succesful loaded!" << std::endl;
    bool showMenu = true;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                std::cout << "[PNZ] Window close event detected." << std::endl;
                window.close();
            } else if (event->is<sf::Event::KeyPressed>()) {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Up) {
                    menu.moveUp();
                } else if (keyEvent->code == sf::Keyboard::Key::Down) {
                    menu.moveDown();
                } else if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    int selectedItem = menu.getSelectedItemIndex();
                    if (selectedItem == 0) {
                        std::cout << "Start game selected" << std::endl;
                        showMenu = false;
                    } else if (selectedItem == 1) {
                        std::cout << "Options selected" << std::endl;
                        // Options
                    } else if (selectedItem == 2) {
                        std::cout << "Exit selected" << std::endl;
                        window.close();
                    }
                }
            }
        }

        window.clear();
        if (showMenu) {
            menu.draw(window);
        } else {
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


          

            window.setView(view1);
            testowa.DrawTheLevel(window);
        }
        window.display();
    }

    return 0;
}
