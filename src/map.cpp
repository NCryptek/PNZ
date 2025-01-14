#include "map.h"

Map::Map(int Width, int Height, std::vector<int> map, sf::Texture &texture) {
    MaxWidth = Width;
    MaxHeight = Height;
    for (int i = 0; i < MaxHeight; i++) {
        for (int j = 0; j < MaxWidth; j++) {
            Tile tmp(map[i*MaxWidth + j], j, i, texture);
            MapOfTheLevel.push_back(tmp);
        }
    }
}

void Map::DrawTheLevel(sf::RenderWindow &window) {
    for (int i = 0; i < MaxHeight; i++) {
        for (int j = 0; j < MaxWidth; j++) {
            window.draw(MapOfTheLevel[i*MaxWidth+j].Draw());
        }
    }
}

