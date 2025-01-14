#include "tile.h"
#include <SFML/Graphics.hpp>

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