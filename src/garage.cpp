#include "garage.h"  

Garage::Garage(float width, float height): sprite(texture){
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


void Garage::Draw(sf::RenderWindow &window) {
    window.draw(sprite);
    for (int i = 0; i < HangarItems.size(); i++) {
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

void Garage::moveUp() {
    if (selectedItemIndex - 1 >= 0) {
        HangarItems[selectedItemIndex].setFillColor(sf::Color::Green);
        selectedItemIndex--;
        HangarItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}

void Garage::moveDown() {
    if (selectedItemIndex + 1 < HangarItems.size()) {
        HangarItems[selectedItemIndex].setFillColor(sf::Color::Green);
        selectedItemIndex++;
        HangarItems[selectedItemIndex].setFillColor(sf::Color::Red);
    }
}
int Garage::getSelectedItemIndex() {
    return selectedItemIndex;
}
