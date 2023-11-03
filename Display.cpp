#include "Display.hpp"
#pragma warning(disable : 4244)

std::string tmpStr = "";
std::wstring bufStr = L"";
LPCWSTR printStr = L"";
bool Display::hasSelectedInventoryItem = false;

bool Display::setupWindow(GameObject& game) 
{

	// Main Window
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = GameObject::WndProc;
	wc.hInstance = hInstance_;
	wc.lpszClassName = WND_CLASS_NAME_;

	if (!RegisterClass(&wc))
	{
		OutputDebugString(L"Could not register window class\n");
		return false;
	}
	else
	{
		return true;
	}
}

bool Display::createWindow()
{

	hWindow = CreateWindowEx(
		0,									// Optional window styles
		WND_CLASS_NAME_,						// Window class
		WINDOW_TITLE_.c_str(),				// Window text
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// Window style
		CW_USEDEFAULT,						// Size and position
		CW_USEDEFAULT,						//	^
		WINDOW_WIDTH_,						//	^
		WINDOW_HEIGHT_,						//	^
		0,				// Parent Window
		0,				// Menu
		hInstance_,		// Instance Handle
		0				// Additional application data
	);
	updateMenuScreen();
	if (!hWindow)
	{
		OutputDebugString(L"Could not create window\n");
		return false;
	} 
	else
	{
		return true;
	}
}

bool Display::paint()
{
	switch (currentMenu_)
	{
	case ContextMenu::START_MENU: {

		PAINTSTRUCT ps;
		HDC deviceContext = BeginPaint(hWindow, &ps); // All painting occurs between BeginPaint and EndPaint
		FillRect(deviceContext, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 3));
		EndPaint(hWindow, &ps);
	} break;
	case ContextMenu::GAME_MENU: {
		tmpStr = "";
		tmpStr.append("Health: " + Display::to_string_with_precision(GameObject::getPlayer()->getHealth(),0) + "\n");
		tmpStr.append("Gold:   " + Display::to_string_with_precision(GameObject::getPlayer()->getGoldAmount(), 0) + "\n");
		tmpStr.append("Level:  " + Display::to_string_with_precision(GameObject::getPlayer()->getLevel(), 0) + "\n");
		
		std::wstring bufStr = std::wstring(tmpStr.begin(), tmpStr.end());

		LPCWSTR printStr = bufStr.c_str();

		PAINTSTRUCT ps;
		RECT rect = RECT();
		GetClientRect(hWindow, &rect);
		rect.left = 200;
		rect.top = 600;
		HDC deviceContext = BeginPaint(hWindow, &ps); // All painting occurs between BeginPaint and EndPaint
		FillRect(deviceContext, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 3));
		SetTextColor(deviceContext, RGB(255,255,255));
		SetBkMode(deviceContext, TRANSPARENT);
		DrawText(deviceContext, printStr, -1, &rect, DT_NOCLIP);
		EndPaint(hWindow, &ps);
	} break;
	case ContextMenu::INVENTORY_MENU: {
		PAINTSTRUCT ps;
		RECT rect = RECT();
		GetClientRect(hWindow, &rect);

		HDC deviceContext = BeginPaint(hWindow, &ps); // All painting occurs between BeginPaint and EndPaint
		FillRect(deviceContext, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 3));
		
		if (GameObject::getPlayer()->getWeaponInventory()->size() > 0 && Display::hasSelectedInventoryItem)
		{
			SetTextColor(deviceContext, RGB(255, 255, 255));
			SetBkMode(deviceContext, TRANSPARENT);

			rect.left = 200;
			rect.top = 300;

			tmpStr = "";
			Weapon* selectedWeapon = GameObject::getPlayer()->getWeaponInventory()->at(GameObject::currentInventoryItemIndex);
			tmpStr.append("Level " + Display::to_string_with_precision(GameObject::getPlayer()->getLevel(), 0));
			tmpStr.append(" ");
			tmpStr.append(selectedWeapon->getName().c_str());
			tmpStr.append(" - ");
			
			tmpStr.append(selectedWeapon->getRarityText().c_str());
			tmpStr.append("\nDamage: ");
			tmpStr.append((to_string_with_precision(selectedWeapon->getDamage(),0)));

			bufStr = std::wstring(tmpStr.begin(), tmpStr.end());

			printStr = bufStr.c_str();
			DrawText(deviceContext, printStr, -1, &rect, DT_NOCLIP);

		}


		EndPaint(hWindow, &ps);
	} break;
	case ContextMenu::BATTLE_MENU: {

		PAINTSTRUCT ps;
		RECT rect = RECT();
		GetClientRect(hWindow, &rect);

		HDC deviceContext = BeginPaint(hWindow, &ps); // All painting occurs between BeginPaint and EndPaint
		FillRect(deviceContext, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 3));
		SetTextColor(deviceContext, RGB(255, 255, 255));
		SetBkMode(deviceContext, TRANSPARENT);

		//Draw player info
		rect.left = 200;
		rect.top = 600;

		tmpStr = "";
		tmpStr.append("Health: " + Display::to_string_with_precision(GameObject::getPlayer()->getHealth(), 0) + "\n");
		tmpStr.append("Gold:   " + Display::to_string_with_precision(GameObject::getPlayer()->getGoldAmount(), 0) + "\n");
		tmpStr.append("Level:  " + Display::to_string_with_precision(GameObject::getPlayer()->getLevel(), 0) + "\n");

		bufStr = std::wstring(tmpStr.begin(), tmpStr.end());
		printStr = bufStr.c_str();
		DrawText(deviceContext, printStr, -1, &rect, DT_NOCLIP);

		// Draw enemy info
		rect.left = 400;
		rect.top = 600;

		tmpStr = "";
		tmpStr.append(GameObject::currentEnemy->getName());
		tmpStr.append("\nHealth: " + Display::to_string_with_precision(GameObject::currentEnemy->getHealth(), 0) + "\n");

		SetTextColor(deviceContext, RGB(255, 0, 0));

		bufStr = std::wstring(tmpStr.begin(), tmpStr.end());
		printStr = bufStr.c_str();
		DrawText(deviceContext, printStr, -1, &rect, DT_NOCLIP);

		EndPaint(hWindow, &ps);
	} break;
	default: {} break;
	}

	return true;
}

bool Display::updateMenuScreen()
{
	switch (currentMenu_)
	{
	case START_MENU: {
		InvalidateRect(hWindow, NULL, NULL);
		DestroyWindow(hGameExploreButton);
		DestroyWindow(hGameRestButton);
		DestroyWindow(hGameOpenInventoryButton);
		DestroyWindow(hGameQuitButton);

		hTitleText = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			TEXT("STATIC"),
			TEXT("GAME"),
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			640 - 150,
			50,
			300,
			125,
			hWindow,
			NULL,
			GetModuleHandle(NULL),
			NULL);

		SendMessage(hTitleText, WM_SETFONT, WPARAM(hTitleFont), TRUE);

		hTitleStartButton = CreateWindow(
			L"BUTTON",
			L"START",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640 - (START_BUTTON_WIDTH / 2),
			240 - (START_BUTTON_HEIGHT / 2),
			START_BUTTON_WIDTH,
			START_BUTTON_HEIGHT,
			hWindow,
			(HMENU)title_start_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL
		);

		hTitleQuitButton = CreateWindow(
			L"BUTTON",
			L"QUIT",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640 - (START_BUTTON_WIDTH / 2),
			240 - (START_BUTTON_HEIGHT / 2) + (1*(1+START_BUTTON_SPACING) * START_BUTTON_HEIGHT),
			START_BUTTON_WIDTH,
			START_BUTTON_HEIGHT,
			hWindow,
			(HMENU)title_quit_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL
		);

	} break;
	case GAME_MENU: {
		InvalidateRect(hWindow, NULL, NULL);
		DestroyWindow(hTitleText);
		DestroyWindow(hTitleStartButton);
		DestroyWindow(hTitleQuitButton);
		DestroyWindow(hInventoryCloseButton);
		DestroyWindow(hInventoryListbox);
		DestroyWindow(hBattleAttackButton);
		DestroyWindow(hBattleRunButton);

		hGameExploreButton = CreateWindow(
			L"BUTTON",
			L"Explore",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640 - (GAME_BUTTON_WIDTH / 2),
			240 - (GAME_BUTTON_HEIGHT / 2),
			GAME_BUTTON_WIDTH,
			GAME_BUTTON_HEIGHT,
			hWindow,
			(HMENU)game_explore_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL);
		hGameRestButton = CreateWindow(
			L"BUTTON",
			L"Rest",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640 - (GAME_BUTTON_WIDTH / 2),
			240 - (GAME_BUTTON_HEIGHT / 2) + (GAME_REST_BUTTON_INDEX*(1+GAME_BUTTON_SPACING) * GAME_BUTTON_HEIGHT),
			GAME_BUTTON_WIDTH,
			GAME_BUTTON_HEIGHT,
			hWindow,
			(HMENU)game_rest_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL);
		hGameOpenInventoryButton = CreateWindow(
			L"BUTTON",
			L"Open Inventory",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640 - (GAME_BUTTON_WIDTH / 2),
			240 - (GAME_BUTTON_HEIGHT / 2) + (GAME_OPEN_INVENTORY_BUTTON_INDEX * (1 + GAME_BUTTON_SPACING) * GAME_BUTTON_HEIGHT),
			GAME_BUTTON_WIDTH,
			GAME_BUTTON_HEIGHT,
			hWindow,
			(HMENU)game_open_inventory_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL);

		hGameQuitButton = CreateWindow(
			L"BUTTON",
			L"Exit",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640 - (GAME_BUTTON_WIDTH / 2),
			240 - (GAME_BUTTON_HEIGHT / 2) + (GAME_QUIT_BUTTON_INDEX*(1+GAME_BUTTON_SPACING) * GAME_BUTTON_HEIGHT),
			GAME_BUTTON_WIDTH,
			GAME_BUTTON_HEIGHT,
			hWindow,
			(HMENU)game_exit_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL);

	} break;
	case INVENTORY_MENU: {
		InvalidateRect(hWindow, NULL, NULL);
		DestroyWindow(hGameExploreButton);
		DestroyWindow(hGameRestButton);
		DestroyWindow(hGameOpenInventoryButton);
		DestroyWindow(hGameQuitButton);
		DestroyWindow(hBattleAttackButton);
		DestroyWindow(hBattleRunButton);

		hInventoryCloseButton = CreateWindow(
			L"BUTTON",
			L"Back",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640,
			240,
			GAME_BUTTON_WIDTH,
			GAME_BUTTON_HEIGHT,
			hWindow,
			(HMENU)inventory_close_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL);

		hInventoryListbox = CreateWindow(
			L"LISTBOX", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_NOTIFY,
			50, 50, 300, 200,
			hWindow, (HMENU)inventory_listbox_label, GetModuleHandle(NULL), NULL);

		// Populate listbox
		for (int i = 0; i < GameObject::getPlayer()->getWeaponInventory()->size(); i++)
		{
			Weapon* currentWeapon = GameObject::getPlayer()->getWeaponInventory()->at(i);
			int pos = (int)SendMessage(hInventoryListbox, LB_ADDSTRING, 0, (LPARAM)currentWeapon->getWName().c_str());
			SendMessage(hInventoryListbox, LB_SETITEMDATA, pos, (LPARAM) i);
		}
		
		SetFocus(hInventoryListbox);
	} break;
	case BATTLE_MENU: {
		InvalidateRect(hWindow, NULL, NULL);
		DestroyWindow(hGameExploreButton);
		DestroyWindow(hGameRestButton);
		DestroyWindow(hGameOpenInventoryButton);
		DestroyWindow(hGameQuitButton);

		hBattleAttackButton = CreateWindow(
			L"BUTTON",
			L"Attack",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,

			640 - (BATTLE_BUTTON_WIDTH / 2),
			240 - (BATTLE_BUTTON_HEIGHT / 2) + (BATTLE_ATTACK_BUTTON_INDEX * (1 + BATTLE_BUTTON_SPACING) * BATTLE_BUTTON_HEIGHT),
			BATTLE_BUTTON_WIDTH,
			BATTLE_BUTTON_HEIGHT,
			hWindow,
			(HMENU)battle_attack_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL);

		hBattleRunButton = CreateWindow(
			L"BUTTON",
			L"Run",
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			640 - (BATTLE_BUTTON_WIDTH / 2),
			240 - (BATTLE_BUTTON_HEIGHT / 2) + (BATTLE_RUN_BUTTON_INDEX * (1 + BATTLE_BUTTON_SPACING) * BATTLE_BUTTON_HEIGHT),
			BATTLE_BUTTON_WIDTH,
			BATTLE_BUTTON_HEIGHT,
			hWindow,
			(HMENU)battle_run_label,
			(HINSTANCE)GetWindowLongPtr(hWindow, GWLP_HINSTANCE),
			NULL);
	} break;
	} 

	return true;
}

template <typename T>
std::string Display::to_string_with_precision(const T a_value, const int n)
{
	std::ostringstream out;
	out.precision(n);
	out << std::fixed << a_value;
	return out.str();
}

void Display::updateCurrentInventorySelection()
{
	int lbItem = (int)SendMessage(hInventoryListbox, LB_GETCURSEL, 0, 0);
	int i = (int)SendMessage(hInventoryListbox, LB_GETITEMDATA, lbItem, 0);
	GameObject::currentInventoryItemIndex = i;
}