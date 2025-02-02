// compile comand: gcc -o main main.c -mwindows

#include <windows.h>

#define WIDTH 800
#define HEIGHT 600

struct box{
    int x, y, z;
    int width, height;
    int fov; // field of view for perspective
};

void setBackground(HDC hdc, int r, int g, int b);
void addBox(HDC hdc, int x, int y);

// Handles the window, closing the window or set pixels
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            setBackground(hdc, 90, 0, 0);

            addBox(hdc, 100, 100);
            addBox(hdc, -100, 100);
            addBox(hdc, 100, -100);
            addBox(hdc, -100, -100);

            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Define window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "SimpleWindowClass";

    RegisterClass(&wc);

    // Create window
    HWND hwnd = CreateWindowEx(0, "SimpleWindowClass", "My Window", 
                               WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                               WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

/*
    Function to set the background color of the window.
    This function loops through all pixels in the window and sets them to the specified color.

    Parameters:
    - HDC hdc: Handle to the device context
    - int r, g, b: RGB color values (0-255) for the background color
*/
void setBackground(HDC hdc, int r, int g, int b) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++){
            SetPixel(hdc, x, y, RGB(r,g,b));
        }
    }
}

void addBox(HDC hdc, int x, int y) {
    int size = 50;

    // Back side
    SetPixel(hdc, (WIDTH+size+x)/2 + x, (HEIGHT+size+y)/2 + y, RGB(255, 0, 255));
    SetPixel(hdc, (WIDTH-size+x)/2 + x, (HEIGHT+size+y)/2 + y, RGB(255, 0, 255));
    SetPixel(hdc, (WIDTH+size+x)/2 + x, (HEIGHT-size+y)/2 + y, RGB(255, 0, 255));
    SetPixel(hdc, (WIDTH-size+x)/2 + x, (HEIGHT-size+y)/2 + y, RGB(255, 0, 255));

    // Front side
    SetPixel(hdc, (WIDTH+size)/2 + x, (HEIGHT+size)/2 + y, RGB(0, 255, 255));
    SetPixel(hdc, (WIDTH-size)/2 + x, (HEIGHT+size)/2 + y, RGB(0, 255, 255));
    SetPixel(hdc, (WIDTH+size)/2 + x, (HEIGHT-size)/2 + y, RGB(0, 255, 255));
    SetPixel(hdc, (WIDTH-size)/2 + x, (HEIGHT-size)/2 + y, RGB(0, 255, 255));
}
