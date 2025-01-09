#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../include/menu.h"
using json = nlohmann::json;

float dmgEff[3][2] = {
    {1.0,  0.5 }, // NOTE: FIRST COLUMN IS FOR LIGHT ARMOR, SECOND FOR HEAVY
    {0.5,  1.0 }, // NOTE: from top to bottom: lasers, guns, high explosives
    {0.75, 0.75}
};
int UnitMap[20][20] = {0};

int isOccupied(int x, int y){
    if(UnitMap[x][y] > 0)
        return 1;
    return 0;
}

class Unit {
    sf::Sprite sprite;
    std::string Name;
    int id;
    int idOwner;
    int maxHealth;
    int damage;
    int range;
    int maxMovement;
    int movement;
    int damageType;
    int armorType;
public:
    int x;
    int Health;
    int y;
    int movePoints;
    Unit(std::string NAME, int ID, int IDOwner, int MAXHEALTH, int DAMAGE, int RANGE, int MOVEMENT, int DT, int AT, int X, int Y, sf::Texture &UnitTexture):  sprite(UnitTexture) {
        if (UnitTexture.loadFromFile("assets/unit.jpg"))
        {
            if(ID == 1) {
                sprite.setTextureRect(sf::IntRect({0, 0},{64, 64}));
                sprite.setPosition({float(X*64), float(Y*64)});
            }
            if(ID == 2) {
                sprite.setTextureRect(sf::IntRect({64, 0},{64, 64}));
                sprite.setPosition({float(X*64), float(Y*64)});
            }
            if(ID == 3) {
                sprite.setTextureRect(sf::IntRect({0, 0},{64, 64}));
                sprite.setPosition({float(X*64), float(Y*64)});
            }
            if(ID == 4) {
                sprite.setTextureRect(sf::IntRect({64, 0},{64, 64}));
                sprite.setPosition({float(X*64), float(Y*64)});
            }
            Name = NAME;
            id = ID;
            idOwner = IDOwner;
            maxHealth = MAXHEALTH;
            Health = maxHealth;
            damage = DAMAGE;
            range = RANGE;
            movement = MOVEMENT;
            maxMovement = movement;
            damageType = DT;
            armorType = AT;
            movePoints = 1;
            x = X;
            y = Y;
            UnitMap[x][y] = idOwner;
        }
    }
    void takeDamage(int dmg, int dT) {
        Health -= (dmg * dmgEff[dT][armorType]);
        if (Health <= 0) {
            Health = 0;
        }
    }
    int GetOwnerID() {
        return idOwner;
    }
    void move(int X, int Y, int maxX, int maxY) {
        if (movePoints < 1)
            return;
        if(movement < 1)
            return;
        if (isOccupied(x+X, y+Y))
            return;
        if(x+X > maxX-1 || y+Y > maxY-1 || y+Y < 0 || x+X < 0)
            return;
        UnitMap[x][y] = 0;
        x += X;
        y += Y;
        UnitMap[x][y] = idOwner;
        movement-=1;
        sprite.setPosition({float(x*64), float(y*64)});
    }
    void Attack(Unit enemy) {
        if (movePoints < 1)
            return;
        int dist = (abs(enemy.x-x)/64 + abs (enemy.y-y))/64;
        if(dist>range)
            return;
        if(enemy.GetOwnerID() == idOwner)
            return;
        enemy.takeDamage(damage, damageType);
        movePoints -= 1;

    }
    void DrawUnit(sf::RenderWindow &window) {
        window.draw(sprite);
    }
    void UpgradeHealth(int newHealth) {
        maxHealth = newHealth;
        Health = newHealth;
    }
    void UpgradeRange(int newRange) {
        range = newRange;
    }
    void UpgradeMovement(int newMovement) {
        movement = newMovement;
    }
    void UpgradeDamage(int newDamage) {
        damage = newDamage;
    }
    void resetMoveAndAtack() {
        movePoints = 1;
        movement = maxMovement;

    }

};




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
            X = float(a*tileWidth);
            Y = float(b*tileHeight);

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
    Map(int Width, int Height, std::vector<int> map, sf::Texture &texture)
    {
        MaxWidth = Width;
        MaxHeight = Height;
        for (int i = 0; i < MaxHeight; i++)
        {
            for (int j = 0; j < MaxWidth; j++)
            {
                Tile tmp(map[i*MaxWidth + j], j, i, texture);
                MapOfTheLevel.push_back(tmp);
            }
        }
    }
    void DrawTheLevel(sf::RenderWindow &window) {
        for (int i = 0; i < MaxHeight; i++)
        {
            for (int j = 0; j < MaxWidth; j++)
            {
                window.draw(MapOfTheLevel[i*MaxWidth+j].Draw());
            }
        }
    }

};

class Garage {
    sf::Sprite sprite;
    sf::Texture texture;
    std::vector<sf::Text> HangarItems;
    sf::Font font;
    int selectedItemIndex;



public:
    Garage(float width, float height): sprite(texture){
        if (!font.openFromFile("./assets/arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            std::abort(); // Abort if the font fails to load
        }
        if (!texture.loadFromFile("assets/hangar_background.jpg"))
            std::cout << "Not loaded file\n";
        sprite.setTextureRect(sf::IntRect({0, 0},{int(width), int(height)}));
        std::vector<std::string> missions = {"Mision 1", "Mision 2", "Mision 3", "Mision 4"};
        for (int i = 0; i < missions.size(); ++i) {
            sf::Text text(font, missions[i], 30);
            text.setStyle(sf::Text::Bold);
            text.setFillColor(sf::Color::Green);
            text.setPosition(sf::Vector2f(width-300, height / (missions.size() + 5) * (i + 1.5)));

            HangarItems.push_back(text);

        }
        selectedItemIndex = 0;
        HangarItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
    void Draw(sf::RenderWindow &window) {
        window.draw(sprite);
        for (int i = 0; i < HangarItems.size(); i++)
        {
            sf::Text blurredText = HangarItems[i];
            blurredText.setFillColor(sf::Color(255, 255, 255, 50));
            for (int x = -4; x <= 4; ++x) {
                for (int y = -4; y <= 4; ++y) {
                    if (x != 0 || y != 0) {
                        blurredText.setPosition(HangarItems[i].getPosition() + sf::Vector2f(x, y));
                        window.draw(blurredText);
                    }
                }
            }
            window.draw(HangarItems[i]);
        }
    }
    void moveUp() {
        if (selectedItemIndex - 1 >= 0) {
            HangarItems[selectedItemIndex].setFillColor(sf::Color::Green);
            selectedItemIndex--;
            HangarItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }

    void moveDown() {
        if (selectedItemIndex + 1 < HangarItems.size()) {
            HangarItems[selectedItemIndex].setFillColor(sf::Color::Green);
            selectedItemIndex++;
            HangarItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }
    int getSelectedItemIndex() {
        return selectedItemIndex;
    }

};

int main()
{   // NOTE: empty texture used later to use map
    sf::Texture TileSheet;
    sf::Texture TankUnit;




    sf::Texture PlayerCursor("assets/unit.jpg");
    sf::Sprite SpritePlayerCursor(PlayerCursor);
    SpritePlayerCursor.setTextureRect(sf::IntRect({256, 256},{64, 64}));
    SpritePlayerCursor.setPosition({0,0});
    // NOTE: MAPS:
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
    // NOTE: PLAYER:
    int currentSelected = 0;
    int PlayerMoney = 20;
    std::vector<Unit> PlayerUnits;
    PlayerUnits.push_back(Unit("Tank",      1, 1, 20, 10, 16, 2, 3, 1, 0, 1, TankUnit));
    PlayerUnits.push_back(Unit("HoverTank", 2, 1, 15, 4, 3, 5, 1, 0, 0, 2, TankUnit));
    PlayerUnits.push_back(Unit("Mech",      3, 1, 25, 6, 2, 3, 2, 0, 0, 3, TankUnit));
    PlayerUnits.push_back(Unit("HeavyTank", 4, 1, 30, 5, 4, 2, 3, 1, 0, 4, TankUnit));
    // NOTE: ENEMY:
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

    // INFO: 0 - garaż, 1-4 - aktywne poziomy

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
                        std::cout << "Mission 1" << std::endl;
                        MaxWidth = test["Width1"];
                        MaxHeight = test["Height1"];
                        level = 1;
                    } else if (selectedItem == 1) {
                        std::cout << "Mission 2" << std::endl;
                        MaxWidth = test["Width2"];
                        MaxHeight = test["Height2"];
                        level = 2;
                    } else if (selectedItem == 2) {
                        std::cout << "Mission 3" << std::endl;
                        MaxWidth = test["Width3"];
                        MaxHeight = test["Height3"];
                        level = 3;
                    } else if (selectedItem == 3) {
                        std::cout << "Mission 4" << std::endl;
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
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
                    state = 4;

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
