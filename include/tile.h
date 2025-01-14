#ifndef TILE_H
#define TILE_H

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
    Tile(int typeOfTile, int a, int b, sf::Texture &tileSheetTexture);  
    sf::Sprite Draw();  
};

#endif  

