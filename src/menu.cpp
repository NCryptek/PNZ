#include "../include/menu.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Menu::Menu(float width, float height, nlohmann::json& settings) : settings(settings), backgroundTexture("./assets/menu_backgroundd.png"), backgroundSprite(backgroundTexture) {
    if (!font.openFromFile("./assets/arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        std::abort(); // Abort if the font fails to load
    }
    std::cout << "[PNZ] Font loaded successfully in menu!" << std::endl;
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2f windowSize(width, height);
    backgroundSprite.setScale(sf::Vector2f(
        windowSize.x / static_cast<float>(textureSize.x),
        windowSize.y / static_cast<float>(textureSize.y)
    ));
    std::cout << "[PNZ] Background image in menu succesful loaded!" << std::endl;

    std::vector<std::string> items = {"Select campaign", "Settings", "Exit"};
    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text(font, items[i], 60);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(width / 2.5, height / (items.size() + 5) * (i + 1.5)));
        
        menuItems.push_back(text);
    }


    std::vector<std::string> items2 = {"Resolution", "Framerate"};        
    for (size_t i = 0; i < items2.size(); ++i) {
        sf::Text text(font, items2[i], 60);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(settings["Screen_Width"] / 2.5, settings["Screen_Height"] / (items2.size() + 5) * (i + 1.5)));
        settingsItems.push_back(text);
    }

    this->settings = settings;

    selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
}

void Menu::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);

    int state = getCurrentPageIndex();
    switch(state) { 
        case 0:
            for (const auto &item : menuItems) {
                sf::Text blurredText = item;
                blurredText.setFillColor(sf::Color(255, 255, 255, 50)); 
                for (int x = -4; x <= 4; ++x) {
                    for (int y = -4; y <= 4; ++y) {
                        if (x != 0 || y != 0) {
                            blurredText.setPosition(item.getPosition() + sf::Vector2f(x, y));
                            window.draw(blurredText);
                        }
                    }
                }
                window.draw(item);
            } 
            break;
        case 1: 
            for (const auto &item : settingsItems) {
                sf::Text blurredText = item;
                blurredText.setFillColor(sf::Color(255, 255, 255, 50)); 
                for (int x = -4; x <= 4; ++x) {
                    for (int y = -4; y <= 4; ++y) {
                        if (x != 0 || y != 0) {
                            blurredText.setPosition(item.getPosition() + sf::Vector2f(x, y));
                            window.draw(blurredText);
                        }
                    }
                }
                window.draw(item);
            } 
            break;
    }
}

void Menu::moveUp() {
    int currentPage = getCurrentPageIndex();
    if (currentPage == 0) { 
        if (selectedItemIndex - 1 >= 0) {
            menuItems[selectedItemIndex].setFillColor(sf::Color::Black);
            selectedItemIndex--;
            menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }
    if (currentPage == 1) { 
        if (selectedItemIndex - 1 >= 0) {
            settingsItems[selectedItemIndex].setFillColor(sf::Color::Black);
            selectedItemIndex--;
            settingsItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }
}

void Menu::moveDown() {
    int currentPage = getCurrentPageIndex();
    if (currentPage == 1) { 
        if (selectedItemIndex + 1 < menuItems.size()) {
            settingsItems[selectedItemIndex].setFillColor(sf::Color::Black);
            selectedItemIndex++;
            settingsItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    } 
    if (currentPage == 0) { 
        if (selectedItemIndex + 1 < menuItems.size()) {
            menuItems[selectedItemIndex].setFillColor(sf::Color::Black);
            selectedItemIndex++;
            menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }
}

int Menu::getSelectedItemIndex() const {
    return selectedItemIndex;
}

void Menu::setSelectedItemIndex(int index) { 
    selectedItemIndex = index;
}

void Menu::setCurrentPageIndex(int index) { 
    currentPageIndex = index;
}

int Menu::getCurrentPageIndex() const{
    return currentPageIndex;
}
