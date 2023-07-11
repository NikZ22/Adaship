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
#include "DetermineWinner.h"
#include "main.h"
using namespace std;

extern MainClass mainClass;

//This checks if any enemy ship cells are still on the board
bool DetermineWinner::userWins() {
    for (const auto& row : mainClass.enemyBoard) {
        for (const auto& cell : row) {
            if (cell == MainClass::CellState::Ship)
                return false;
        }
    }

    return true;
}

//This checks if any player ship cells are still on the board
bool DetermineWinner::opponentWins() {
    for (const auto& row : mainClass.playerBoard) {
        for (const auto& cell : row) {
            if (cell == MainClass::CellState::Ship)
                return false;
        }
    }

    return true;
}