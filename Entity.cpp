#include "Entity.hpp"

int Entity::entityCount_ = 0;
std::vector<Entity*> Entity::entityList_ = std::vector<Entity*>();

Entity::Entity()
{
	health_ = 100.0f;
	maxHealth_ = health_;
	level_ = 1;
	name_ = "entity";
	entityCount_++;
	entityID_ = entityCount_;
	entityList_.push_back(this);
	return;
}

Entity::~Entity()
{
	Entity::entityCount_--;
	std::vector<Entity*>::iterator it = std::find(entityList_.begin(), entityList_.end(), this);
	if (it != entityList_.end())
	{
		entityList_.erase(it);
	}
	return;
}

float Entity::getHealth() const
{
	return health_;
}

float Entity::setHealth(float h)
{
	health_ = h;
	if (health_ <= 0)
		die();
	else if (health_ > maxHealth_)
		health_ = maxHealth_;
	return health_;
}

float Entity::updateHealth(float dh)
{
	health_ += dh;
	if (health_ <= 0)
		die();
	else if (health_ > maxHealth_)
		health_ = maxHealth_;
	return health_;
}


float Entity::getMaxHealth() const
{
	return maxHealth_;
}

float Entity::setMaxHealth(float h)
{
	maxHealth_ = h;
	if (health_ > maxHealth_)
		health_ = maxHealth_;
	if (maxHealth_ <= 0)
		die();
	return maxHealth_;
}

float Entity::updateMaxHealth(float dh)
{
	maxHealth_ += dh;
	if (health_ > maxHealth_)
		health_ = maxHealth_;
	if (maxHealth_ <= 0)
		die();
	return maxHealth_;
}

int Entity::getLevel() const
{
	return level_;
}

int Entity::setLevel(int l)
{
	level_ = l;
	return level_;
}

int Entity::levelUp()
{
	return ++level_;
}

int Entity::multiLevelUp(int n)
{
	level_ += n;
	return level_;
}


std::string Entity::getName() const
{
	return name_;
}

std::string Entity::setName(std::string n)
{
	name_ = n;
	return name_;
}

int Entity::getEntityID() const
{
	return entityID_;
}

int Entity::getEntityCount() const
{
	return entityCount_;
}

void Entity::die()
{
	alive_ = false;
	return;
}

bool Entity::isAlive() const
{
	return alive_;
}