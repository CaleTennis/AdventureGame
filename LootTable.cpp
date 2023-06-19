#include "LootTable.hpp"

ItemRarity LootTable::calculateLootRarity()
{
	int roll = Random::rand_int(0, 101);
	if (roll < W_JUNK)
		return NONE;
	else if (roll < W_COMMON)
		return JUNK;
	else if (roll < W_UNUSUAL)
		return COMMON;
	else if (roll < W_RARE)
		return UNUSUAL;
	else if (roll < W_MYTHICAL)
		return RARE;
	else if (roll < W_ARTIFACT)
		return MYTHICAL;
	else
		return ARTIFACT;
	return NONE;
}