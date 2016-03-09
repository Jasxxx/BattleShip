#pragma once
#include <Windows.h>

class Board;

class Game
{
	bool playing;

	enum Color
	{
		Black = 0,
		Gray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
	};

	enum State { MENU, SETUP, PLAY };

	State m_eCurrentState;
	bool isAI;
	bool showAll;

	Board* p1;
	Board* p2;

	int m_nP1Ships;
	int m_nP2Ships;

	int m_nInputX;
	int m_nInputY;

	bool selected;

	int m_nCurrentMenu;
	int m_nCurrentSelection;

	int m_nCoordX;
	int m_nCoordY;

	bool p1Turn;
	int m_nCurrShip;

	int m_nWinner;
	bool gameOver;

public:
	Game();
	~Game();

	void(Game::*Update)();
	void(Game::*Display)();

	bool Run();
	void Input();

	void MenuUpdate();
	void SetupUpdate();
	void PlayUpdate();

	void MenuDisplay();
	void SetupDisplay();
	void PlayDisplay();

	void ResetGame(); 

	// helper functions for common console manipulation
	void ClearScreen();
	void ForegroundColor(WORD attr);
	void BackgroundColor(WORD attr);
	void ResetColor() { ForegroundColor(Gray); BackgroundColor(Black); }
	void SetCursorPosition(int x, int y);
};
