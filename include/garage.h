#ifndef GARAGE_H
#define GARAGE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>


class Garage {
private:
    sf::Sprite sprite;                   
    sf::Texture texture;                  
    std::vector<sf::Text> HangarItems;    
    sf::Font font;                        
    int selectedItemIndex;                

public:
    Garage(float width, float height);
    void Draw(sf::RenderWindow &window);
    void moveUp();
    void moveDown();
    int getSelectedItemIndex();
};

#endif  
