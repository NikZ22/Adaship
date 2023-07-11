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
#include "main.h"
using namespace std;

extern MainClass mainClass;

//Checks if file exists with given path
bool SetUp::fileExists(const string filePath) {
    ifstream file(filePath);
    return file.good();
}

//This will extract the information from the config.ini file and populate the settings class that will be used to generate
// the ships.
void SetUp::extractSettings(const string filePath) {
    fstream settingsFile;
    string line;
    string category;
    string delimiter = "=";
    vector<Settings::Ship> shipSettings;

    settingsFile.open(filePath, ios::in);

    settingsFile.clear();
    settingsFile.seekg(0, ios::beg);

    while (getline(settingsFile, line)) {
//This checks which type of settings is being copied over and let the next lines be put into the ships setting for this 
// example
        if (line.find("Ships") != string::npos) {
            category = line;
            continue;
        }

        if (category.find("Ships") != string::npos) {
            size_t pos = line.find(delimiter);
            string value = line.substr(pos + delimiter.length());
 
//This will attempt to put the extracted number into the class, if nots an int it will error out and carry on reading the
// rest of the file
            try {
                shipSettings.push_back({ stoi(value) });
            }
            catch (exception e) {
                cout << "Error:" << e.what() << endl;
            }
            continue;
        }
    }

    settingsFile.close();

//This is then placed in an array that is later read for ship generation
    for (const Settings::Ship& ship : shipSettings) {
        shipLengths.push_back(ship.size);
    }

//If config.ini exists but is empty/all invalid value/no values set for [Ships], it will give a default array and regenerate
// the config.ini file
    if (shipLengths.size() == 0) {
        shipLengths = { 2,3,3,4,5 };
        createSettingsFile(filePath);
    }
}

//This checks if a given value is an integer
bool SetUp::isInteger(string value) {
    try {
        size_t pos;
        int intValue = stoi(value, &pos);
        return pos == value.length();
    }
    catch (const exception) {
        return false;
    }
}

//This create a new config.ini file with default settings being set
void SetUp::createSettingsFile(const string filePath) {
    const string defaultSettings = "[Ships]\nCarrier=5\nBattleship=4\nDestroyer=3\nSubmarine=3\nPatrol=2";

    ofstream settingsFile(filePath);

    settingsFile << defaultSettings;

    settingsFile.close();
}

//This is ran when the start button is clicked and will delete all objects on the screen and generate the grids and ships 
void SetUp::startGame(HWND mainScreen, HDC hdc, PAINTSTRUCT ps, HWND titleBanner, HWND startButton, HWND quitButton, HWND mainWindow) {
        gameStarted = true;
        DestroyWindow(titleBanner);
        DestroyWindow(startButton);
        DestroyWindow(quitButton);

        // Generate board and ships, and start the game
        hdc = BeginPaint(mainScreen, &ps);
        drawGrid(hdc);
        EndPaint(mainScreen, &ps);
        initializeBoard();
        generateEnemyShips();
        generateShips(mainWindow);
        InvalidateRect(mainWindow, NULL, TRUE);
}

//This draws out both grids on the screen
void SetUp::drawGrid(HDC hdc) {
    const int GRID_OFFSET = mainClass.GRID_OFFSET;
    const int TITLE_HEIGHT = mainClass.TITLE_HEIGHT;
    const int BOARD_SIZE = mainClass.BOARD_SIZE;
    const int CELL_SIZE = mainClass.CELL_SIZE;
    const int WINDOW_WIDTH = mainClass.WINDOW_WIDTH;

    // Draw player board title
    RECT playerTitleRect;
    playerTitleRect.left = GRID_OFFSET;
    playerTitleRect.top = GRID_OFFSET - TITLE_HEIGHT;
    playerTitleRect.right = GRID_OFFSET + (BOARD_SIZE * CELL_SIZE);
    playerTitleRect.bottom = GRID_OFFSET;
    DrawTextW(hdc, L"Player Board", -1, &playerTitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Draw enemy board title
    RECT enemyTitleRect;
    enemyTitleRect.left = GRID_OFFSET + (BOARD_SIZE + 2) * CELL_SIZE;
    enemyTitleRect.top = GRID_OFFSET - TITLE_HEIGHT;
    enemyTitleRect.right = enemyTitleRect.left + (BOARD_SIZE * CELL_SIZE);
    enemyTitleRect.bottom = GRID_OFFSET;
    DrawTextW(hdc, L"Enemy Board", -1, &enemyTitleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    //Draws out each players cells
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int playerCellX = col * CELL_SIZE + GRID_OFFSET;
            int playerCellY = row * CELL_SIZE + GRID_OFFSET;

            int enemyCellX = col * CELL_SIZE + GRID_OFFSET + (BOARD_SIZE + 2) * CELL_SIZE;
            int enemyCellY = row * CELL_SIZE + GRID_OFFSET;

            // Draw player board cell
            SelectObject(hdc, mainClass.whiteBrush);
            Rectangle(hdc, playerCellX, playerCellY, playerCellX + CELL_SIZE, playerCellY + CELL_SIZE);
            mainClass.drawCell(hdc, row, col, mainClass.enemyBoard[row][col], "player");

            // Draw enemy board cell
            SelectObject(hdc, mainClass.whiteBrush);
            Rectangle(hdc, enemyCellX, enemyCellY, enemyCellX + CELL_SIZE, enemyCellY + CELL_SIZE);
            mainClass.drawCell(hdc, row, col, mainClass.playerBoard[row][col], "enemy");
        }
    }

    mainClass.drawMessage(hdc, L"Click on the left board to make a guess");
}

//This will set every state of all cells to empty to be filled later
void SetUp::initializeBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            mainClass.playerBoard[row][col] = MainClass::CellState::Empty;
            mainClass.enemyBoard[row][col] = MainClass::CellState::Empty;
        }
    }
}

//This check if ships intersect and ensure that each ship has its own cell on the board
bool SetUp::checkShipIntersection(const MainClass::Ship& ship1, const MainClass::Ship& ship2) {
    // Check if ships intersect by comparing their start and end coordinates
    if (ship1.startRow <= ship2.endRow && ship1.endRow >= ship2.startRow &&
        ship1.startCol <= ship2.endCol && ship1.endCol >= ship2.startCol) {
        if ((ship1.endRow == ship2.startRow - 1 && ship1.endCol == ship2.startCol - 1) ||
            (ship1.startRow == ship2.endRow + 1 && ship1.startCol == ship2.endCol + 1) ||
            (ship1.startRow == ship2.endRow + 1 && ship1.endCol == ship2.startCol - 1) ||
            (ship1.endRow == ship2.startRow - 1 && ship1.startCol == ship2.endCol + 1)) {
            return true; // Ships intersect at a corner
        }
        return true; // Ships intersect
    }
    return false; // Ships do not intersect
}

//This will generate enemy ships for the player to find on their board
void SetUp::generateEnemyShips() {
    chrono::high_resolution_clock::duration seed = chrono::high_resolution_clock::now().time_since_epoch();
    srand(static_cast<unsigned>(seed.count()));

//This will go through all the ship sizes extracted and find a place for them on the grid
    for (int i = 0; i < shipLengths.size(); i++) {
        bool validPlacement = false;

        while (!validPlacement) {
            enemyShip.startRow = rand() % BOARD_SIZE;
            enemyShip.startCol = rand() % BOARD_SIZE;
            int direction = rand() % 2;

//This determines if the ships is placed horizontally or vertically
            if (direction == 0) {
                if (enemyShip.startRow + shipLengths[i] >= BOARD_SIZE) {
                    enemyShip.startRow -= shipLengths[i];
                }
                enemyShip.endRow = enemyShip.startRow + shipLengths[i] - 1;
                enemyShip.endCol = enemyShip.startCol;
            }
            else {
                if (enemyShip.startCol + shipLengths[i] >= BOARD_SIZE) {
                    enemyShip.startCol -= shipLengths[i];
                }
                enemyShip.endRow = enemyShip.startRow;
                enemyShip.endCol = enemyShip.startCol + shipLengths[i] - 1;
            }

            // Check if the ship intersects with any existing ship
            bool intersects = false;
            for (const auto& existingShip : mainClass.enemyShips) {
                if (checkShipIntersection(enemyShip, existingShip)) {
                    intersects = true;
                    break;
                }
            }

            if (!intersects) {
                validPlacement = true;
            }
        }

        // Place ship cells on the board (Will not mark cells for enemy ships)
        for (int row = enemyShip.startRow; row <= enemyShip.endRow; row++) {
            for (int col = enemyShip.startCol; col <= enemyShip.endCol; col++) {
                mainClass.enemyBoard[row][col] = MainClass::CellState::Ship;
            }
        }

        mainClass.enemyShips.push_back(enemyShip);
    }
}

//This clears the players board if they are not happy with their ship placement
void SetUp::clearBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            mainClass.playerBoard[row][col] = MainClass::CellState::Empty;
        }
    }
    mainClass.ships.clear();
}

//This will give the user of the option to reroll their ship placement or to lock in and play
bool SetUp::getConfirmation(HWND hWnd, const std::wstring& message) {
    int result = MessageBoxW(hWnd, message.c_str(), L"Confirm Choice", MB_YESNO);
    return (result == IDYES);
}

//This will generate the player ships
void SetUp::generateShips(HWND mainWindow) {
    std::chrono::high_resolution_clock::duration seed = std::chrono::high_resolution_clock::now().time_since_epoch();
    srand(static_cast<unsigned>(seed.count()));
    bool confirmChoice = false;

    while (!confirmChoice) {
//Clear the players stored ships coords and board if they do not confirm choice
        clearBoard();
        mainClass.ships.clear(); 

//Cycles through each ship length from config file
        for (int i = 0; i < shipLengths.size(); i++) {
            MainClass::Ship ship;
            bool validPlacement = false;

            while (!validPlacement) {
                ship.startRow = rand() % BOARD_SIZE;
                ship.startCol = rand() % BOARD_SIZE;
                int direction = rand() % 2;

                if (direction == 0) {
                    if (ship.startRow + shipLengths[i] >= BOARD_SIZE) {
                        ship.startRow -= shipLengths[i];
                    }
                    ship.endRow = ship.startRow + shipLengths[i] - 1;
                    ship.endCol = ship.startCol;
                }
                else {
                    if (ship.startCol + shipLengths[i] >= BOARD_SIZE) {
                        ship.startCol -= shipLengths[i];
                    }
                    ship.endRow = ship.startRow;
                    ship.endCol = ship.startCol + shipLengths[i] - 1;
                }

                // Check if the ship intersects with any existing ship
                bool intersects = false;
                for (const auto& existingShip : mainClass.ships) {
                    if (checkShipIntersection(ship, existingShip)) {
                        intersects = true;
                        break;
                    }
                }

                if (!intersects) {
                    validPlacement = true;
                }
            }

            // Mark ship cells on the board
            for (int row = ship.startRow; row <= ship.endRow; row++) {
                for (int col = ship.startCol; col <= ship.endCol; col++) {
                    mainClass.playerBoard[row][col] = MainClass::CellState::Ship;
                }
            }

            mainClass.ships.push_back(ship);
        }

        // Redraw the board with ship cells highlighted
        HDC hdc = GetDC(mainWindow);
        drawGrid(hdc);

        confirmChoice = getConfirmation(mainWindow, L"Are you happy with the placement of your ships? If not, select no to reroll the position of them randomly.");
    }
}