#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


// TODO: move class and SetupMap to external file
class Ground {
    sf::Texture GroundTexture{("./assets/ground.png")};
    sf::Sprite GroundSprite{(GroundTexture)};
    float X {0}, Y {0};
public:
    Ground (int a, int b) {
        X+=a;
        Y+=b;
        GroundSprite.setPosition ({X, Y});
    }
    sf::Sprite PrintSprite() {
        return GroundSprite; 
    }
};
Ground** SetupMap(int MaxWidth, int MaxHeight)
{
    int increment = 0;
    Ground **MapArray1 = new Ground *[MaxWidth*MaxHeight];
    for (int i = 0; i < MaxWidth; i++)
    {
        for (int j = 0; j < MaxHeight; j++){
            MapArray1[increment] = new Ground(i*200, j*200);
            increment++;
        }
    }
    return MapArray1;
}

int main()
{
    int MaxWidth = 10, MaxHeight = 20;
    Ground **test = SetupMap(MaxWidth, MaxHeight);

    sf::Font font("./assets/arial.ttf");
    sf::Text text(font);
    text.setString("Hey");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Red);

    sf::View view1(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));


    // NOTE: config.json is a config file (wow) that contains widthm height and famerate 
    std::fstream f("./assets/config.json");
    json data = json::parse(f);

    // NOTE: creating and setting up window 
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

        int positionX = view1.getCenter().x;
        int positionY = view1.getCenter().y;
        float MovingValue = 5.f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && positionX > 0)
            view1.move({-MovingValue, 0});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && positionY > 0)
            view1.move({0, -MovingValue});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && positionY < MaxHeight*200)
            view1.move({0, MovingValue});
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && positionX < MaxWidth*200)
            view1.move({MovingValue, 0});

        window.clear();
        window.setView(view1);
        window.draw(text);
        int increment = 0;
        for (int i = 0; i < MaxWidth; i++)
        {
            for (int j = 0; j < MaxHeight; j++) 
            {

                window.draw(test[increment++]->PrintSprite());
            }
        }
        
        window.display();
    }
}
