#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "Entity.hpp"
#include "Weapon.hpp"
#include "EncounterType.hpp"
#include "Random.hpp"

class Player : public Entity
{
private:
	const float FIST_DAMAGE_MULTIPLIER = 20.0f;
	const int BACKPACK_MAX_SIZE = 20;
	const float SINGLE_ENEMY_ENCOUNTER_RATE = 100.0f;
	
	long long experience_ = 0;
	int goldAmount_ = 0;
	
	float accuracyModifier_ = 0.0f;

	Weapon* fistWeapon_ = nullptr;
	Weapon* currentWeapon_ = nullptr;

	std::vector<Weapon*> weaponInventory_;

	void updateLevel();
	bool landsAttack();

	long long timesExplored_ = 0;
	long long timesRested_ = 0;
	long long lifetimeExperience_ = 0;
	long long goldCollected_ = 0;

	//TODO: Fix broken stats
	long long lifetimeDamage_ = 0; 
	long double healthHealed_ = 0; 
	long double damageTaken_ = 0;  
public:
	Player() { return; }
	~Player() { return; }


	void setup();

	//Experience
	long long experienceCurve();
	Player* addExperience(long long n);
	long long getExperience() const { return experience_; }
	
	//Gold
	int getGoldAmount() const { return goldAmount_; }
	Player* updateGoldAmount(int amt);
	Player* setGoldAmount(int amt);

	Weapon* getWeapon() { return currentWeapon_; }
	Weapon* setWeapon(Weapon* weapon);
	
	std::vector<Weapon*>* getWeaponInventory() { return &weaponInventory_; }
	bool isBackpackFull();
	int getBackpackMaxSize() { return BACKPACK_MAX_SIZE; }

	float getDamage();
	float getAttackDamage();
	float takeDamage(float dmg);

	//Game
	EncounterType explore();
	void rest();

	//Stats
	const long long& timesExplored = timesExplored_;
	const long long& timesRested = timesRested_;
	const long long& lifetimeExperience = lifetimeExperience_;
	const long long& goldCollected = goldCollected_;
	const long long& lifetimeDamage = lifetimeDamage_;	
	const long double& damageTaken = damageTaken_;		
	const long double& healthHealed = healthHealed_;		
	long long totalActions = 0;
	long long enemiesSlain = 0;
	long long itemsCollected = 0;

};

#endif