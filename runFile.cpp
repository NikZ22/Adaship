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

SetUp setUp;
MainClass mainClass;
DetermineWinner determineWinner;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//This is the main function that gets ran on start
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

//This will extract from the settings file or create a default config and then extract settings
    if (setUp.fileExists(setUp.iniFilePath)) {
        setUp.extractSettings(setUp.iniFilePath);
    }
    else {
        setUp.createSettingsFile(setUp.iniFilePath);
        setUp.extractSettings(setUp.iniFilePath);
    }

//Initialize window class
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"BattleshipClass";

 //Check if class already exists
    if (!RegisterClassEx(&wcex)) {
        MessageBoxA(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

//Create the main window
    hWnd = CreateWindowExA(
        0,
        "BattleshipClass",
        "Battleship",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, mainClass.WINDOW_WIDTH * 2, mainClass.WINDOW_HEIGHT * 1.5,
        NULL, NULL, hInstance, NULL
    );

    if (hWnd == NULL) {
        MessageBoxA(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    mainClass.mainWindow = hWnd;

//Create title component
    mainClass.mainTitle = CreateWindowW(
        L"STATIC",          
        L"Battleship",      
        WS_VISIBLE | WS_CHILD,  
        mainClass.GRID_OFFSET,                  
        50,                  
        mainClass.WINDOW_WIDTH * 2,   
        50,       
        hWnd,               
        NULL,               
        hInstance,          
        NULL                
    );
    SendMessageW(mainClass.mainTitle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));


//Initialize brushes
    mainClass.whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
    mainClass.grayBrush = CreateSolidBrush(RGB(128, 128, 128));
    mainClass.redBrush = CreateSolidBrush(RGB(255, 0, 0));
    mainClass.greenBrush = CreateSolidBrush(RGB(0, 255, 0));

//Create start game button
    mainClass.startButton = CreateWindowW(
        L"BUTTON",          
        L"Start Game",      
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
        mainClass.GRID_OFFSET,        
        mainClass.BUTTON_TOP,         
        mainClass.BUTTON_WIDTH,       
        mainClass.BUTTON_HEIGHT,      
        hWnd,               
        (HMENU)mainClass.ID_START_GAME_BUTTON,    
        hInstance,          
        NULL                
    );

//Create quit game button
    mainClass.quitButton = CreateWindowW(
        L"BUTTON",          
        L"Quit Game",       
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
        mainClass.GRID_OFFSET + mainClass.BUTTON_WIDTH + mainClass.BUTTON_MARGIN,    
        mainClass.BUTTON_TOP,         
        mainClass.BUTTON_WIDTH,       
        mainClass.BUTTON_HEIGHT,      
        hWnd,               
        (HMENU)mainClass.ID_QUIT_GAME_BUTTON,     
        hInstance,          
        NULL                
    );

//Display the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

//Game loop
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

//Clean up
    DeleteObject(mainClass.whiteBrush);
    DeleteObject(mainClass.grayBrush);

    return (int)msg.wParam;
}

//This is an active callback listener that processes each interaction that happens to the main window interface
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;

    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case mainClass.ID_START_GAME_BUTTON:
            if (!setUp.gameStarted) {
//Handle start game button click
                hdc = BeginPaint(hWnd, &ps);
                setUp.startGame(hWnd, hdc, ps, mainClass.mainTitle, mainClass.startButton, mainClass.quitButton, mainClass.mainWindow);
            }
            break;

        case mainClass.ID_QUIT_GAME_BUTTON:
//Handle quit game button click
            mainClass.quitGame();
            break;

        default:
            break;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
//Draw the grid and other game elements here
        if (setUp.gameStarted) {
            setUp.drawGrid(hdc);
        }
        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDOWN:
    {
//Handle click down on the grid
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        if (setUp.gameStarted) {
            mainClass.processClick(xPos, yPos);
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    }

    case WM_DESTROY:
//Handle if user presses 'x' on window
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}