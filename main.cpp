#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <ctime>
#include <string>
#include <chrono>
using namespace std;

const int BOARD_SIZE = 10;
const int SHIP_SIZE = 5;
const int CELL_SIZE = 30;
const int WINDOW_WIDTH = BOARD_SIZE * CELL_SIZE + 50;
const int WINDOW_HEIGHT = (BOARD_SIZE + 1) * CELL_SIZE + 100;
const int GRID_OFFSET = 10;
const int MESSAGE_HEIGHT = 40;
const int TITLE_HEIGHT = 10;

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void InitializeBoard();
void GenerateShips();
void GenerateEnemyShips();
void PlaceShip(HDC hdc, int row, int col);
void DrawGrid(HDC hdc);
void DrawCell(HDC hdc, int row, int col, CellState state, string board);
void DrawMessage(HDC hdc, const std::wstring& message);
void ProcessClick(int x, int y);
void ProcessGuess(int row, int col);
void computerShot();
bool userWins();
bool opponentWins();
bool getConfirmation(HWND hWnd, const std::wstring& message);
void clearBoard();

HWND g_hWnd;
HBRUSH g_hBrushWhite;
HBRUSH g_hBrushBlue;
HBRUSH g_hBrushGray;
HBRUSH g_hBrushRed;
HBRUSH g_hBrushGreen;
int alphaForRedBrush = 128;
std::vector<std::vector<CellState>> g_playerBoard(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::Empty));
std::vector<std::vector<CellState>> g_enemyBoard(BOARD_SIZE, std::vector<CellState>(BOARD_SIZE, CellState::Empty));
std::vector<Ship> g_ships;
vector<Ship> enemyShips;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    // Initialize window class
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = "BattleshipClass";

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    hWnd = CreateWindowEx(
        0,
        "BattleshipClass",
        "Battleship",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH * 2, WINDOW_HEIGHT * 1.5,
        NULL, NULL, hInstance, NULL
    );

    if (hWnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    g_hWnd = hWnd;

    // Initialize brushes
    g_hBrushWhite = CreateSolidBrush(RGB(255, 255, 255));
    g_hBrushBlue = CreateSolidBrush(RGB(0, 0, 255));
    g_hBrushGray = CreateSolidBrush(RGB(128, 128, 128));
    g_hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
    g_hBrushGreen = CreateSolidBrush(RGB(0, 255, 0));

    // Initialize the game board
    InitializeBoard();

    // Generate ships
    GenerateEnemyShips();
    GenerateShips();

    // Display the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Game loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Clean up
    DeleteObject(g_hBrushWhite);
    DeleteObject(g_hBrushBlue);
    DeleteObject(g_hBrushGray);

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        DrawGrid(hdc);
        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDOWN:
    {
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        ProcessClick(xPos, yPos);
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

void InitializeBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            g_playerBoard[row][col] = CellState::Empty;
            g_enemyBoard[row][col] = CellState::Empty;
        }
    }
}

void GenerateShips() {
    chrono::high_resolution_clock::duration seed = chrono::high_resolution_clock::now().time_since_epoch();
    srand(static_cast<unsigned>(seed.count()));
    bool confirmChoice = false;

    while (!confirmChoice) {
        clearBoard();
        for (int i = 0; i < SHIP_SIZE; i++) {
            Ship ship;
            ship.startRow = rand() % BOARD_SIZE;
            ship.startCol = rand() % BOARD_SIZE;
            int direction = rand() % 2;

            if (direction == 0) {
                if (ship.startRow + SHIP_SIZE >= BOARD_SIZE) {
                    ship.startRow -= SHIP_SIZE;
                }
                ship.endRow = ship.startRow + SHIP_SIZE - 1;
                ship.endCol = ship.startCol;
            }
            else {
                if (ship.startCol + SHIP_SIZE >= BOARD_SIZE) {
                    ship.startCol -= SHIP_SIZE;
                }
                ship.endRow = ship.startRow;
                ship.endCol = ship.startCol + SHIP_SIZE - 1;
            }
            // Mark ship cells on the board
            for (int row = ship.startRow; row <= ship.endRow; row++) {
                for (int col = ship.startCol; col <= ship.endCol; col++) {
                    g_playerBoard[row][col] = CellState::Ship;
                }
            }

            g_ships.push_back(ship);
        }

        // Redraw the board with ship cells highlighted
        HDC hdc = GetDC(g_hWnd);
        DrawGrid(hdc);

        confirmChoice = getConfirmation(g_hWnd, L"Are you happy with the placement of your ships?");
    }
}

void GenerateEnemyShips() {
    chrono::high_resolution_clock::duration seed = chrono::high_resolution_clock::now().time_since_epoch();
    srand(static_cast<unsigned>(seed.count()));
    
    for (int i = 0; i < SHIP_SIZE; i++) {
         Ship enemyShip;
         enemyShip.startRow = rand() % BOARD_SIZE;
         enemyShip.startCol = rand() % BOARD_SIZE;
         int direction = rand() % 2;

         if (direction == 0) {
            if (enemyShip.startRow + SHIP_SIZE >= BOARD_SIZE) {
                enemyShip.startRow -= SHIP_SIZE;
            }
            enemyShip.endRow = enemyShip.startRow + SHIP_SIZE - 1;
            enemyShip.endCol = enemyShip.startCol;
         }
         else {
            if (enemyShip.startCol + SHIP_SIZE >= BOARD_SIZE) {
                enemyShip.startCol -= SHIP_SIZE;
            }
            enemyShip.endRow = enemyShip.startRow;
            enemyShip.endCol = enemyShip.startCol + SHIP_SIZE - 1;
         }

            // Mark ship cells on the board
         for (int row = enemyShip.startRow; row <= enemyShip.endRow; row++) {
            for (int col = enemyShip.startCol; col <= enemyShip.endCol; col++) {
                g_enemyBoard[row][col] = CellState::Ship;
            }
         }

         enemyShips.push_back(enemyShip);
    }
}

void DrawGrid(HDC hdc) {
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


    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            int playerCellX = col * CELL_SIZE + GRID_OFFSET;
            int playerCellY = row * CELL_SIZE + GRID_OFFSET;

            int enemyCellX = col * CELL_SIZE + GRID_OFFSET + (BOARD_SIZE + 2) * CELL_SIZE;
            int enemyCellY = row * CELL_SIZE + GRID_OFFSET;

            // Draw player board cell
            SelectObject(hdc, g_hBrushWhite);
            Rectangle(hdc, playerCellX, playerCellY, playerCellX + CELL_SIZE, playerCellY + CELL_SIZE);
            DrawCell(hdc, row, col, g_enemyBoard[row][col], "player");

            // Draw enemy board cell
            SelectObject(hdc, g_hBrushWhite);
            Rectangle(hdc, enemyCellX, enemyCellY, enemyCellX + CELL_SIZE, enemyCellY + CELL_SIZE);
            DrawCell(hdc, row, col, g_playerBoard[row][col], "enemy");
        }
    }

    DrawMessage(hdc, L"Click on the enemy board to make a guess.");
}

void DrawCell(HDC hdc, int row, int col, CellState state, string board) {
    int cellX = col * CELL_SIZE + GRID_OFFSET;
    int cellY = row * CELL_SIZE + GRID_OFFSET;

    int enemyCellX = col * CELL_SIZE + GRID_OFFSET + (BOARD_SIZE + 2) * CELL_SIZE;
    int enemyCellY = row * CELL_SIZE + GRID_OFFSET;

    switch (state) {
        case CellState::Ship:
            SelectObject(hdc, g_hBrushGreen);
            break;

        case CellState::Hit:
            SelectObject(hdc, g_hBrushRed);
            break;

        case CellState::Miss:
            SelectObject(hdc, g_hBrushGray);
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

void DrawMessage(HDC hdc, const std::wstring& message) {
    RECT rect;
    rect.left = GRID_OFFSET;
    rect.top = (BOARD_SIZE + 1) * CELL_SIZE + GRID_OFFSET;
    rect.right = WINDOW_WIDTH - GRID_OFFSET;
    rect.bottom = rect.top + MESSAGE_HEIGHT;

    DrawTextW(hdc, message.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void ProcessClick(int x, int y) {
    int row = y / CELL_SIZE;
    int col = x / CELL_SIZE;

    if (row >= BOARD_SIZE || col >= BOARD_SIZE) {
        return;
    }

    if (g_enemyBoard[row][col] == CellState::Hit || g_enemyBoard[row][col] == CellState::Miss) {
        return;
    }

    ProcessGuess(row, col);

    if (userWins()) {
        HDC hdc = GetDC(g_hWnd);
        DrawMessage(hdc, L"Game Over! You sunk all the ships.");
        MessageBoxW(g_hWnd, L"You win! You sunk all the ships.", L"Game Over", MB_OK);
        exit(0);
    }
    computerShot();

    if (opponentWins()) {
        HDC hdc = GetDC(g_hWnd);
        DrawMessage(hdc, L"Game Over! All your ships have been sunk.");
        MessageBoxW(g_hWnd, L"You lose! All your ships have been sunk.", L"Game Over", MB_OK);
        exit(0);
    }
}

void ProcessGuess(int row, int col) {
    for (Ship& ship : enemyShips) {
        if (row >= ship.startRow && row <= ship.endRow && col >= ship.startCol && col <= ship.endCol) {
            g_enemyBoard[row][col] = CellState::Hit;
            return;
        }
    }

    g_enemyBoard[row][col] = CellState::Miss;
}

bool userWins() {
    for (const auto& row : g_enemyBoard) {
        for (const auto& cell : row) {
            if (cell == CellState::Ship)
                return false;
        }
    }

    return true;
}

bool opponentWins() {
    for (const auto& row : g_playerBoard) {
        for (const auto& cell : row) {
            if (cell == CellState::Ship)
                return false;
        }
    }

    return true;
}

bool getConfirmation(HWND hWnd, const std::wstring& message) {
    int result = MessageBoxW(hWnd, message.c_str(), L"Confirm Choice", MB_YESNO);
    return (result == IDYES);
}

void clearBoard() {
    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            g_playerBoard[row][col] = CellState::Empty;
        }
    }
    g_ships.clear();
}

void computerShot() {
    bool shotFired = false;

    while (!shotFired) {
        shotFired = true;
        int row = rand() % BOARD_SIZE;
        int col = rand() % BOARD_SIZE;

        switch (g_playerBoard[row][col])
        {
        case(CellState::Ship):
            g_playerBoard[row][col] = CellState::Hit;
            break;
        case(CellState::Empty):
            g_playerBoard[row][col] = CellState::Miss;
            break;
        default:
            shotFired = false;
            break;
        }

        //if (g_playerBoard[row][col] == CellState::Miss || g_playerBoard[row][col] == CellState::Hit) { shotFired = false; }
        //if (g_playerBoard[row][col] == CellState::Ship) {
           // g_playerBoard[row][col] = CellState::Hit;
       // }
       // if(g_playerBoard[row][col] == CellState::Empty) { g_playerBoard[row][col] = CellState::Miss; }
        
    }
}
