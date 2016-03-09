#pragma once
#include "Board.h"
#include <iostream>

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10

using namespace std;
Board::Board()
{
	Position.X = 0;
	Position.Y = 0;

	ResetBoard();

	for (int i = 0; i < 5; i++)
	{
		m_arrShips[i].horizontal = false;
		m_arrShips[i].isAlive = false;
		m_arrShips[i].isSet = false;
		m_arrShips[i].position = COORD{ 0, 0 };
	}

	m_arrShips[0].length = 5;
	m_arrShips[1].length = 4;
	m_arrShips[2].length = 3;
	m_arrShips[3].length = 3;
	m_arrShips[4].length = 2;

	m_arrShips[0].indexPositions.resize(5);
	m_arrShips[1].indexPositions.resize(4);
	m_arrShips[2].indexPositions.resize(3);
	m_arrShips[3].indexPositions.resize(3);
	m_arrShips[4].indexPositions.resize(2);

	pastPosition = COORD{ 5, 5 };
	m_cOldValue = ' ';

	m_nShipsRemaining = 5;
}

Board::~Board()
{

}

// clears board for new game
void Board::ResetBoard()
{
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			m_arrBoard[x][y] = ' ';
			m_arrAttBoard[x][y] = ' ';
		}
	}
}


// Prints current state of board
void Board::DrawBoard()
{
	int currentY = Position.Y;

	SetCursorPosition(Position.X, Position.Y);
	cout << "          " <<m_strName << " Ships Remaining: " << m_nShipsRemaining;
	SetCursorPosition(Position.X, ++currentY);
	cout << "   A B C D E F G H I J";
	SetCursorPosition(Position.X, ++currentY);
	cout << "  ---------------------";
	SetCursorPosition(Position.X, ++currentY);
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		cout << y << ' ';
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			cout << '|' << m_arrBoard[x][y];
			if (x == BOARD_WIDTH - 1)
			{
				cout << '|';
				SetCursorPosition(Position.X, ++currentY);
			}
		}
		cout << "  ---------------------";
		SetCursorPosition(Position.X, ++currentY);
	}

	int attX = Position.X + 26;

	currentY = ++Position.Y;
	SetCursorPosition(attX, Position.Y);
	cout << "   A B C D E F G H I J";
	SetCursorPosition(attX, ++currentY);
	cout << "  ---------------------";
	SetCursorPosition(attX, ++currentY);
	for (int y = 0; y < BOARD_HEIGHT; y++)
	{
		cout << y << ' ';
		for (int x = 0; x < BOARD_WIDTH; x++)
		{
			cout << '|' << m_arrAttBoard[x][y];
			if (x == BOARD_WIDTH - 1)
			{
				cout << '|';
				SetCursorPosition(attX, ++currentY);
			}
		}
		cout << "  ---------------------";
		SetCursorPosition(attX, ++currentY);
	}

}

int Board::CheckShot(int x, int y)
{
	if (m_arrBoard[x][y] == ' ')
	{
		m_arrBoard[x][y] = 'O';
		return 0;
	}
	else if (m_arrBoard[x][y] == '#')
	{
		m_arrBoard[x][y] = 'X';
		return 1;
	}
	else if (m_arrBoard[x][y] == 'X' || m_arrBoard[x][y] == 'O')
	{
		return 2;
	}

	return 2;
}

void Board::MarkShot(int x, int y, int result)
{
	switch (result)
	{
	case 0:
		m_arrAttBoard[x][y] = 'O';
		m_cOldValue = 'O';
		break;
	case 1:
		m_arrAttBoard[x][y] = 'X';
		m_cOldValue = 'X';
		break;
	default:
		break;
	}
}

void Board::SetCursorPosition(int x, int y)
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

bool Board::CheckShipPlacement(int index)
{
	for (int i = 0; i < 5; i++)
	{
		if (i == index || m_arrShips[i].isSet == false)
		{
			continue;
		}

		for (int j = 0; j < (int)m_arrShips[i].indexPositions.size(); j++)
		{
			for (int k = 0; k < (int)m_arrShips[index].indexPositions.size(); k++)
			{
				if (m_arrShips[i].indexPositions[j].X == m_arrShips[index].indexPositions[k].X && m_arrShips[i].indexPositions[j].Y == m_arrShips[index].indexPositions[k].Y)
				{
					return false;
				}
			}
		}
	}

	return true;
}

void Board::SetShip(int index)
{
	m_arrShips[index].isAlive = true;
	m_arrShips[index].isSet = true;
}

void Board::ActivateTempShip(int index)
{
	int offSet = 1;
	for (int i = 0; i < m_arrShips[index].length; i++)
	{
		if (i == 0)
		{
			m_arrShips[index].indexPositions[0] = m_arrShips[index].position;
		}
		else
		{
			if (m_arrShips[index].horizontal)
			{
				m_arrShips[index].indexPositions[i].X = m_arrShips[index].indexPositions[i - 1].X + offSet;
				m_arrShips[index].indexPositions[i].Y = m_arrShips[index].indexPositions[0].Y;
			}
			else
			{
				m_arrShips[index].indexPositions[i].X = m_arrShips[index].indexPositions[0].X;
				m_arrShips[index].indexPositions[i].Y = m_arrShips[index].indexPositions[i - 1].Y + offSet;
			}
		}

		m_arrBoard[m_arrShips[index].indexPositions[i].X][m_arrShips[index].indexPositions[i].Y] = '#';
	}
}

void Board::ActiveShipCheck()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_arrShips[i].isSet)
		{
			for (int j = 0; j < m_arrShips[i].length; j++)
			{
				m_arrBoard[m_arrShips[i].indexPositions[j].X][m_arrShips[i].indexPositions[j].Y] = '#';
			}
		}
	}
}

void Board::ActivateTempShot(int x, int y)
{
	if (pastPosition.X == x && pastPosition.Y == y)
	{
		m_arrAttBoard[x][y] = 'T';
	}
	else
	{	
		m_arrAttBoard[pastPosition.X][pastPosition.Y] = m_cOldValue;
		m_cOldValue = m_arrAttBoard[x][y];
		m_arrAttBoard[x][y] = 'T';
		pastPosition = COORD{ x, y };
	}
}

int Board::CheckShipsRemaining()
{
	int result = 5;

	for (int i = 0; i < 5; i++)
	{
		if (m_arrShips[i].isAlive == false)
		{
			result--;
			continue;
		}

		int hitCount = 0;
		for (int j = 0; j < (int)m_arrShips[i].indexPositions.size(); j++)
		{
			if (m_arrBoard[m_arrShips[i].indexPositions[j].X][m_arrShips[i].indexPositions[j].Y] == 'X')
			{
				hitCount++;
			}
		}

		if (hitCount == m_arrShips[i].indexPositions.size())
		{
			m_arrShips[i].isAlive = false;
			result--;
		}
	}

	m_nShipsRemaining = result;
	return result;
}

void Board::ResetShips()
{
	for (int i = 0; i < 5; i++)
	{
		m_arrShips[i].horizontal = false;
		m_arrShips[i].isAlive = false;
		m_arrShips[i].isSet = false;
		m_arrShips[i].position = COORD{ 0, 0 };
	}

	pastPosition = COORD{ 5, 5 };
	m_cOldValue = ' ';

	m_nShipsRemaining = 5;
}