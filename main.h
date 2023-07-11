#pragma once

#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <ctime>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

class MainClass {
public:
	static constexpr int BOARD_SIZE = 10;
	static constexpr int CELL_SIZE = 30;
	static constexpr int WINDOW_WIDTH = BOARD_SIZE * CELL_SIZE + 50;
	static constexpr int WINDOW_HEIGHT = (BOARD_SIZE + 1) * CELL_SIZE + 100;
	static constexpr int GRID_OFFSET = 10;
	static constexpr int MESSAGE_HEIGHT = 40;
	static constexpr int TITLE_HEIGHT = 10;
	static constexpr int ID_START_GAME_BUTTON = 1001;
	static constexpr int ID_QUIT_GAME_BUTTON = 1002;
	static constexpr int BUTTON_WIDTH = 100;
	static constexpr int BUTTON_HEIGHT = 30;
	static constexpr int BUTTON_MARGIN = 10;
	static constexpr int BUTTON_TOP = WINDOW_HEIGHT - BUTTON_HEIGHT - BUTTON_MARGIN;

	enum class CellState {
		Empty,
		Ship,
		Hit,
		Miss
	};

	struct Ship {
		int startRow;
		int startCol;
		int endRow;
		int endCol;
	};

	HWND mainWindow;
	HWND mainTitle;
	HWND startButton;
	HWND quitButton;

	HBRUSH whiteBrush;
	HBRUSH grayBrush;
	HBRUSH redBrush;
	HBRUSH greenBrush;

	std::vector<std::vector<CellState>> playerBoard;
	std::vector<std::vector<CellState>> enemyBoard;
	std::vector<Ship> ships;
	std::vector<Ship> enemyShips;

	MainClass() {
		playerBoard.resize(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::Empty));
		enemyBoard.resize(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::Empty));
	}
	
	void processClick(int x, int y);
	void processGuess(int row, int col);
	void computerShot();
	void drawCell(HDC hdc, int row, int col, CellState state, std::string board);
	void drawMessage(HDC hdc, const std::wstring message);
	void quitGame();
private:
};