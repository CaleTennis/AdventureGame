#ifndef __ITEM_HPP__
#define __ITEM_HPP__

#include "ItemType.hpp"
#include "ItemRarity.hpp"
#include <unordered_map>
#include <string>

class Item
{
private:
	ItemType itemType_ = ITEM_BLANK;
	ItemRarity itemRarity_ = NONE;
	std::string itemRarityText_ = "NULL_RARITY";
	std::string itemName_ = "NULL_NAME";
	int itemLevel_ = 1;
protected:
	ItemType setType(ItemType type);
	static std::unordered_map <std::string, ItemRarity> const rarityTable_;

public:
	Item() { return; };
	~Item() { return; };

	ItemType getType() const { return itemType_; }

	std::string getName() const { return itemName_; }
	std::string setName(std::string name);

	int getLevel() const { return itemLevel_; };
	int setLevel(int lvl);

	bool setRarity(std::string rarity);
	ItemRarity getRarity() { return itemRarity_; }
	std::string getRarityText() { return itemRarityText_; }
};

#endif