#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;
int main()
{
    ifstream f("./assets/test.json");
    json data = json::parse(f);
    auto window = sf::RenderWindow(sf::VideoMode({(data["width"]), (data["height"])}), "CMake SFML Project");
    window.setFramerateLimit(int(data["framerate"]));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear();

        sf::Font font("./assets/arial.ttf");
        sf::Text text(font);
        text.setString("Hey");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        window.draw(text);

        window.display();
    }
}
