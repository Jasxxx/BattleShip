#include "Game.h"

int main()
{
	bool playing = true;
	Game game;

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = GetLargestConsoleWindowSize(handle);
	SMALL_RECT rect = { 0, 0, 119, 49 };
	SetConsoleWindowInfo(handle, TRUE, &rect);

	while (playing)
	{
		playing = game.Run();
	
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			playing = false;
		}
	}

	return 0;
}

