#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "Entity.hpp"
#include "GameObject.hpp"
#include "Random.hpp"
#include "Weapon.hpp"
#include "LootTable.hpp"

class Player;
class Entity;
class Display;
class GameObject;

class Enemy : public Entity
{
private:
	const float ENEMY_HEALTH_GROWTH_RATE = 57.0f;
	long long expRewardOnDeath_;
	long long goldRewardOnDeath_;
	Weapon* currentWeapon_ = nullptr;

public:
	void setExpRewardOnDeath(long long e);
	long long getExpRewardOnDeath() const;
	float getDamage() const;
	Enemy();
	~Enemy();
};
#endif