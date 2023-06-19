#include "Enemy.hpp"

Enemy::Enemy()
{
	int levelRoll = Random::rand_int(1,4);
	if (levelRoll == 1)
		setLevel(GameObject::getPlayer()->getLevel() - 1);
	else if (levelRoll == 2)
		setLevel(GameObject::getPlayer()->getLevel());
	else if (levelRoll == 3)
		setLevel(GameObject::getPlayer()->getLevel() + 1);
	else
		setLevel(GameObject::getPlayer()->getLevel());
	
	if (getLevel() == 0)
		setLevel(1);

	setName("Level " + std::to_string(getLevel()) + " Enemy");
	setHealth(setMaxHealth((getLevel() * (ENEMY_HEALTH_GROWTH_RATE - 1)) + 100.0f));
	
	expRewardOnDeath_ = (long long)25 * (long long)getLevel();

	goldRewardOnDeath_ = (long long)GameObject::getPlayer()->getLevel() * (long long)getLevel();

	return;
}

Enemy::~Enemy()
{
	if (!isAlive())
	{
		GameObject::getPlayer()->addExperience(expRewardOnDeath_);
		GameObject::getPlayer()->updateGoldAmount((int)goldRewardOnDeath_);
		GameObject::getPlayer()->enemiesSlain++;

		LootTable lt = LootTable();
		ItemRarity dropRarity = lt.calculateLootRarity();

		if (dropRarity == NONE || GameObject::getPlayer()->isBackpackFull())
			return;

		Weapon* weaponDropPtr = Weapon::randomWeapon();

		while (weaponDropPtr->getRarity() != dropRarity)
			weaponDropPtr = Weapon::randomWeapon();

		Weapon* weaponDrop = new Weapon(*weaponDropPtr);
		weaponDrop->setDamageAndLevelAutomatically(GameObject::getPlayer()->getLevel());

		GameObject::getPlayer()->getWeaponInventory()->push_back(weaponDrop);
		GameObject::getPlayer()->itemsCollected++;

		return;
	}
	else
	{
		return;
	}
}


void Enemy::setExpRewardOnDeath(long long e)
{
	expRewardOnDeath_ = e;
	return;
}

long long Enemy::getExpRewardOnDeath() const
{
	return expRewardOnDeath_;
}

float Enemy::getDamage() const
{
	float dmg = (float)getLevel();
	//dmg += currentWeapon_->getDamage();
	return dmg;
}