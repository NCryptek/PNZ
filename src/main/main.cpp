#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
using namespace std;
int main()
{
    ifstream ConfigFile;
    string one;
    ConfigFile.open("./assets/config.txt");
    ConfigFile >> one;
    cout << one ;
    ConfigFile.close();
    ofstream ConfOut("./assets/config.txt");
    ConfOut << "tests";
    ConfOut.close();

    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(60);

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
