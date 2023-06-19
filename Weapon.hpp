#ifndef __WEAPON_HPP__
#define __WEAPON_HPP__

#include "Item.hpp"
#include <fstream>
#include <iostream>
#include <string> 
#include <vector>
#include "Random.hpp"

class Weapon :
    public Item
{
private:
    float weaponDamage_ = 10.0f; // Default value = 10.0f
    float weaponAccuracy_ = 75.0f; // Default value = 50.0f

    // Weapon rarity damage modifiers
    const double WDM_JUNK = 0;
    const double WDM_COMMON = 1;
    const double WDM_UNUSUAL = 1.025;
    const double WDM_RARE = 1.05;
    const double WDM_MYTHICAL = 1.10;
    const double WDM_ARTIFACT = 1.15;

public:
    Weapon(); // Default weapon, fists
    Weapon(std::string weaponName, std::string weaponRarity);
    ~Weapon();

    static std::vector<Weapon*> weaponList;
    static bool initWeaponList();
    static Weapon* randomWeapon();

    float getDamage() { return weaponDamage_; }
    Weapon* setDamage(float dmg);
    Weapon* setDamageAndLevelAutomatically(int playerLevel);

    float getAccuracy() { return weaponAccuracy_; }
    Weapon* setAccuracy(float acc);

};

#endif