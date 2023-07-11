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
#include "main.h"
using namespace std;

class SetUp {
public:
	const string iniFilePath = "config.ini";
	bool gameStarted = false;
	const int BOARD_SIZE = MainClass::BOARD_SIZE;

	MainClass::Ship enemyShip;

	bool fileExists(const std::string filePath);
	void extractSettings(const std::string filePath);
	void createSettingsFile(const std::string filePath);
	bool isInteger(std::string value);

	void startGame(HWND mainScreen, HDC hdc, PAINTSTRUCT ps, HWND titleBanner, HWND startButton, HWND quitButton, HWND mainWindow);

	void drawGrid(HDC hdc);
	void initializeBoard();
	bool checkShipIntersection(const MainClass::Ship& ship1, const MainClass::Ship& ship2);
	void generateEnemyShips();
	void generateShips(HWND mainWindow);
	void clearBoard();
	bool getConfirmation(HWND hWnd, const std::wstring& message);
	std::vector<int> shipLengths;

	class Settings {
	public:
		struct Ship {
			int size;
		};

		std::vector<Ship> ships;

		Settings(const std::vector<Ship>& ships)
			: ships(ships) {}
	};
};