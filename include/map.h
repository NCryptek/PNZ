#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>  
#include <vector>
#include "tile.h"  

class Map {
private:
    int MaxWidth;                          
    int MaxHeight;                         
    std::vector<Tile> MapOfTheLevel;       

public:
    Map(int Width, int Height, std::vector<int> map, sf::Texture &texture);
    void DrawTheLevel(sf::RenderWindow &window);
};

#endif  
