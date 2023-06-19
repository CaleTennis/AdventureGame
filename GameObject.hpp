#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <time.h>

#include "GameState.hpp"
#include "GameCommand.hpp"
#include "Random.hpp"
#include "ContextMenu.hpp"

#include "Weapon.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

class Enemy;

class GameObject
{
protected:
	const clock_t CMD_DELAY = 100; // Milliseconds


	char inputChar = 0;
	GameCommand currentCmd = CMD_BLANK;
	ContextMenu currentMenu = START_MENU;
	int currentInventoryItemIndex = 1;
	static Player player;
	static Enemy* currentEnemy;

	void inventoryMoveSelectionUp();
	void inventoryMoveSelectionDown();

public:
	GameObject() { return; };
	~GameObject() { return; };

	GameState state = STATE_SETUP;
	bool isGameOver = false;

	static Player* getPlayer();

	void setup(); 
	void getInput();
	void parseInput();
	void validateInput();
	void processEvents();
	void updateDisplay();
	void quickCleanup();
	void fullCleanup();
	void gameOver();
};

#endif