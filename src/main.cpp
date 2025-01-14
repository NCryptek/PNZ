#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include "menu.h"
#include "unit.h"
#include "tile.h"
#include "map.h"
#include "garage.h"
using json = nlohmann::json;

int main() {   
    // NOTE: Empty texture used later to use map
    sf::Texture TileSheet;
    sf::Texture TankUnit;
    sf::Texture PlayerCursor("assets/unit.jpg");
    sf::Sprite SpritePlayerCursor(PlayerCursor);
    SpritePlayerCursor.setTextureRect(sf::IntRect({256, 256},{64, 64}));
    SpritePlayerCursor.setPosition({0,0});
    
    // NOTE: Current map list:
    std::fstream tel("config/maps.json");
    json test = json::parse(tel);
    auto v = test["Map1"].get<std::vector<int>>();
    int MaxWidth = test["Width1"];
    int MaxHeight = test["Height1"];
    Map MapaNr1(MaxWidth, MaxHeight, v, TileSheet);
    v = test["Map2"].get<std::vector<int>>();
    MaxWidth = test["Width2"];
    MaxHeight = test["Height2"];
    Map MapaNr2(MaxWidth, MaxHeight, v, TileSheet);
    v = test["Map3"].get<std::vector<int>>();
    MaxWidth = test["Width3"];
    MaxHeight = test["Height3"];
    Map MapaNr3(MaxWidth, MaxHeight, v, TileSheet);
    v = test["Map4"].get<std::vector<int>>();
    MaxWidth = test["Width4"];
    MaxHeight = test["Height4"];
    Map MapaNr4(MaxWidth, MaxHeight, v, TileSheet);
    
    // NOTE: Player Data:
    int currentSelected = 0;
    int PlayerMoney = 20;
    std::vector<Unit> PlayerUnits;
    PlayerUnits.push_back(Unit("Tank",      1, 1, 20, 10, 16, 2, 3, 1, 0, 1, TankUnit));
    PlayerUnits.push_back(Unit("HoverTank", 2, 1, 15, 4, 3, 5, 1, 0, 0, 2, TankUnit));
    PlayerUnits.push_back(Unit("Mech",      3, 1, 25, 6, 2, 3, 2, 0, 0, 3, TankUnit));
    PlayerUnits.push_back(Unit("HeavyTank", 4, 1, 30, 5, 4, 2, 3, 1, 0, 4, TankUnit));
    
    // NOTE: Enemy Data:
    std::vector<Unit> EnemyUnits;
    EnemyUnits.push_back(Unit("Tank",      1, 2, 20, 10, 6, 2, 3, 1, 5, 1, TankUnit));
    EnemyUnits.push_back(Unit("HoverTank", 2, 2, 15, 4, 3, 5, 1, 0, 5, 2, TankUnit));
    EnemyUnits.push_back(Unit("Mech",      3, 2, 25, 6, 2, 3, 2, 0, 5, 3, TankUnit));
    EnemyUnits.push_back(Unit("HeavyTank", 4, 2, 30, 5, 4, 2, 3, 1, 5, 4, TankUnit));
    
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
    view1.zoom(0.5);
    view1.setCenter({200, 200});
    std::cout << "[PNZ] View succesful created!" << std::endl;

    sf::Font font;
    if (!font.openFromFile("./assets/arial.ttf")) { 
        std::cerr << "Failed to load font in main!" << std::endl;
        return -1;  
    }
    std::cout << "[PNZ] Font succesful loaded!" << std::endl;

    // NOTE: Creating menu
    Menu menu(window.getSize().x, window.getSize().y, settings);
    std::cout << "[PNZ] Menu succesful loaded!" << std::endl;
    bool showMenu = true;
    int level = -1;
    int state = 0;
    // 1 - Attack;
    // 2 - move;
    // 0 - choosing unit;
    // 4 - next player;
    int currentPlayer = 1;
    // 1 - player
    // -1 - enemy
    Garage gar(window.getSize().x, window.getSize().y);

    // INFO: 0 - garage, 1-4 - active levels

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                std::cout << "[PNZ] Window close event detected." << std::endl;
                window.close();
            }
            if (event->is<sf::Event::KeyPressed>() && showMenu) {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Up)
                    menu.moveUp();

                if (keyEvent->code == sf::Keyboard::Key::Down)
                    menu.moveDown();

                if (keyEvent->code == sf::Keyboard::Key::Enter) {
                    int selectedItem = menu.getSelectedItemIndex();
                    int currentPage = menu.getCurrentPageIndex();
                    if (selectedItem == 0 && currentPage == 0) {
                        std::cout << "[PNZ -> Menu] Selected: Campaign" << std::endl;
                        showMenu = false;
                        level = 0;
                    } else if (selectedItem == 1 && currentPage == 0) {
                        std::cout << "[PNZ -> Menu] Selected: Options" << std::endl;
                        menu.setMenuPage(0, 1);
                    } else if (selectedItem == 2 && currentPage == 0) {
                        std::cout << "[PNZ -> Menu] Selected: Exit" << std::endl;
                        window.close();
                    } else if (selectedItem == 2 && currentPage == 1) { 
                        std::cout << "[PNZ -> Men - Options] Selected: Back" << std::endl;
                        menu.setMenuPage(0, 0);
                    }
                } else if (keyEvent->code == sf::Keyboard::Key::Right) { 
                    int selectedItem = menu.getSelectedItemIndex();
                    int currentPage = menu.getCurrentPageIndex();
                    if (selectedItem == 0 && currentPage == 1) { 
                        std::cout << "[PNZ -> Menu - Settings] Changed: Resolution" << std::endl;
                        menu.setSetting(1, window);
                    }
                    if (selectedItem == 1 && currentPage == 1) { 
                        std::cout << "[PNZ -> Menu - Settings] Changed: Framerate" << std::endl;
                        menu.setSetting(2, window);
                    }
                }
            }
            if (event->is<sf::Event::KeyPressed>() && level == 0) {
                const auto& keyEvent = event->getIf<sf::Event::KeyPressed>();
                if (keyEvent->code == sf::Keyboard::Key::Up)
                    gar.moveUp();

                if (keyEvent->code == sf::Keyboard::Key::Down)
                    gar.moveDown();

                if (keyEvent->code == sf::Keyboard::Key::N) {
                    int selectedItem = gar.getSelectedItemIndex();
                    if (selectedItem == 0) {
                        std::cout << "[PNZ -> Missions] Loading 1" << std::endl;
                        MaxWidth = test["Width1"];
                        MaxHeight = test["Height1"];
                        level = 1;
                    } else if (selectedItem == 1) {
                        std::cout << "[PNZ -> Missions] Loading 2" << std::endl;
                        MaxWidth = test["Width2"];
                        MaxHeight = test["Height2"];
                        level = 2;
                    } else if (selectedItem == 2) {
                        std::cout << "[PNZ -> Missions] Loading 3" << std::endl;
                        MaxWidth = test["Width3"];
                        MaxHeight = test["Height3"];
                        level = 3;
                    } else if (selectedItem == 3) {
                        std::cout << "[PNZ -> Missions] Loading 4" << std::endl;
                        MaxWidth = test["Width4"];
                        MaxHeight = test["Height4"];
                        level = 4;

                    }
                }
            }
        }
        window.clear();

        if (showMenu) {
            menu.draw(window);
        } else {
            if(level == 1) {
                window.setView(view1);
                MapaNr1.DrawTheLevel(window);
            }
            if(level == 2) {
                window.setView(view1);
                MapaNr2.DrawTheLevel(window);
            }
            if(level == 3) {
                window.setView(view1);
                MapaNr3.DrawTheLevel(window);
            }
            if(level == 4) {
                window.setView(view1);
                MapaNr4.DrawTheLevel(window);
            }
            if (EnemyUnits.size() == 0)
            {
                level = 0;
                PlayerMoney +=5;
                EnemyUnits.push_back(Unit("Tank",      1, 2, 20, 10, 6, 2, 3, 1, 0, 1, TankUnit));
                EnemyUnits.push_back(Unit("HoverTank", 2, 2, 15, 4, 3, 5, 1, 0, 0, 2, TankUnit));
                EnemyUnits.push_back(Unit("Mech",      3, 2, 25, 6, 2, 3, 2, 0, 0, 3, TankUnit));
                EnemyUnits.push_back(Unit("HeavyTank", 4, 2, 30, 5, 4, 2, 3, 1, 0, 4, TankUnit));
                PlayerUnits.clear();
                PlayerUnits.push_back(Unit("Tank",      1, 1, 20, 10, 6, 2, 3, 1, 0, 1, TankUnit));
                PlayerUnits.push_back(Unit("HoverTank", 2, 1, 15, 4, 3, 5, 1, 0, 0, 2, TankUnit));
                PlayerUnits.push_back(Unit("Mech",      3, 1, 25, 6, 2, 3, 2, 0, 0, 3, TankUnit));
                PlayerUnits.push_back(Unit("HeavyTank", 4, 1, 30, 5, 4, 2, 3, 1, 0, 4, TankUnit));

            };
            if (PlayerUnits.size() == 0)
            {
                level = 0;
                PlayerMoney -=5;
                EnemyUnits.push_back(Unit("Tank",      1, 2, 20, 10, 6, 2, 3, 1, 0, 1, TankUnit));
                EnemyUnits.push_back(Unit("HoverTank", 2, 2, 15, 4, 3, 5, 1, 0, 0, 2, TankUnit));
                EnemyUnits.push_back(Unit("Mech",      3, 2, 25, 6, 2, 3, 2, 0, 0, 3, TankUnit));
                EnemyUnits.push_back(Unit("HeavyTank", 4, 2, 30, 5, 4, 2, 3, 1, 0, 4, TankUnit));
                EnemyUnits.clear();
                PlayerUnits.push_back(Unit("Tank",      1, 1, 20, 10, 6, 2, 3, 1, 0, 1, TankUnit));
                PlayerUnits.push_back(Unit("HoverTank", 2, 1, 15, 4, 3, 5, 1, 0, 0, 2, TankUnit));
                PlayerUnits.push_back(Unit("Mech",      3, 1, 25, 6, 2, 3, 2, 0, 0, 3, TankUnit));
                PlayerUnits.push_back(Unit("HeavyTank", 4, 1, 30, 5, 4, 2, 3, 1, 0, 4, TankUnit));

            };

            if(level == 0) {
                gar.Draw(window);
            }
            else {
                if(state == 4) {
                    sf::sleep(sf::milliseconds(200));
                    state = 0;
                    currentPlayer *= -1;
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) && state == 0)
                    currentSelected = 0;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) && state == 0)
                    currentSelected = 1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3) && state == 0)
                    currentSelected = 2;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4) && state == 0)
                    currentSelected = 3;
                int positionX = view1.getCenter().x;
                int positionY = view1.getCenter().y;
                float MovingValue = 5.f;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
                    state = 2;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
                    state = 1;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
                    state = 0;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V)) { 
                    state = 4;
                    PlayerUnits[currentSelected].resetMoveAndAtack();
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && state == 2) {
                    if(currentPlayer == 1) {
                        PlayerUnits[currentSelected].move(0, -1, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                    if(currentPlayer == -1) {
                        EnemyUnits[currentSelected].move(0, -1, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && state == 2) {
                    if(currentPlayer == 1) {
                        PlayerUnits[currentSelected].move(0, 1, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                    if(currentPlayer == -1) {
                        EnemyUnits[currentSelected].move(0, 1, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && state == 2) {
                    if(currentPlayer == 1) {
                        PlayerUnits[currentSelected].move(-1, 0, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                    if(currentPlayer == -1) {
                        EnemyUnits[currentSelected].move(-1, 0, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && state == 2) {
                    if(currentPlayer == 1) {
                        PlayerUnits[currentSelected].move(1, 0, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                    if(currentPlayer == -1) {
                        EnemyUnits[currentSelected].move(1, 0, MaxWidth, MaxHeight);
                        sf::sleep(sf::milliseconds(200));
                    }
                }

                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && state == 1) {
                    SpritePlayerCursor.move({0, -64});
                    sf::sleep(sf::milliseconds(200));
                }
                        
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && state == 1) {
                    SpritePlayerCursor.move({0, 64});
                    sf::sleep(sf::milliseconds(200));
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && state == 1) {
                    SpritePlayerCursor.move({-64, 0});
                    sf::sleep(sf::milliseconds(200));
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && state == 1) {
                    SpritePlayerCursor.move({64, 0});
                    sf::sleep(sf::milliseconds(200));
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && state == 1) {
                    float tmpa = SpritePlayerCursor.getPosition().x/64;
                    float tmpb = SpritePlayerCursor.getPosition().y/64;
                    if(currentPlayer == 1) {
                        for (int i = 0; i < EnemyUnits.size(); i++) {
                            if (tmpa == EnemyUnits[i].x && tmpb == EnemyUnits[i].y) {
                                PlayerUnits[currentSelected].Attack(EnemyUnits[i]);
                            }
                        }
                    sf::sleep(sf::milliseconds(200));
                    }
                }


                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && positionX > 0)
                    view1.move({-MovingValue, 0});
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && positionY > 0)
                    view1.move({0, -MovingValue});
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && positionY < MaxHeight*64)
                    view1.move({0, MovingValue});
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && positionX < MaxWidth*64)
                    view1.move({MovingValue, 0});
                for (int i = 0; i < PlayerUnits.size(); i++) {
                    PlayerUnits[i].DrawUnit(window);
                }
                for (int i = 0; i < EnemyUnits.size(); i++) {
                    EnemyUnits[i].DrawUnit(window);
                }
                if(state == 1) {
                    window.draw(SpritePlayerCursor);
                }
            }
        }
        window.display();
    }

    return 0;
}
