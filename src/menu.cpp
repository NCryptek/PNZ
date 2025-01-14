#include "menu.h"

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

    int fontSize = static_cast<int>(std::min(width, height) * 0.05f);

    std::vector<std::string> items = {"Select campaign", "Settings", "Exit"};
    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text(font, items[i], fontSize);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(width / 2.5, 800 / (items.size() + 5) * (i + 1.5)));
        
        mainItems.push_back(text);
    }

    std::string resolution = "Resolution: " + to_string(settings["Screen_Width"]) + "x" + to_string(settings["Screen_Height"]);
    std::string framerate = "Framerate: " + to_string(settings["Framerate"]);
    std::vector<std::string> items2 = {resolution, framerate, "Cofnij"};        
    for (size_t i = 0; i < items2.size(); ++i) {
        sf::Text text(font, items2[i], fontSize);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(width / 2.5, 800 / (items2.size() + 5) * (i + 1.5)));
        settingsItems.push_back(text);
    }

    this->settings = settings;

    selectedItemIndex = 0;
    mainItems[selectedItemIndex].setFillColor(sf::Color::Red);
}

void Menu::draw(sf::RenderWindow &window) {
    window.draw(backgroundSprite);

    int state = getCurrentPageIndex();
    switch(state) { 
        case 0:
            for (const auto &item : mainItems) {
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
            mainItems[selectedItemIndex].setFillColor(sf::Color::Black);
            selectedItemIndex--;
            mainItems[selectedItemIndex].setFillColor(sf::Color::Red);
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
        if (selectedItemIndex + 1 < settingsItems.size()) {
            settingsItems[selectedItemIndex].setFillColor(sf::Color::Black);
            selectedItemIndex++;
            settingsItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    } 
    if (currentPage == 0) { 
        if (selectedItemIndex + 1 < mainItems.size()) {
            mainItems[selectedItemIndex].setFillColor(sf::Color::Black);
            selectedItemIndex++;
            mainItems[selectedItemIndex].setFillColor(sf::Color::Red);
        }
    }
}

int Menu::getSelectedItemIndex() const {
    return selectedItemIndex;
}

void Menu::setMenuPage(int index, int menuPage) { 
    int oldIndex = selectedItemIndex;
    selectedItemIndex = index;
    currentPageIndex = menuPage;
    if (menuPage == 1) { 
        mainItems[oldIndex].setFillColor(sf::Color::Black);
        settingsItems[index].setFillColor(sf::Color::Red);
    }   
    if (menuPage == 0) { 
        settingsItems[oldIndex].setFillColor(sf::Color::Black);
        mainItems[index].setFillColor(sf::Color::Red);
    }
}

int Menu::getCurrentPageIndex() const{
    return currentPageIndex;
}

void Menu::setSetting(int settingType, sf::RenderWindow& window) { 
    if (settingType == 1) { 
        int width[3] = {1920, 1280, 800};
        int height[3] = {1080, 720, 600};
        
        int currentWidth = settings["Screen_Width"];
        int currentHeight = settings["Screen_Height"];

        for (int i = 0; i < 3; ++i) {
            if (width[i] == currentWidth && height[i] == currentHeight) {
                int nextIndex = (i + 1) % 3; 
                settings["Screen_Width"] = width[nextIndex];
                settings["Screen_Height"] = height[nextIndex];

                window.setSize(sf::Vector2u(width[nextIndex], height[nextIndex]));
                std::string resolution = "Resolution: "  + std::to_string(width[nextIndex]) + "x" + std::to_string(height[nextIndex]);
                settingsItems[0].setString(resolution);

                int fontSize = static_cast<int>(std::min(width[nextIndex], height[nextIndex]) * 0.05f);
                for (auto &item : settingsItems) item.setCharacterSize(fontSize);
                for (auto &item : mainItems) item.setCharacterSize(fontSize);

                std::ofstream outFile("./config/settings.json");
                outFile << settings.dump(4);
                outFile.close();

                break;
            }
        }
    } else { 
        int framerate [3] = {60, 30}; 
        int currentFramerate = settings["Framerate"];
        for (int i = 0; i < 2; ++i) {
            if (framerate[i] == currentFramerate) {
                int nextIndex = (i + 1) % 2; 
                settings["Framerate"] = framerate[nextIndex];

                window.setFramerateLimit(framerate[nextIndex]);

                std::string frame = "Framerate: "  + std::to_string(framerate[nextIndex]);
                settingsItems[1].setString(frame); 

                std::ofstream outFile("./config/settings.json");
                outFile << settings.dump(4);
                outFile.close();

                break;
            }
        }
    }
}
