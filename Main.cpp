#include <stdio.h>
#include <Windows.h>
#include "Display.hpp"
#include "GameObject.hpp"
#pragma warning(disable : 28251)

GameObject game = GameObject();

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nShowCmd)
{
	Display::getInstance().setHInstance(hInstance);
	game.setup();

	if (!(Display::getInstance().setupWindow(game)))
		return 1;
	if (!(Display::getInstance().createWindow()))
		return 1;

	// Delta time code written by Daniel Blagy: https://www.youtube.com/watch?v=ZdtMJPJx9jk

	LARGE_INTEGER cpu_frequency;
	LARGE_INTEGER last_counter;
	LARGE_INTEGER current_counter;

	QueryPerformanceFrequency(&cpu_frequency);
	QueryPerformanceCounter(&last_counter);
	while (!game.isGameOver)
	{
		QueryPerformanceCounter(&current_counter);

		int64_t counter_elapsed = current_counter.QuadPart - last_counter.QuadPart;

		float delta = (float)counter_elapsed / (float)cpu_frequency.QuadPart; // in seconds
		last_counter = current_counter;
		
		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{	
			TranslateMessage(&message);

			// send message to callback function
			DispatchMessage(&message); 
		}
	}

	return 0;
}