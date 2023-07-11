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
#include "SetUp.h"
#include "DetermineWinner.h"
#include "main.h"
using namespace std;

extern SetUp setUp;
extern DetermineWinner determineWinner;

//Outputs a message to the user
void MainClass::drawMessage(HDC hdc, const wstring message) {
    RECT rect;

    rect.left = GRID_OFFSET;
    rect.top = (BOARD_SIZE + 1) * CELL_SIZE + GRID_OFFSET;
    rect.right = WINDOW_WIDTH - GRID_OFFSET;
    rect.bottom = rect.top + MESSAGE_HEIGHT;

    DrawTextW(hdc, message.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

//Colours individual cell on grid in appropiate colour depending on cell state
void MainClass::drawCell(HDC hdc, int row, int col, CellState state, string board) {
    int cellX = col * CELL_SIZE + GRID_OFFSET;
    int cellY = row * CELL_SIZE + GRID_OFFSET;

    int enemyCellX = col * CELL_SIZE + GRID_OFFSET + (BOARD_SIZE + 2) * CELL_SIZE;
    int enemyCellY = row * CELL_SIZE + GRID_OFFSET;

    switch (state) {
    case CellState::Ship:
        if (board == "enemy") {
            SelectObject(hdc, greenBrush);
        }
        break;

    case CellState::Hit:
        SelectObject(hdc, redBrush);
        break;

    case CellState::Miss:
        SelectObject(hdc, grayBrush);
        break;

    default:
        return;
    }

    if (board == "player") {
        Rectangle(hdc, cellX, cellY, cellX + CELL_SIZE, cellY + CELL_SIZE);
    }
    else if (board == "enemy") {
        Rectangle(hdc, enemyCellX, enemyCellY, enemyCellX + CELL_SIZE, enemyCellY + CELL_SIZE);
    }
}

//This will process a players click on their board
void MainClass::processClick(int x, int y) {
    int row = y / CELL_SIZE;
    int col = x / CELL_SIZE;

    if (row >= BOARD_SIZE || col >= BOARD_SIZE) {
        return;
    }

    if (enemyBoard[row][col] == CellState::Hit || enemyBoard[row][col] == CellState::Miss) {
        return;
    }

//This calls to the functions that marks whether it was a hit or miss
    processGuess(row, col);

//This checks if the user has hit all enemy ship cells
    if (determineWinner.userWins()) {
        HDC hdc = GetDC(mainWindow);
        drawMessage(hdc, L"Game Over! You sunk all the enemy ships.");
        MessageBoxW(mainWindow, L"You win! You sunk all the enemy ships.", L"Game Over", MB_OK);
        exit(0);
    }

//This makes an enemy take a random shot on the board with your ships on them
    computerShot();

//This checks if the computer has hit all player ship cells
    if (determineWinner.opponentWins()) {
        HDC hdc = GetDC(mainWindow);
        drawMessage(hdc, L"Game Over! All your ships have been sunk.");
        MessageBoxW(mainWindow, L"You lose! All your ships have been sunk.", L"Game Over", MB_OK);
        exit(0);
    }
}

//Determine if the hit was a hit or miss
void MainClass::processGuess(int row, int col) {
    for (Ship& ship : enemyShips) {
        if (row >= ship.startRow && row <= ship.endRow && col >= ship.startCol && col <= ship.endCol) {
            enemyBoard[row][col] = CellState::Hit;
            return;
        }
    }

    enemyBoard[row][col] = CellState::Miss;
}

//Make an 'AI' randomly shoot on the board with player ship cells
void MainClass::computerShot() {
    bool shotFired = false;

    while (!shotFired) {
        shotFired = true;
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;

//Determins whether enemy shot was hit or miss
        switch (playerBoard[row][col]) {
            case(CellState::Ship):
                playerBoard[row][col] = CellState::Hit;
                break;
            case(CellState::Empty):
                playerBoard[row][col] = CellState::Miss;
                break;
            default:
                shotFired = false;
                break;
        }     
    }
}

//This is to quit out the game with quit button on the main menu
void MainClass::quitGame() {
    // Close the application
    DestroyWindow(mainWindow);
    exit(0);
}