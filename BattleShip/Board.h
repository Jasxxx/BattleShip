#pragma once
#include <Windows.h>
#include <string>
#include <vector>

using namespace std;

class Board
{
	COORD Position;

	struct Ship
	{
		COORD position;
		vector<COORD> indexPositions;
		int length;
		bool horizontal;
		bool isSet;
		bool isAlive;
	};

	COORD pastPosition;
	char m_cOldValue;
	int m_nShipsRemaining;

public:
	string m_strName;
	Ship m_arrShips[5];
	Board();
	~Board();

	char m_arrBoard[23][10];
	char m_arrAttBoard[23][10];

	void ResetBoard();
	void DrawBoard();
	int CheckShot(int x, int y);
	void MarkShot(int x, int y, int result);

	void SetName(string name) { m_strName = name; }

	void SetPosition(int x, int y) { Position = COORD{ x, y }; }

	void SetCursorPosition(int x, int y);

	bool CheckShipPlacement(int index);
	void SetShip(int index);

	void ActivateTempShip(int index);
	void ActiveShipCheck();
	void ActivateTempShot(int x, int y);

	int CheckShipsRemaining();

	void ResetShips();
};

