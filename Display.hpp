#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <Windows.h>
#include <string>
#include <sstream>
#include "GameObject.hpp"
#include "ContextMenu.hpp"

enum { title_start_label = 1000, title_quit_label, game_explore_label, game_rest_label, game_exit_label, game_open_inventory_label,inventory_listbox_label, inventory_close_label, battle_attack_label, battle_run_label };

class Entity;
class Player;
class GameObject;
class Enemy;

class Display
{
private:
	const wchar_t* WND_CLASS_NAME_ = L"gameWindowClass";
	const std::wstring WINDOW_TITLE_ = L"TextbasedAdventureGame";
	const int WINDOW_WIDTH_ = 1280;
	const int WINDOW_HEIGHT_ = 720;

	const int START_BUTTON_WIDTH = 150;
	const int START_BUTTON_HEIGHT = 50;
	const float START_BUTTON_SPACING = 0.5f;

	const int GAME_BUTTON_WIDTH = 125;
	const int GAME_BUTTON_HEIGHT = 35;
	const float GAME_BUTTON_SPACING = 0.1f;

	const int GAME_EXPLORE_BUTTON_INDEX = 0;
	const int GAME_REST_BUTTON_INDEX = 1;
	const int GAME_OPEN_INVENTORY_BUTTON_INDEX = 2;
	const int GAME_QUIT_BUTTON_INDEX = 3;

	const int BATTLE_BUTTON_WIDTH = 125;
	const int BATTLE_BUTTON_HEIGHT = 35;
	const float BATTLE_BUTTON_SPACING = 0.1f;

	const int BATTLE_ATTACK_BUTTON_INDEX = 0;
	const int BATTLE_RUN_BUTTON_INDEX = 1;

	ContextMenu currentMenu_ = START_MENU;

	HINSTANCE hInstance_ = nullptr;
	WNDCLASS wc = {};

	HWND hWindow = nullptr;			// Main window handle

	HWND hTitleText = nullptr;		// Start screen handles
	HWND hTitleStartButton = nullptr;
	HWND hTitleQuitButton = nullptr;

	HWND hGameExploreButton = nullptr; // Game screen handles
	HWND hGameRestButton = nullptr;
	HWND hGameOpenInventoryButton = nullptr;
	HWND hGameQuitButton = nullptr;

	HWND hInventoryCloseButton = nullptr; // Inventory screen handles
	HWND hInventoryListbox = nullptr;

	HWND hBattleAttackButton = nullptr; // Battle screen handles
	HWND hBattleRunButton = nullptr;

	HFONT hTitleFont = CreateFont(120,0,0,0,FW_DONTCARE,TRUE,TRUE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,L"Comic Sans");
		
protected:
	Display() = default;
	template<typename T>
	std::string to_string_with_precision(const T a_value, const int n = 6);

public:
	Display(const Display&) = delete;
	Display& operator=(const Display&) = delete;
	~Display() {}

	static bool hasSelectedInventoryItem;

	static Display& getInstance()
	{
		static Display display;
		return display;
	}

	bool setupWindow(GameObject& game);
	bool createWindow();

	bool paint();
	bool updateMenuScreen();
	void repaint() { InvalidateRect(hWindow, NULL, NULL); }

	void setHInstance(HINSTANCE hInstance) { hInstance_ = hInstance; }

	ContextMenu getCurrentMenu() { return currentMenu_; }
	void setCurrentMenu(ContextMenu menu) { currentMenu_ = menu; };

	void updateCurrentInventorySelection();
};

#endif