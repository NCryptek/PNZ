#include "../include/menu.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Menu::Menu(float width, float height) : backgroundTexture("./assets/menu_background.png"), backgroundSprite(backgroundTexture) {
    if (!font.openFromFile("./assets/arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        std::abort(); // Abort if the font fails to load
    }
    std::cout << "[PNZ] Font loaded successfully in menu!" << std::endl;


    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2f windowSize(width, height);
    // backgroundSprite.setScale(windowSize);
    backgroundSprite.setScale(sf::Vector2f(
        windowSize.x / static_cast<float>(textureSize.x),
        windowSize.y / static_cast<float>(textureSize.y)
    ));

    std::cout << "[PNZ] Background image in menu succesful loaded!" << std::endl;

    std::vector<std::string> items = {"Select campaign", "Settings", "Close"};
    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text(font, items[i], 60);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);

        text.setPosition(sf::Vector2f(width / 2.5, height / (items.size() + 5) * (i + 1.5)));
        // text.setPosition(sf::Vector2f(25.0f, height / (items.size() + 5) * (i + 1)));
        
        menuItems.push_back(text);
    }

    selectedItemIndex = 0;
    menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
}

// void Menu::draw(sf::RenderWindow &window) {
//     window.draw(backgroundSprite);
//     for (const auto &item : menuItems) {
//         window.draw(item);
//     }
// }

void Menu::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);
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
}

void Menu::moveUp() {
    if (selectedItemIndex - 1 >= 0) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::Black);
        selectedItemIndex--;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (selectedItemIndex + 1 < menuItems.size()) {
        menuItems[selectedItemIndex].setFillColor(sf::Color::Black);
        selectedItemIndex++;
        menuItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

int Menu::getSelectedItemIndex() const {
    return selectedItemIndex;
}