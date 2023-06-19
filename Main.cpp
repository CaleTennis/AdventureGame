#include <stdio.h>
#include "GameObject.hpp"

GameObject game = GameObject();

int main(int argc, char* argv[])
{
	while (!game.isGameOver) {
		switch (game.state)
		{
		case STATE_SETUP:
			game.setup();
			break;
		case STATE_GET_INPUT:
			game.getInput();
			break;
		case STATE_PARSE_INPUT:
			game.parseInput();
			break;
		case STATE_VALIDATE_INPUT:
			game.validateInput();
			break;
		case STATE_PROCESS_EVENTS:
			game.processEvents();
			break;
		case STATE_QUICK_CLEANUP:
			game.quickCleanup();
			break;
		case STATE_UPDATE_DISPLAY:
			game.updateDisplay();
			break;
		case STATE_FULL_CLEANUP:
			game.fullCleanup();
			break;
		case STATE_GAME_OVER:

			game.gameOver();
			
			printf("\u001b[50;1H");
			for (int i = 1; i < 50; i++)
			{
				printf("\u001b[J");
				printf("\u001b[F");
			}
			printf("\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\t\t\t\t\t\tGame exited sucessfully.");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
			break;
		default:
			break;
		}
	}
	return 0;
}