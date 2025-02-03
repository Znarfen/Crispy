// compile comand: gcc -o main main.c -mwindows

#include <windows.h>
#include <math.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600
#define SCALE 10;

void logMsg(char msg[]);
void setBackground(HDC hdc, int r, int g, int b);
void drawBox(HDC hdc, int x, int y, int r, int g, int b);
void drawLine(HDC hdc, int pX, int pY, int qX, int qY, int r, int g, int b);

// Handles the window, closing the window or set pixels
int x = 0;
int y = 0;
int sx = 1;
int sy = 0;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            SetTimer(hwnd, 1, 30, NULL);  // Timer with ID=1, 30ms interval
            return 0;

        case WM_TIMER:
            if (wParam == 1) {
                x += sx;
                y += sy;

                if (x > 100) { x = 100; sx = 0; sy = -1; }
                if (y < -100) { y = -100; sx = -1; sy = 0; }
                if (x < -100) { x = -100; sx = 0; sy = 1; }
                if (y > 100) { y = 100; sx = 1; sy = 0; }

                InvalidateRect(hwnd, NULL, FALSE);  // Only redraw changed area
            }
            return 0;

        case WM_ERASEBKGND:
            return 1;  // Prevent flickering

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Clear background to white
            HBRUSH bgBrush = CreateSolidBrush(RGB(255, 255, 255));
            FillRect(hdc, &ps.rcPaint, bgBrush);
            DeleteObject(bgBrush);

            //setBackground(hdc, 0, 0, 0);
            drawBox(hdc, x, y, 0, 0, 255);
            //drawBox(hdc, x, y, 255, 255, 255);
            EndPaint(hwnd, &ps);
        }
        return 0;

        case WM_CLOSE:
            KillTimer(hwnd, 1); // Stop timer when closing
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
    Save a simple string in log.txt

    Parameters:
    - char msg[]: String to be stored in log.txt
*/
void logMsg(char msg[]) {
    FILE *file = fopen("log.txt", "a"); // Open in append mode
    if (file) {
        fprintf(file, "%s\n", msg);
        fclose(file);
    }
}

/*
    Function to set the background color of the window.
    This function loops through all pixels in the window and sets them to the specified color.

    Parameters:
    - HDC hdc: Handle to the device context
    - int r, g, b: RGB color values (0-255) for the background color
*/
void setBackground(HDC hdc, int r, int g, int b) {
    for (int y = 0; y <= HEIGHT; y++) {
        for (int x = 0; x <= WIDTH; x++){
            SetPixel(hdc, x, y, RGB(r,g,b));
        }
    }
}

/*
    Draws a "box" on the center of the screen (if x and y are 0)

    Parameters:
    - HDC hdc: Handle to the device context
    - int x, y: Coordinates of the center of the box
    - int r, g, b: RGB color values (0-255) for the box color
*/
void drawBox(HDC hdc, int x, int y, int r, int g, int b) {
    int size = 50;
    int fx = x / 2;
    int fy = y / 2;

    int p1[] = {(WIDTH+size)/2 + x, (HEIGHT+size)/2 + y};
    int p2[] = {(WIDTH-size)/2 + x, (HEIGHT+size)/2 + y};
    int p3[] = {(WIDTH+size)/2 + x, (HEIGHT-size)/2 + y};
    int p4[] = {(WIDTH-size)/2 + x, (HEIGHT-size)/2 + y};

    int q1[] = {(WIDTH+size+fx)/2 + x, (HEIGHT+size+fy)/2 + y};
    int q2[] = {(WIDTH-size+fx)/2 + x, (HEIGHT+size+fy)/2 + y};
    int q3[] = {(WIDTH+size+fx)/2 + x, (HEIGHT-size+fy)/2 + y};
    int q4[] = {(WIDTH-size+fx)/2 + x, (HEIGHT-size+fy)/2 + y};

    // Back side
    SetPixel(hdc, q1[0], q1[1], RGB(r, g, b));
    SetPixel(hdc, q2[0], q2[1], RGB(r, g, b));
    SetPixel(hdc, q3[0], q3[1], RGB(r, g, b));
    SetPixel(hdc, q4[0], q4[1], RGB(r, g, b));

    // Front side
    SetPixel(hdc, p1[0], p1[1], RGB(r, g, b));
    SetPixel(hdc, p2[0], p2[1], RGB(r, g, b));
    SetPixel(hdc, p3[0], p3[1], RGB(r, g, b));
    SetPixel(hdc, p4[0], p4[1], RGB(r, g, b));

    // Conecting lines
    
    // Back side
    drawLine(hdc, q1[0], q1[1], q2[0], q2[1], r, g, b);
    drawLine(hdc, q1[0], q1[1], q3[0], q3[1], r, g, b);
    drawLine(hdc, q2[0], q2[1], q4[0], q4[1], r, g, b);
    drawLine(hdc, q3[0], q3[1], q4[0], q4[1], r, g, b);

    // Conect front to back
    drawLine(hdc, p1[0], p1[1], q1[0], q1[1], r, g ,b);
    drawLine(hdc, p2[0], p2[1], q2[0], q2[1], r, g ,b);
    drawLine(hdc, p3[0], p3[1], q3[0], q3[1], r, g ,b);
    drawLine(hdc, p4[0], p4[1], q4[0], q4[1], r, g ,b);

    // Front side
    drawLine(hdc, p1[0], p1[1], p2[0], p2[1], r, g, b);
    drawLine(hdc, p1[0], p1[1], p3[0], p3[1], r, g, b);
    drawLine(hdc, p2[0], p2[1], p4[0], p4[1], r, g, b);
    drawLine(hdc, p3[0], p3[1], p4[0], p4[1], r, g, b);
}

/*
    Function to draw a line between two points

    Parameters:
    - HDC hdc: Handle to the device context
    - int pX, pY: x and y coordinates of the first point
    - int qX, qY: x and y coordinates of the second point
    - int r, g, b: RGB color values (0-255) for the line color
*/
void drawLine(HDC hdc, int pX, int pY, int qX, int qY, int r, int g, int b) {
    int l =  abs(abs(pX) - abs(qX)) + abs(abs(pY) - abs(qY));
    for (int g = 0; g < l; g++){
        SetPixel(hdc, 
                pX + (qX-pX) * g/l,
                pY + (qY-pY) * g/l,
                RGB(r, g, b));
    }
}







