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

class DetermineWinner {
public:
	MainClass::Ship enemyShip;

	bool userWins();
	bool opponentWins();
};