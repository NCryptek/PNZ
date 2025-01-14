#ifndef UNIT_H
#define UNIT_H

#include <SFML/Graphics.hpp>
#include <string>

extern int UnitMap[20][20];  
extern float dmgEff[3][2]; 

class Unit {
    sf::Sprite sprite;
    std::string Name;
    int id;
    int idOwner;
    int maxHealth;
    int damage;
    int range;
    int maxMovement;
    int movement;
    int damageType;
    int armorType;
    
public:
    int x;
    int Health;
    int y;
    int movePoints;

    Unit(std::string NAME, int ID, int IDOwner, int MAXHEALTH, int DAMAGE, int RANGE, 
         int MOVEMENT, int DT, int AT, int X, int Y, sf::Texture &UnitTexture);

    void takeDamage(int dmg, int dT);
    int GetOwnerID();
    void move(int X, int Y, int maxX, int maxY);
    void Attack(Unit enemy);
    void DrawUnit(sf::RenderWindow &window);
    void UpgradeHealth(int newHealth);
    void UpgradeRange(int newRange);
    void UpgradeMovement(int newMovement);
    void UpgradeDamage(int newDamage);
    void resetMoveAndAtack();
    int isOccupied(int x, int y);
};

#endif