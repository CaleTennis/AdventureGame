#include "Player.hpp"

// Must be called after all components have been initialized
void Player::setup()
{
	fistWeapon_ = (new Weapon("Fists", "none"))->setDamage((float)getLevel() * FIST_DAMAGE_MULTIPLIER);
	fistWeapon_->setRarity("none");
	currentWeapon_ = fistWeapon_;
	return;
}

Player* Player::updateGoldAmount(int amt)
{
	if ((goldAmount_ + amt) < 0)
		goldAmount_ = 0;
	else
		goldAmount_ += amt;
	if (amt > 0)
		goldCollected_ += amt;
	return this;
}
Player* Player::setGoldAmount(int amt)
{

	if (amt > goldAmount_)
		goldCollected_ += (long long)(amt - (long long)goldAmount_);

	if (amt < 0)
		goldAmount_ = 0;
	else
		goldAmount_ = amt;
	return this;
}

Player* Player::addExperience(long long n)
{
	experience_ += n;
	lifetimeExperience_ += n;
	updateLevel();
	return this;
}

long long Player::experienceCurve()
{
	return ((((long long)2 * (long long)getLevel()) + (long long)1) * (long long)57);
	// Sample levels:
	// 1: 57XP

}

void Player::updateLevel()
{
	if (experience_ > experienceCurve())
	{
		while (experience_ > experienceCurve())
		{
			levelUp();
			fistWeapon_->setDamage(getLevel() * FIST_DAMAGE_MULTIPLIER);
			experience_ -= experienceCurve();
			if (experience_ < 0)
				experience_ = 0;
		}
	}
}

bool Player::isBackpackFull()
{
	if (weaponInventory_.size() >= BACKPACK_MAX_SIZE)
		return true;
	else
		return false;
}

EncounterType Player::explore()
{
	timesExplored_++;
	int roll = Random::rand_int(0, 100);
	EncounterType encounterType = ET_NOTHING;

	if (roll < SINGLE_ENEMY_ENCOUNTER_RATE)
		encounterType = ET_BATTLE;
	else
		encounterType = ET_NOTHING;

	return encounterType;
}

void Player::rest()
{
	timesRested_++;

	healthHealed_ += (getMaxHealth() - getHealth());
	setHealth(getMaxHealth());
	return;
}

Weapon* Player::setWeapon(Weapon* weapon)
{
	if (weapon == nullptr)
	{
		currentWeapon_ = fistWeapon_;
	}
	else
	{
		currentWeapon_ = weapon;
	}
	return currentWeapon_;
}

float Player::getDamage()
{
	float dmg = 0;
	dmg += (*currentWeapon_).getDamage();
	return dmg;
};

float Player::getAttackDamage()
{
	float dmg = 0;
	dmg += (*currentWeapon_).getDamage();
	lifetimeDamage_ += (long long)dmg;
	if (landsAttack())
	{
		if (Random::fifty_fifty())
		{
			return dmg;
		}
		else
		{
			return dmg * 0.5f;
		}
	}
	else
		return 0;
	return dmg;
}

float Player::takeDamage(float dmg)
{
	updateHealth(-dmg);
	damageTaken_ += dmg;
	return getHealth();
}

bool Player::landsAttack() {
	if (Random::rand_int(0, 101) < (currentWeapon_->getAccuracy() + accuracyModifier_))
		return true;
	else
		return false;
}