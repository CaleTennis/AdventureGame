#include "Item.hpp"

std::unordered_map<std::wstring, ItemRarity> const Item::rarityTable_ = {
		   {L"none", ItemRarity::NONE},
		   {L"junk", ItemRarity::JUNK},
		   {L"common", ItemRarity::COMMON},
		   {L"unusual", ItemRarity::UNUSUAL},
		   {L"rare", ItemRarity::RARE},
		   {L"mythical", ItemRarity::MYTHICAL},
		   {L"artifact", ItemRarity::ARTIFACT}
};

ItemType Item::setType(ItemType type)
{
	itemType_ = type;
	return itemType_;
}

int Item::setLevel(int lvl)
{
	if (lvl <= 1)
		itemLevel_ = 1;
	else
		itemLevel_ = lvl;
	return itemLevel_;
}

bool Item::setRarity(std::wstring rarity)
{
	for (int i = 0; i < wcslen(rarity.c_str()); i++)
		rarity[i] = putchar(tolower(rarity[i]));
	
	auto it = Item::rarityTable_.find(rarity.c_str());

	if (it != Item::rarityTable_.end())
	{
		std::wstring rarityTxt = it->first;
		rarityTxt[0] = putchar(toupper(rarityTxt[0]));
		itemRarityWText_ = rarityTxt;
		itemRarityText_ = std::string(rarityTxt.begin(),rarityTxt.end());
		itemRarity_ = it->second;
	}
	else
		return false;

	return true;
}