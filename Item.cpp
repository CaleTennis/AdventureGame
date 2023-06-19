#include "Item.hpp"

std::unordered_map<std::string, ItemRarity> const Item::rarityTable_ = {
		   {"none", ItemRarity::NONE},
		   {"junk", ItemRarity::JUNK},
		   {"common", ItemRarity::COMMON},
		   {"unusual", ItemRarity::UNUSUAL},
		   {"rare", ItemRarity::RARE},
		   {"mythical", ItemRarity::MYTHICAL},
		   {"artifact", ItemRarity::ARTIFACT}
};

ItemType Item::setType(ItemType type)
{
	itemType_ = type;
	return itemType_;
}

std::string Item::setName(std::string name)
{
	itemName_ = name;
	return itemName_;
}

int Item::setLevel(int lvl)
{
	if (lvl <= 1)
		itemLevel_ = 1;
	else
		itemLevel_ = lvl;
	return itemLevel_;
}

bool Item::setRarity(std::string rarity)
{
	for (int i = 0; i < strlen(rarity.c_str()); i++)
		rarity[i] = putchar(tolower(rarity[i]));
	
	auto it = Item::rarityTable_.find(rarity.c_str());

	if (it != Item::rarityTable_.end())
	{
		std::string rarityTxt = it->first;
		rarityTxt[0] = putchar(toupper(rarityTxt[0]));
		itemRarityText_ = rarityTxt;
		itemRarity_ = it->second;
	}
	else
		return false;

	return true;
}