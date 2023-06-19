#ifndef __GAMECOMMAND_HPP__
#define __GAMECOMMAND_HPP__

enum GameCommand
{
	// Generic
	CMD_BLANK = 0,
	CMD_EXIT_GAME,
	// Start Menu
	CMD_START_GAME,

	// Normal Game Menu
	CMD_EXPLORE,
	CMD_REST,
	CMD_ENTER_STATS_MENU,

	// Battle Menu
	CMD_BATTLE_ATTACK,
	CMD_BATTLE_RUN,

	// Inventory Menu
	CMD_ENTER_INV,
	CMD_EXIT_INV,
	CMD_INV_UP,
	CMD_INV_DOWN,
	CMD_INV_DISCARD,
	CMD_INV_TOGGLE_EQUIP,

	// Stats Menu
	CMD_EXIT_STATS_MENU,

	// Battle
	CMD_ATTACK,
	CMD_RUN
};

#endif