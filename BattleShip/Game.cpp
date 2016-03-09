#pragma once
#include "Game.h"
#include "Board.h"
#include <string>
#include <iostream>

using namespace std;

Game::Game()
{
	m_eCurrentState = MENU;

	Update = &Game::MenuUpdate;
	Display = &Game::MenuDisplay;

	p1 = new Board();
	p1->SetPosition(10, 5);
	p1->m_strName = "Player 1";

	p2 = new Board();
	p2->SetPosition(10, 31);
	p2->m_strName = "Player 2";

	m_nInputX = 0;
	m_nInputY = 0;

	selected = false;

	m_nCurrentMenu = 0;
	m_nCurrentSelection = 0;
	m_nCoordX = 0;
	m_nCoordY = 0;

	p1Turn = true;

	m_nCurrShip = 0;

	m_nP1Ships = 5;
	m_nP2Ships = 5;

	playing = true;

	m_nWinner = 0;
	gameOver = false;
}


Game::~Game()
{
	delete p1;
	delete p2;
}

bool Game::Run()
{
	m_nInputX = 0;
	m_nInputY = 0;

	Input();

	(this->*Update)();
	LockWindowUpdate(GetConsoleWindow());
	ClearScreen();
	(this->*Display)();
	LockWindowUpdate(nullptr);
	Sleep(25);

	return playing;
}

void Game::Input()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		m_nInputX = -1;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		m_nInputX = 1;
	}

	if (GetAsyncKeyState(VK_UP))
	{
		m_nInputY = -1;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		m_nInputY = 1;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		selected = true;
	}

	if (GetAsyncKeyState(VK_F1))
	{
		showAll = !showAll;
	}
}

void Game::MenuUpdate()
{
	if (m_nInputX != 0 || m_nInputY != 0)
	{
		m_nCurrentSelection += (m_nInputX + m_nInputY);

		if (m_nCurrentSelection < 0)
		{
			m_nCurrentSelection = 1;
		}
		if (m_nCurrentSelection > 1)
		{
			m_nCurrentSelection = 0;
		}
	}

	if (selected)
	{
		if (m_nCurrentSelection == 0) // player vs player
		{
			Update = &Game::SetupUpdate;
			Display = &Game::SetupDisplay;
		}
		else // exit
		{
			playing = false;
		}

		selected = false;
	}
}

void Game::SetupUpdate()
{
	Board* currPlayer = nullptr;

	if (p1Turn)
	{
		currPlayer = p1;
	}
	else
	{
		currPlayer = p2;
	}

	if (GetAsyncKeyState(VK_RETURN))
	{
		currPlayer->m_arrShips[m_nCurrShip].horizontal = !currPlayer->m_arrShips[m_nCurrShip].horizontal;
	}

	if (m_nInputX != 0 || m_nInputY != 0)
	{
		m_nCoordX += m_nInputX;
		m_nCoordY += m_nInputY;

		if (currPlayer->m_arrShips[m_nCurrShip].horizontal)
		{
			if (m_nCoordX < 0)
			{
				m_nCoordX = 10 - currPlayer->m_arrShips[m_nCurrShip].length;
			}
			else if (m_nCoordX + currPlayer->m_arrShips[m_nCurrShip].length > 10)
			{
				m_nCoordX = 0;
			}

			if (m_nCoordY < 0)
			{
				m_nCoordY = 9;
			}
			else if (m_nCoordY > 9)
			{
				m_nCoordY = 0;
			}
		}
		else
		{
			if (m_nCoordY < 0)
			{
				m_nCoordY = 10 - currPlayer->m_arrShips[m_nCurrShip].length;
			}
			else if (m_nCoordY + currPlayer->m_arrShips[m_nCurrShip].length > 10)
			{
				m_nCoordY = 0;
			}

			if (m_nCoordX < 0)
			{
				m_nCoordX = 9;
			}
			else if (m_nCoordX > 9)
			{
				m_nCoordX = 0;
			}
		}

	}

	currPlayer->m_arrShips[m_nCurrShip].position = COORD{ m_nCoordX, m_nCoordY };

	currPlayer->ResetBoard();
	currPlayer->ActivateTempShip(m_nCurrShip);
	currPlayer->ActiveShipCheck();

	if (selected)
	{
		selected = false;
		m_nCoordX = 0;
		m_nCoordY = 0;

		if (currPlayer->CheckShipPlacement(m_nCurrShip))
		{
			currPlayer->SetShip(m_nCurrShip);

			if (p1Turn && m_nCurrShip == 4)
			{
				p1Turn = false;
				m_nCurrShip = 0;
			}
			else if (!p1Turn && m_nCurrShip == 4)
			{
				Update = &Game::PlayUpdate;
				Display = &Game::PlayDisplay;
				m_nCoordX = 5;
				m_nCoordY = 5;
				p1Turn = true;
			}
			else
			{
				m_nCurrShip++;
			}
		}
	}
}

void Game::PlayUpdate()
{
	Board* currPlayer = nullptr;
	Board* nextPlayer = nullptr;

	if (p1Turn)
	{
		currPlayer = p1;
		nextPlayer = p2;
	}
	else
	{
		currPlayer = p2;
		nextPlayer = p1;
	}

	if (m_nInputX != 0 || m_nInputY != 0)
	{
		m_nCoordX += m_nInputX;
		m_nCoordY += m_nInputY;

		if (m_nCoordX < 0)
		{
			m_nCoordX = 9;
		}
		else if (m_nCoordX > 9)
		{
			m_nCoordX = 0;
		}

		if (m_nCoordY < 0)
		{
			m_nCoordY = 9;
		}
		else if (m_nCoordY > 9)
		{
			m_nCoordY = 0;
		}
	}

	currPlayer->ActivateTempShot(m_nCoordX, m_nCoordY);

	if (selected)
	{
		selected = false;
		int result = nextPlayer->CheckShot(m_nCoordX, m_nCoordY);
		if (result == 2)
		{
			return;
		}
		else
		{
			currPlayer->MarkShot(m_nCoordX, m_nCoordY, result);
		}
		p1Turn = !p1Turn;
	}

	m_nP1Ships = p1->CheckShipsRemaining();
	m_nP2Ships = p2->CheckShipsRemaining();
}

void Game::MenuDisplay()
{
	SetCursorPosition(52, 0);
	cout << "BATTLESHIP";

	if (m_nCurrentSelection == 0)
	{
		ForegroundColor(Black);
		BackgroundColor(Gray);
	}

	SetCursorPosition(49, 3);
	cout << "Player Vs Player";

	ResetColor();

	if (m_nCurrentSelection == 1)
	{
		ForegroundColor(Black);
		BackgroundColor(Gray);
	}

	SetCursorPosition(55, 5);
	cout << "Exit";

	ResetColor();
}

void Game::SetupDisplay()
{
	p1->DrawBoard();
	p2->DrawBoard();
}

void Game::PlayDisplay()
{
	SetCursorPosition(50, 3);
	p1->DrawBoard();

	SetCursorPosition(50, 30);
	p2->DrawBoard();

	if (m_nP1Ships == 0)
	{
		ClearScreen();
		SetCursorPosition(27, 25);
		cout << "Player 2 Wins Press ENTER to continue";
		if (GetAsyncKeyState(VK_RETURN))
		{
			ResetGame();
		}
	}
	else if (m_nP2Ships == 0)
	{
		ClearScreen();
		SetCursorPosition(27, 25);
		cout << "Player 1 Wins Press ENTER to continue";
		if (GetAsyncKeyState(VK_RETURN))
		{
			ResetGame();
		}
	}
}

void Game::ClearScreen()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);

	DWORD width = info.dwSize.X, num;
	COORD coord = { 0, 0, };
	for (int i = 0; i < info.dwSize.Y; ++i)
	{
		coord.Y = i;
		FillConsoleOutputAttribute(handle, info.wAttributes, width, coord, &num);
		FillConsoleOutputCharacter(handle, ' ', width, coord, &num);
	}

	SetConsoleCursorPosition(handle, coord);
}

void Game::ForegroundColor(WORD attr)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	SetConsoleTextAttribute(handle, info.wAttributes & 0xF0 | attr);
}

void Game::BackgroundColor(WORD attr)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);
	SetConsoleTextAttribute(handle, info.wAttributes & 0x0F | attr << 4);
}

void Game::SetCursorPosition(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(handle, &info);

	int left = x;
	int top = y;

	left = max(left, 0);
	left = min(left, info.dwSize.X - 1);
	top = max(top, 0);
	top = min(top, info.dwSize.Y - 1);

	COORD coord = { left, top };
	SetConsoleCursorPosition(handle, coord);
}

void Game::ResetGame()
{
	p1->ResetBoard();
	p2->ResetBoard();

	p1->ResetShips();
	p2->ResetShips();

	Update = &Game::MenuUpdate;
	Display = &Game::MenuDisplay;
	p1Turn = true;

	m_nP1Ships = 5;
	m_nP1Ships = 5;
	m_nCurrShip = 0;
}