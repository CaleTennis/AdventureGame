#ifndef __LOOTTABLE_HPP__
#define __LOOTTABLE_HPP__

#include "Weapon.hpp"
#include "Entity.hpp"
#include "Armor.hpp"
#include "ItemRarity.hpp"
#include "Random.hpp"

class LootTable
{
private:
	// DROP PROBABILITY OF EACH TIER OF WEAPON
	
	const double W_NONE = 0;					
	const double W_JUNK = 25;		// 25					
	const double W_COMMON = 60;		// 60			
	const double W_UNUSUAL = 80;	// 80			
	const double W_RARE = 90;	    // 90		
	const double W_MYTHICAL = 99;	// 99			
	const double W_ARTIFACT = 100;	// 100			

public:
	ItemRarity calculateLootRarity();
};

#endif