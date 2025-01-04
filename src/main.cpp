#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../include/menu.h"
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
    std::fstream f("./config/settings.json");
    json settings = json::parse(f);
    std::cout << "[PNZ] Config succesful loaded!" << std::endl;

    // NOTE: creating and setting up window 
    auto window = sf::RenderWindow(sf::VideoMode({(settings["Screen_Width"]), (settings["Screen_Height"])}), "PNZ");
    window.setFramerateLimit(int(settings["Framerate"]));
    std::cout << "[PNZ] Window succesful created and set!" << std::endl;

    // NOTE: Creating view 
    sf::View view1(sf::FloatRect({0.f, 0.f}, {(settings["Screen_Width"]), (settings["Screen_Height"])}));
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


            for (int i = 0; i < MaxWidth; i++) {
                for (int j = 0; j < MaxHeight; j++) {
                    DrawMapSprite(i * tileWidth, j * tileHeight, map[i][j], window);
                }
            }

            window.setView(view1);
        }
        window.display();
    }

    return 0;
}
