#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <iostream>
#include <string>
#include <cstring>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include "Display.hpp"

#include "GameState.hpp"
#include "GameCommand.hpp"
#include "Random.hpp"
#include "ContextMenu.hpp"

#include "Weapon.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

class Enemy;
class Display;
class Player;
class Entity;

class GameObject
{
protected:
	static const clock_t CMD_DELAY = 100; // Milliseconds

	static Player player;

	void inventoryMoveSelectionUp();
	void inventoryMoveSelectionDown();

public:
	GameObject() {}
	~GameObject() {}

	static GameState state;
	static bool isGameOver;
	static int currentInventoryItemIndex;
	static Enemy* currentEnemy;

	static Player* getPlayer();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	void setup(); 
	void gameOver();
	static void forceCloseGame();
};

#endif