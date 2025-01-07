#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

class Menu {
public:
    Menu(float width, float height, nlohmann::json& settings);
    void draw(sf::RenderWindow &window);
    void moveUp();
    void moveDown();
    void setMenuPage(int index, int menuPage);
    void setSetting(int settingType, sf::RenderWindow& window);
    int getCurrentPageIndex() const;
    int getSelectedItemIndex() const;

private:
    int selectedItemIndex;
    int currentPageIndex = 0;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<sf::Text> mainItems;
    std::vector<sf::Text> settingsItems;
    nlohmann::json& settings;
};