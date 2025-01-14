#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include "unit.h"
using json = nlohmann::json;

int UnitMap[20][20] = {0};  
float dmgEff[3][2] = {
    {1.0,  0.5 }, 
    {0.5,  1.0 }, 
    {0.75, 0.75}
}; 

Unit::Unit(std::string NAME, int ID, int IDOwner, int MAXHEALTH, int DAMAGE, int RANGE, int MOVEMENT, int DT, int AT, int X, int Y, sf::Texture &UnitTexture):  sprite(UnitTexture) {
    // if (UnitTexture.loadFromFile("assets/unit.jpg")) {
        if(ID == 1) {
            sprite.setTextureRect(sf::IntRect({0, 0},{64, 64}));
            sprite.setPosition({float(X*64), float(Y*64)});
        }
        if(ID == 2) {
            // sprite.setTextureRect(sf::IntRect({64, 0},{64, 64}));
            sprite.setTextureRect(sf::IntRect({0, 0},{64, 64}));
            sprite.setPosition({float(X*64), float(Y*64)});
        }
        if(ID == 3) {
            sprite.setTextureRect(sf::IntRect({0, 0},{64, 64}));
            sprite.setPosition({float(X*64), float(Y*64)});
        }
        if(ID == 4) {
            // sprite.setTextureRect(sf::IntRect({64, 0},{64, 64}));
            sprite.setTextureRect(sf::IntRect({0, 0},{64, 64}));
            sprite.setPosition({float(X*64), float(Y*64)});
        }

        Name = NAME;
        id = ID;
        idOwner = IDOwner;
        maxHealth = MAXHEALTH;
        Health = maxHealth;
        damage = DAMAGE;
        range = RANGE;
        movement = MOVEMENT;
        maxMovement = movement;
        damageType = DT;
        armorType = AT;
        movePoints = 1;
        x = X;
        y = Y;
        UnitMap[x][y] = idOwner;
    // }
}

void Unit::takeDamage(int dmg, int dT) {
    Health -= (dmg * dmgEff[dT][armorType]);
    if (Health <= 0) {
        Health = 0;
    }
}

int Unit::GetOwnerID() {
    return idOwner;
}

void Unit::move(int X, int Y, int maxX, int maxY) {
    if (movePoints < 1)
        return;
    if(movement < 1)
        return;
    if (isOccupied(x+X, y+Y))
        return;
    if(x+X > maxX-1 || y+Y > maxY-1 || y+Y < 0 || x+X < 0)
        return;
    UnitMap[x][y] = 0;
    x += X;
    y += Y;
    UnitMap[x][y] = idOwner;
    movement-=1;
    sprite.setPosition({float(x*64), float(y*64)});
}

void Unit::Attack(Unit enemy) {
    if (movePoints < 1)
        return;
    int dist = (abs(enemy.x-x)/64 + abs (enemy.y-y))/64;
    if(dist>range)
        return;
    if(enemy.GetOwnerID() == idOwner)
        return;
    enemy.takeDamage(damage, damageType);
    movePoints -= 1;
}
    
void Unit::DrawUnit(sf::RenderWindow &window) {
        window.draw(sprite);
}

void Unit::UpgradeHealth(int newHealth) {
    maxHealth = newHealth;
    Health = newHealth;
}

void Unit::UpgradeRange(int newRange) {
    range = newRange;
}
    
void Unit::UpgradeMovement(int newMovement) {
    movement = newMovement;
}

void Unit::UpgradeDamage(int newDamage) {
    damage = newDamage;
}

void Unit::resetMoveAndAtack() {
    movePoints = 1;
    movement = maxMovement;
}

int Unit::isOccupied(int x, int y){
    if(UnitMap[x][y] > 0)
        return 1;
    return 0;
}
