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
    int getSelectedItemIndex() const;
    void setSelectedItemIndex(int index);
    void setCurrentPageIndex(int index);
    int getCurrentPageIndex() const;

private:
    int selectedItemIndex;
    int currentPageIndex = 0;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    std::vector<sf::Text> menuItems;
    std::vector<sf::Text> settingsItems;
    nlohmann::json& settings;
};