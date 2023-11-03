#include "GameObject.hpp"
#pragma warning(disable : 26495)

Player GameObject::player = Player();
Enemy* GameObject::currentEnemy = nullptr;
bool GameObject::isGameOver = false;
int GameObject::currentInventoryItemIndex = 1;
GameState GameObject::state = STATE_SETUP;

LRESULT CALLBACK GameObject::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lResult = 0;

	switch (uMsg)
	{
	case WM_COMMAND: {
		switch (LOWORD(wParam))
		{
		case title_start_label: {
			OutputDebugString(L"Switching to game menu...\n");
			Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
			Display::getInstance().updateMenuScreen();
		} break;
		case title_quit_label: {
			OutputDebugString(L"Exiting game...");
			forceCloseGame();
			CloseWindow(hWnd);
		} break;
		case game_explore_label: {
			GameObject::player.totalActions++;

			if (GameObject::player.explore() == ET_BATTLE)
			{
				currentEnemy = new Enemy();
				Display::getInstance().setCurrentMenu(ContextMenu::BATTLE_MENU);
				Display::getInstance().updateMenuScreen();
			}
		} break;
		case game_rest_label: {
			GameObject::player.totalActions++;
			GameObject::player.rest();
		} break;
		case game_exit_label: {
			OutputDebugString(L"Quitting to title screen...\n");
			Display::getInstance().setCurrentMenu(ContextMenu::START_MENU);
			Display::getInstance().updateMenuScreen();
		} break;
		case game_open_inventory_label: {
			Display::getInstance().setCurrentMenu(ContextMenu::INVENTORY_MENU);
			Display::getInstance().updateMenuScreen();
		} break;

		case inventory_listbox_label: {
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE: {
				Display::hasSelectedInventoryItem = true;
				Display::getInstance().updateCurrentInventorySelection();
				Display::getInstance().repaint();

			} break;
			default: {} break;
			}
		} break;

		case inventory_close_label: {
			Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
			Display::getInstance().updateMenuScreen();
			Display::hasSelectedInventoryItem = false;
		} break;
		case battle_attack_label: {
			GameObject::player.totalActions++;

			GameObject::currentEnemy->updateHealth(-GameObject::player.getAttackDamage());
			GameObject::player.takeDamage(GameObject::currentEnemy->getDamage());

			if (!GameObject::currentEnemy->isAlive()) {
				delete currentEnemy;
				Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
				Display::getInstance().updateMenuScreen();
			}
		} break;
		case battle_run_label: {
			GameObject::player.totalActions++;
			delete currentEnemy;
			Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
			Display::getInstance().updateMenuScreen();
		} break;
		default: { 
			if (currentEnemy != nullptr) currentEnemy = nullptr;
		} break;
		}
		Display::getInstance().repaint();
	} break;
	case WM_CLOSE:
	{
		OutputDebugString(L"Processing Window Close event...\n");
		forceCloseGame();
		CloseWindow(hWnd);
	} break;
	case WM_DESTROY:
	{
		OutputDebugString(L"Processing Window Destroy event...\n");
		forceCloseGame();
		PostQuitMessage(NULL);
		return 0;
	} break;
	case WM_PAINT:
	{
		OutputDebugString(L"Processing Window Paint event...\n");
		Display::getInstance().paint();
	} break;
	default:
	{
		lResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
	} break;
	}

	return lResult;
}

void GameObject::setup()
{

	if (!Weapon::initWeaponList())
	{
		printf("Could not initialize weapon list\n");
		GameObject::isGameOver = true;
		GameObject::state = GameState::STATE_FULL_CLEANUP;
		return;
	}

	GameObject::player.setup(); 
	GameObject::player.setWName(L"Player");

	GameObject::state = GameState::STATE_UPDATE_DISPLAY;
	return;
}

void GameObject::gameOver()
{
	GameObject::isGameOver = true;
	return;
}

Player* GameObject::getPlayer()
{
	return &GameObject::player;
}

void GameObject::inventoryMoveSelectionUp()
{
	if (GameObject::currentInventoryItemIndex <= 1)
		GameObject::currentInventoryItemIndex = 1;
	else
		GameObject::currentInventoryItemIndex--;
}

void GameObject::inventoryMoveSelectionDown()
{
	if (GameObject::currentInventoryItemIndex >= GameObject::player.getWeaponInventory()->size())
		GameObject::currentInventoryItemIndex = GameObject::player.getWeaponInventory()->size();
	else
		GameObject::currentInventoryItemIndex++;

}

void GameObject::forceCloseGame()
{
	GameObject::isGameOver = true;
	GameObject::state = GameState::STATE_GAME_OVER;

	return;
}
/*
void GameObject::getInput()
{
	clock_t startTime = clock();
	GameObject::inputChar = 0;

	bool gettingInput = true;
	while (gettingInput) {
		GameObject::inputChar = _getch();
		if ((clock() - startTime) > GameObject::CMD_DELAY)
			gettingInput = false;
	}

	inputChar = putchar(tolower(inputChar));

	printf("\u001b[3A");

	GameObject::state = GameState::STATE_PARSE_INPUT;
	return;
}

void GameObject::parseInput()
{
	GameObject::state = GameState::STATE_VALIDATE_INPUT;
	return;
}

void GameObject::validateInput()
{
	switch (Display::getInstance().getCurrentMenu()) {

	case ContextMenu::START_MENU: {
		if (GameObject::inputChar == '1')
			GameObject::currentCmd = GameCommand::CMD_START_GAME;
		else if (GameObject::inputChar == '2')
			GameObject::currentCmd = GameCommand::CMD_EXIT_GAME;
		else {
			GameObject::currentCmd = GameCommand::CMD_BLANK;
			GameObject::state = GameState::STATE_UPDATE_DISPLAY;
			return;
		}
	} break;
	case ContextMenu::GAME_MENU: {
		if (GameObject::inputChar == '5')
			GameObject::currentCmd = GameCommand::CMD_EXIT_GAME;
		else if (GameObject::inputChar == '1')
			GameObject::currentCmd = GameCommand::CMD_EXPLORE;
		else if (GameObject::inputChar == '2')
			GameObject::currentCmd = GameCommand::CMD_REST;
		else if (GameObject::inputChar == '3')
			GameObject::currentCmd = GameCommand::CMD_ENTER_INV;
		else if (GameObject::inputChar == '4')
			GameObject::currentCmd = GameCommand::CMD_ENTER_STATS_MENU;
		else
		{
			GameObject::currentCmd = GameCommand::CMD_BLANK;
			GameObject::state = GameState::STATE_UPDATE_DISPLAY;
			return;
		}
	} break;
	case ContextMenu::INVENTORY_MENU: {
	
		if (GameObject::inputChar == 'w' && !GameObject::player.getWeaponInventory()->empty())
			GameObject::currentCmd = GameCommand::CMD_INV_UP;
		else if (GameObject::inputChar == 's' && !GameObject::player.getWeaponInventory()->empty())
			GameObject::currentCmd = GameCommand::CMD_INV_DOWN;
		else if (
			GameObject::inputChar == 'd'
			&& !GameObject::player.getWeaponInventory()->empty()
			&& (GameObject::player.getWeaponInventory()->at(currentInventoryItemIndex - 1) != GameObject::player.getWeapon()))
		{
			GameObject::currentCmd = GameCommand::CMD_INV_DISCARD;
		}
		else if (GameObject::inputChar == 'e' && !GameObject::player.getWeaponInventory()->empty())
			GameObject::currentCmd = GameCommand::CMD_INV_TOGGLE_EQUIP;
		else if (GameObject::inputChar == 'q')
			GameObject::currentCmd = GameCommand::CMD_EXIT_INV;
		else
		{
			GameObject::currentCmd = CMD_BLANK;
			GameObject::state = GameState::STATE_UPDATE_DISPLAY;
			return;
		}
	} break;
	case ContextMenu::BATTLE_MENU: {
		if (GameObject::inputChar == '1')
			GameObject::currentCmd = GameCommand::CMD_BATTLE_ATTACK;
		else if (GameObject::inputChar == '2')
			GameObject::currentCmd = GameCommand::CMD_BATTLE_RUN;
		else
		{
			GameObject::currentCmd = GameCommand::CMD_BLANK;
			GameObject::state = GameState::STATE_UPDATE_DISPLAY;
			return;
		}

	} break;
	case ContextMenu::STATS_MENU: {
		GameObject::currentCmd = GameCommand::CMD_EXIT_STATS_MENU;
	} break;
	default: 
		break;
	}
	GameObject::state = GameState::STATE_PROCESS_EVENTS;
	return;
}

void GameObject::processEvents()
{

	switch (GameObject::currentCmd)
	{
	// Game Menu
	case GameCommand::CMD_BLANK: {
		GameObject::state = GameState::STATE_UPDATE_DISPLAY;
	} break;
	case GameCommand::CMD_START_GAME: {
		Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
	} break;
	case GameCommand::CMD_EXPLORE: {
		GameObject::player.totalActions++;

		EncounterType et = GameObject::player.explore();
		if (et == ET_NOTHING)
			break;
		else if (et == ET_BATTLE) {
			Display::getInstance().setCurrentMenu(ContextMenu::BATTLE_MENU);

			GameObject::currentEnemy = new Enemy;
		}
	} break;
	case GameCommand::CMD_REST: {
		GameObject::player.totalActions++;
		GameObject::player.rest();
	} break;
	case GameCommand::CMD_EXIT_GAME: {
		GameObject::state = GameState::STATE_FULL_CLEANUP;
		return;
	} break;
	
	// Inventory
	case GameCommand::CMD_ENTER_INV: {
		Display::getInstance().setCurrentMenu(ContextMenu::INVENTORY_MENU);
	} break;

	case GameCommand::CMD_EXIT_INV: {
		Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
	} break;
	case GameCommand::CMD_INV_UP:{
		inventoryMoveSelectionUp();
	}break; 
	case GameCommand::CMD_INV_DOWN: {
		inventoryMoveSelectionDown();
	} break;
	case GameCommand::CMD_INV_DISCARD: {
		delete GameObject::player.getWeaponInventory()->at(currentInventoryItemIndex - 1);
		GameObject::player.getWeaponInventory()->erase(GameObject::player.getWeaponInventory()->begin() + GameObject::currentInventoryItemIndex - 1);
		inventoryMoveSelectionUp();
	} break;
	case GameCommand::CMD_INV_TOGGLE_EQUIP: {
		Weapon* tmpWeapon = GameObject::player.getWeapon();
		if (GameObject::player.getWeaponInventory()->at(GameObject::currentInventoryItemIndex - 1) == GameObject::player.getWeapon())
			GameObject::player.setWeapon(nullptr);
		else
			GameObject::player.setWeapon(GameObject::player.getWeaponInventory()->at(GameObject::currentInventoryItemIndex - 1));
	} break;

	// Stats
	case GameCommand::CMD_ENTER_STATS_MENU: {
		Display::getInstance().setCurrentMenu(ContextMenu::STATS_MENU);
	} break;
	case GameCommand::CMD_EXIT_STATS_MENU: {
		Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
		;
	} break;

	// Battle
	case GameCommand::CMD_BATTLE_ATTACK: {
		GameObject::player.totalActions++;

		GameObject::currentEnemy->updateHealth(-GameObject::player.getAttackDamage());
		GameObject::player.takeDamage(GameObject::currentEnemy->getDamage());
		if (!GameObject::currentEnemy->isAlive()) {
			Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
			delete currentEnemy;
		}
	} break;
	case GameCommand::CMD_BATTLE_RUN: {
		GameObject::player.totalActions++;
		delete currentEnemy;
		Display::getInstance().setCurrentMenu(ContextMenu::GAME_MENU);
	} break;

	default: {} break;
	}

	// hopefully unreachable code------ TODO: delete/replace at some point
	if (GameObject::player.isAlive() == false)
	{
		GameObject::state = GameState::STATE_FULL_CLEANUP;
		return;
	}
	GameObject::state = GameState::STATE_QUICK_CLEANUP;
	return;
}

void GameObject::quickCleanup()
{
	GameObject::state = GameState::STATE_UPDATE_DISPLAY;
	return;
}

void GameObject::updateDisplay()
{
	// Clear Screen
	printf("\u001b[50;1H");
	for (int i = 1; i < 50 ; i++)
	{
		printf("\u001b[J");
		printf("\u001b[F");
	}

	switch (Display::getInstance().getCurrentMenu())
	{
	case ContextMenu::START_MENU: {
		printf("\t\t\t\t\t\tPlease select an option:\n");
		printf("\t\t\t\t\t\t\t1. Start\n");
		printf("\t\t\t\t\t\t\t2. Exit\n");
	} break;
	case ContextMenu::GAME_MENU: {
		printf("\t\t\t\t\t\tPlease select an option:\n");
		printf("\t\t\t\t\t\t\t1. Explore\n");
		if(GameObject::player.getHealth() == GameObject::player.getMaxHealth())
			printf("\t\t\t\t\t\t\t2. Rest (full health)\n");
		else
			printf("\t\t\t\t\t\t\t2. Rest\n");
		if(GameObject::player.isBackpackFull())
			printf("\t\t\t\t\t\t\t3. View Inventory (full)\n");
		else
			printf("\t\t\t\t\t\t\t3. View Inventory\n");
		printf("\t\t\t\t\t\t\t4. View stats\n");
		printf("\t\t\t\t\t\t\t5. Exit game\n");
		printf("\u001b[6A");
		printf("Health: %3.0f\n",GameObject::player.getHealth());
		printf("Gold: %i\n", GameObject::player.getGoldAmount());
		printf("Level: %i\n", GameObject::player.getLevel());
		printf("Exp to Next Level: %lli\n", GameObject::player.experienceCurve()-GameObject::player.getExperience());
	} break;
	case ContextMenu::INVENTORY_MENU: {

		printf("\t\t\t\t\t\t\t Inventory: (%lli/%i):\n", GameObject::player.getWeaponInventory()->size(), GameObject::player.getBackpackMaxSize());
		if (GameObject::player.getWeaponInventory()->size() != 0)
			printf("\n-------------------------------------\n");

		Weapon* selectedWeapon = nullptr;

		for (int i = 0; i < GameObject::player.getWeaponInventory()->size(); i++)
		{
			Weapon* currentWeapon = GameObject::player.getWeaponInventory()->at(i);

			if (i == GameObject::currentInventoryItemIndex - 1)
			{
				std::string printMsg = "| %s<-\u001b[36G";
				if (currentWeapon == GameObject::player.getWeapon())
					printMsg += "~|\n";
				else
					printMsg += " |\n";

				printf(printMsg.c_str(), currentWeapon->getName().c_str());
				selectedWeapon = currentWeapon;
			}
			else
			{
				std::string printMsg = "| %s\u001b[36G";

				if (currentWeapon == GameObject::player.getWeapon())
					printMsg += "~|\n";
				else
					printMsg += " |\n";

				printf(printMsg.c_str(), currentWeapon->getName().c_str());
			}
		}
		Weapon* equippedWeapon = GameObject::player.getWeapon();

		if (GameObject::player.getWeaponInventory()->size() != 0)
			printf("-------------------------------------\n");

		printf("\u001b[3;64H-------------------------------------------------");
		printf("\u001b[4;64H| Current Weapon: %s\u001b[4;112H|", equippedWeapon->getName().c_str());
		printf("\u001b[5;64H| - %s / Level %i\u001b[5;112H|", equippedWeapon->getRarityText().c_str(),equippedWeapon->getLevel());
		printf("\u001b[6;64H| - Dmg: %.0f\u001b[6;112H|", equippedWeapon->getDamage());
		printf("\u001b[7;64H| Current Armor: %s\u001b[7;112H|\n", "None");
		printf("\u001b[8;64H-------------------------------------------------");

		if (selectedWeapon != nullptr)
		{
			printf("\u001b[10;64H-------------------------------------------------\n");
			printf("\u001b[11;64H| Selected Weapon: %s\u001b[11;112H|", selectedWeapon->getName().c_str());
			printf("\u001b[12;64H| - %s / Level %i\u001b[12;112H|", selectedWeapon->getRarityText().c_str(), selectedWeapon->getLevel());
			printf("\u001b[13;64H| - Dmg: %.0f\u001b[13;112H|", selectedWeapon->getDamage());
			printf("\u001b[14;64H-------------------------------------------------");

			printf("\n\t\t\t\t\t\t\t\t Options: \n");

			printf("\t\t\t\t\t\t\t\tW: up\n");
			printf("\t\t\t\t\t\t\t\tS: down\n");
			
			if (selectedWeapon != GameObject::player.getWeapon())
				printf("\t\t\t\t\t\t\t\tD: discard\n");

			if (selectedWeapon == GameObject::player.getWeapon())
				printf("\t\t\t\t\t\t\t\tE: unequip\n");
			else		
				printf("\t\t\t\t\t\t\t\tE: equip\n");
			printf("\t\t\t\t\t\t\t\tQ: close\n");
		}
		else
		{
			printf("\u001b[10;64HQ: close");
		}
	} break;
	case ContextMenu::BATTLE_MENU: {
		printf("\t\t\t\t\t\tPlease select a battle option:\n");
		printf("\t\t\t\t\t\t\t1. Attack\n");
		printf("\t\t\t\t\t\t\t2. Run\n");
		printf("\u001b[3A");
		printf("Health: %3.0f\n", GameObject::player.getHealth());
		printf("Gold: %i\n", GameObject::player.getGoldAmount());
		printf("Level: %i\n", GameObject::player.getLevel());
		printf("Exp to Next Level: %lli\n", GameObject::player.experienceCurve() - GameObject::player.getExperience());
		printf("Current Damage: %.0f\n", GameObject::player.getDamage());
		printf("\n");
		printf((GameObject::currentEnemy)->getName().c_str());
		printf("\nHealth: %3.0f\n", GameObject::currentEnemy->getHealth());
	} break;

	case ContextMenu::STATS_MENU: {
		printf("\t\t\t\t\t\t\tStatistics:\n");
		printf("\t\t\t\t\t\tTimes Explored: \t%lli\n", GameObject::player.timesExplored);
		printf("\t\t\t\t\t\tTimes Rested: \t\t%lli\n", GameObject::player.timesRested);
		printf("\t\t\t\t\t\tLevels Gained: \t\t%i\n", GameObject::player.getLevel());
		printf("\t\t\t\t\t\tTotal Experience: \t%lli\n", GameObject::player.lifetimeExperience);
		printf("\t\t\t\t\t\tGold Collected: \t%lli\n", GameObject::player.goldCollected);
		//printf("\t\t\t\t\t\tDamage Dealt: \t\t%lli\n", GameObject::player.lifetimeDamage);
		//printf("\t\t\t\t\t\tDamage Taken: \t\t%d\n", GameObject::player.damageTaken);
		//printf("\t\t\t\t\t\tHealth Healed: \t\t%d\n", GameObject::player.healthHealed);
		printf("\t\t\t\t\t\tEnemies Slain: \t\t%lli\n", GameObject::player.enemiesSlain);
		printf("\t\t\t\t\t\tTotal actions: \t\t%lli\n", GameObject::player.totalActions);
		printf("\t\t\t\t\t\tItems Collected: \t%lli\n", GameObject::player.itemsCollected);

		printf("\n\t\t\t\t\t\t(Type any command to exit)\n");
	} break;
	default: {} break;
	}
	printf("\u001b[99999;99999H");
	GameObject::state = GameState::STATE_GET_INPUT;
	return;
}

void GameObject::fullCleanup()
{
	GameObject::state = GameState::STATE_GAME_OVER;
	return;
}
*/