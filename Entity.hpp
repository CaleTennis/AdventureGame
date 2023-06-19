#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <string>
#include <iostream>
#include <vector>

class Entity
{
private:
	float health_;
	float maxHealth_;
	int level_;
	std::string name_;
	int entityID_;
	static int entityCount_;
	static std::vector<Entity*> entityList_;
	bool entityCountInitialized_ = false;
	bool alive_ = true;
	void die();

public:
	Entity();
	~Entity();
	
	float getHealth() const;
	float setHealth(float h);
	float updateHealth(float dh);

	float getMaxHealth() const;
	float setMaxHealth(float h);
	float updateMaxHealth(float dh);

	int getLevel() const;
	int setLevel(int l);
	int levelUp();
	int multiLevelUp(int n);

	std::string getName() const;
	std::string setName(std::string n);

	int getEntityID() const;
	
	int getEntityCount() const;

	bool isAlive() const;

};

#endif