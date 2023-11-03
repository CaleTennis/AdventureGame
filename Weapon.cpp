#include "Weapon.hpp"
#pragma warning(disable : 4267) // Type conversion warning: possible data loss


std::vector<Weapon*> Weapon::weaponList;

Weapon::Weapon(std::wstring weaponName, std::wstring weaponRarity)
{
	setType(ITEM_WEAPON);
	setRarity(weaponRarity);
	//setDamage(weaponDamage);
	setWName(weaponName);
	return;
}

Weapon::~Weapon()
{}


bool Weapon::initWeaponList()
{
	enum WeaponInitStep {
		NAME,
		RARITY
	} currentWeaponInitState = NAME;

	std::wstring weaponName = L"";
	std::wstring weaponRarity = L"";
	
	char nextChar = '\0';

	std::ifstream weapon_data("WeaponList.txt");

	if (weapon_data.is_open())
	{
		while (weapon_data.good())
		{
			nextChar = weapon_data.get();
			if (nextChar == '\n')
			{
				Weapon::weaponList.push_back(new Weapon(weaponName, weaponRarity));
				weaponName = L"";
				weaponRarity = L"";
				currentWeaponInitState = NAME;
			}
			else if (nextChar == ',')
			{
				currentWeaponInitState = RARITY;
			}
			else if (currentWeaponInitState == NAME)
			{
				weaponName += nextChar;
			}
			else if (currentWeaponInitState == RARITY)
			{
				weaponRarity += nextChar;
			}
			else
			{
				continue;
			}
		}
	}
	else
	{
		return false;
	}


	if (Weapon::weaponList.empty())
	{
		return false;
	}

	return true;
}

Weapon* Weapon::randomWeapon()
{
	Weapon* weapon = Weapon::weaponList.at(Random::rand_int(0, Weapon::weaponList.size()));
	return weapon;
}

Weapon* Weapon::setDamage(float dmg)
{
	weaponDamage_ = dmg;
	return this;
}

Weapon* Weapon::setDamageAndLevelAutomatically(int playerLevel)
{
	double dmg = playerLevel * Random::rand_int(50, 70);
	switch (getRarity())
	{
	case JUNK:
	{
		dmg *= WDM_JUNK;
	} break;
	case COMMON:
	{
		dmg *= WDM_COMMON;
	} break;
	case UNUSUAL:
	{
		dmg *= WDM_UNUSUAL;
	} break;
	case RARE:
	{
		dmg *= WDM_RARE;
	} break;
	case MYTHICAL:
	{
		dmg *= WDM_MYTHICAL;
	} break;
	case ARTIFACT:
	{
		dmg *= WDM_ARTIFACT;
	} break;
	}
	setDamage(dmg);
	setLevel(playerLevel);
	return this;
}

Weapon* Weapon::setAccuracy(float acc)
{
	if (acc >= 0.0f)
	{
		if (acc <= 100.0f)
			weaponAccuracy_ = acc;
		else
			weaponAccuracy_ = 100;
	} 
	else
	{
		weaponAccuracy_ = 0.0f;
	}

	return this;
}