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
	std::wstring itemRarityWText_ = L"NULL_RARITY";
	std::string itemRarityText_ = "NULL_RARITY";
	std::string itemName_ = "NULL_NAME";
	std::wstring itemWName_ = L"NULL_NAME";
	int itemLevel_ = 1;
protected:
	ItemType setType(ItemType type);
	static std::unordered_map <std::wstring, ItemRarity> const rarityTable_;

public:
	Item() { return; };
	~Item() { return; };

	ItemType getType() const { return itemType_; }

	std::wstring getWName() const { return itemWName_; }
	std::wstring setWName(std::wstring name) {
		itemWName_ = name;
		itemName_ = std::string(itemWName_.begin(), itemWName_.end());
		return itemWName_;
	}

	std::string getName() const { return itemName_; }
	std::string setName(std::string name) {
		itemName_ = name;
		itemWName_ = std::wstring(itemName_.begin(), itemName_.end());
		return itemName_;
	}


	int getLevel() const { return itemLevel_; };
	int setLevel(int lvl);

	bool setRarity(std::wstring rarity);
	ItemRarity getRarity() { return itemRarity_; }
	std::wstring getRarityWText() { return itemRarityWText_; }
	std::string getRarityText() { return itemRarityText_;  }
};

#endif