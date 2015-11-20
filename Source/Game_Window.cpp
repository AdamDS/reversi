#include "Game_DirectX.h"
#include "error_messages.h"
using namespace std;

bool gameover = false;

/* Windows event handling function */
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_DESTROY:
    gameover = true;
    PostQuitMessage(0);
    break;
  }
  return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* Main Windows entry function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  //set the new window's properties
  WNDCLASSEX wc;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = (WNDPROC)WinProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = APPTITLE.c_str();
  wc.hIconSm = NULL;
  RegisterClassEx(&wc);

  //create a new window
  HWND hwnd = CreateWindow(APPTITLE.c_str(), APPTITLE.c_str(), 
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
    SCREENW, SCREENH, NULL, NULL, hInstance, NULL);

  //was there an error creating the window?
  if (!hwnd) return 0;

  //display the window
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  //initialize the game
  if (!Game_Init(hwnd)) return 0;

  //main message loop
  MSG message;
  while (!gameover) {
    if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    Game_Run(hwnd);
  }

  Game_End();
  return message.wParam;
}