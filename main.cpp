#include <windows.h>

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

HWND g_hEdit; // Global variable to hold the handle to the edit control
HBRUSH g_hEditBackgroundBrush;
const int GRID_SIZE = 8;
const int CELL_SIZE = 100;

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {
    // Register the window class
    const char* CLASS_NAME = "myWin32WindowClass";
    WNDCLASS wc{};
    wc.hInstance = currentInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpfnWndProc = WindowProcessMessages;
    RegisterClass(&wc);

    int windowWidth = GRID_SIZE * CELL_SIZE;
    int windowHeight = GRID_SIZE * CELL_SIZE;

    // Create the window
    HWND hwnd = CreateWindow(CLASS_NAME, "Win32 Tutorial",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,         // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,             // Window initial position
        windowWidth, windowHeight,                // Window size
        nullptr, nullptr, currentInstance, nullptr);

    // Window loop
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Set the pen color and style for the grid lines
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HGDIOBJ hOldPen = SelectObject(hdc, hPen);

        // Draw the vertical grid lines
        for (int x = 0; x <= GRID_SIZE; ++x) {
            int xPos = x * CELL_SIZE;
            MoveToEx(hdc, xPos, 0, nullptr);
            LineTo(hdc, xPos, GRID_SIZE * CELL_SIZE);
        }

        // Draw the horizontal grid lines
        for (int y = 0; y <= GRID_SIZE; ++y) {
            int yPos = y * CELL_SIZE;
            MoveToEx(hdc, 0, yPos, nullptr);
            LineTo(hdc, GRID_SIZE * CELL_SIZE, yPos);
        }

        // Restore the old pen and release the current pen
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, param, lparam);
    }
    return 0;
}
