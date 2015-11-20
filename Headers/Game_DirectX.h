#pragma once

//header files
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x800
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <Xinput.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <WinBase.h>
#include <sstream>
#include <iostream>
#include "Board.h"
#include "error_messages.h"
using namespace std;

//libraries
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"Kernel32.lib")

//program values
extern const string APPTITLE;
extern int SCREENW;
extern int SCREENH;
extern bool gameover;

//Direct3D objects
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;
extern LPD3DXSPRITE fontspriteobj;
extern LPD3DXFONT fontArial24;

//Direct3D functions
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen);
void Direct3D_Shutdown();
LPDIRECT3DSURFACE9 LoadSurface(string filename);
void DrawSpaceSurface(LPDIRECT3DSURFACE9 dest, 
  int y, int x, LPDIRECT3DSURFACE9 source);

//font functions
LPD3DXFONT MakeFont(string name, int size);
void FontPrint(LPD3DXFONT font, int x, int y, string text, 
  D3DCOLOR color);
void Direct3DFont_Shutdown();

//DirectInput objects, devices, and states
extern LPDIRECTINPUT8 dinput;
//extern LPDIRECTINPUTDEVICE8 dimouse;
extern LPDIRECTINPUTDEVICE8 dikeyboard;
//extern DIMOUSESTATE mouse_state;
//extern XINPUT_GAMEPAD controllers[4];

//DirectInput functions
bool DirectInput_Init(HWND);
void DirectInput_Update();
void DirectInput_Shutdown();
int Key_Down(int);
//int Mouse_Button(int);
//int Mouse_X();
//int Moust_Y();
//void XInput_Vibrate(int contNum = 0, int amount = 65535);
//bool XInput_Controller_Found();

//game functions
bool Game_Init(HWND hwnd);
void Game_Run(HWND hwnd);
void Game_End();