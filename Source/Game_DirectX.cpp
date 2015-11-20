#include "Game_DirectX.h"

//Direct3D objects
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;

//Direct3D fonts
LPD3DXSPRITE fontspriteobj = NULL;
LPD3DXFONT fontArial24 = NULL;

//DirectInput variables
LPDIRECTINPUT8 dinput = NULL;
//LPDIRECTINPUTDEVICE8 dimouse = NULL;
LPDIRECTINPUTDEVICE8 dikeyboard = NULL;
//DIMOUSESTATE mouse_state;
char keys[256];
//XINPUT_GAMEPAD controllers[4];

/* Direct3D initialization function */
bool Direct3D_Init(HWND hwnd, int width, int height, bool fullscreen) {
  //initialize Direct3D
  d3d = Direct3DCreate9(D3D_SDK_VERSION);
  fail_Initialize_DTD(d3d, hwnd);

  //set Direct3D presentation parameters
  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory(&d3dpp, sizeof(d3dpp));
  d3dpp.Windowed = (!fullscreen); //start as SCREENW x SCREENH
  d3dpp.SwapEffect = D3DSWAPEFFECT_COPY; //poor alt D3DSWAPEFFECT_DISCARD
  d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
  d3dpp.BackBufferCount = 1;
  d3dpp.BackBufferWidth = width;
  d3dpp.BackBufferHeight = height;
  d3dpp.hDeviceWindow = hwnd;

  //create Direct3D device
  d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, 
    D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
  fail_Create_DTDD(d3ddev, hwnd);

  //get a pointer to the back buffer surface
  d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
  
  //set font
  fontArial24 = MakeFont("Arial", 24);
  D3DXCreateSprite(d3ddev, &fontspriteobj);

  return true;
}

/* Direct3D shutdown */
void Direct3D_Shutdown() {
  if (d3ddev) d3ddev->Release();
  if (d3d) d3d->Release();
}

/* Draws a surface to the screen using StretchRect */
//void DrawBoardSurface(LPDIRECT3DSURFACE9 dest, Board board, 
//  LPDIRECT3DSURFACE9 source) {
//    //get width/height from source surface
//    D3DSURFACE_DESC desc;
//    source->GetDesc(&desc);
//
//    //create rects for drawing
//    RECT source_rect = {0, 0, 
//      (long)desc.Width, (long)desc.Height };
//    RECT dest_rect = { 0, 0, 
//      (long)desc.Width, (long)desc.Height };
//
//    //draw the source surface onto the dest
//    d3ddev->StretchRect(source, &source_rect, 
//      dest, &dest_rect, D3DTEXF_NONE);
//
//    //int X, Y;
//    //LPDIRECT3DSURFACE9 L, D;
//    //for (int i; i<sizeof(board.BOARDH); i++) {
//    //  for (int j; j<sizeof(board.BOARDW); j++) {
//    //    if (FILLS[i][j]==1) {
//    //      X = j; //disc location is a function of place on board
//    //      Y = i; 
//    //      DrawDiscSurface(dest, X, Y, L);
//    //    } else if (FILLS[i][j]==2) {
//    //      X = j; //disc location is a function of place on board
//    //      Y = i; 
//    //      DrawDiscSurface(dest, X, Y, D);
//    //    }
//    //  }
//    //}
//}

void DrawSpaceSurface(LPDIRECT3DSURFACE9 dest, 
  int y, int x, LPDIRECT3DSURFACE9 source) {
    D3DSURFACE_DESC desc;
    source->GetDesc(&desc);

    RECT source_rect = {0, 0, (long)desc.Width, (long)desc.Height};
    RECT dest_rect = {(long)x, (long)y, 
      (long)x+(desc.Width), (long)y+(desc.Height)};
    d3ddev->StretchRect(source, &source_rect, dest, &dest_rect, D3DTEXF_NONE);
}

/* Loads a bitmap file into a surface */
LPDIRECT3DSURFACE9 LoadSurface(string filename) {
  LPDIRECT3DSURFACE9 image = NULL;

  //get width and height from bitmap file
  D3DXIMAGE_INFO info;
  HRESULT result = D3DXGetImageInfoFromFile(filename.c_str(), &info);
  if (result!=D3D_OK) return NULL;

  //create surface
  result = d3ddev->CreateOffscreenPlainSurface(
    info.Width, //width of surface
    info.Height, //height of surface
    D3DFMT_X8R8G8B8, //surface format
    D3DPOOL_DEFAULT, //memory pool to use
    &image, //pointer to the surface
    NULL); //reserved (always NULL)

  if (result!=D3D_OK) return NULL;

  //load surface from file into newly created surface
  result = D3DXLoadSurfaceFromFile( 
    image, //destination surface
    NULL, //destination palette
    NULL, //destination rectangle
    filename.c_str(), //source filename
    NULL, //source rectangle
    D3DX_DEFAULT, //controls how image is filtered
    D3DCOLOR_XRGB(0,0,0), //for transparency (0 for none)
    NULL); //source image info (usually NULL)

  //make sure file was loaded okay
  if (result!=D3D_OK) return NULL;

  return image;
}

/* Font */
LPD3DXFONT MakeFont(string name, int size) {
  LPD3DXFONT font = NULL;
  D3DXFONT_DESC desc = {
    size, //height
    0, //width
    0, //weight
    0, //miplevels
    false, //italic
    DEFAULT_CHARSET, //charset
    OUT_TT_PRECIS, //output precision
    CLIP_DEFAULT_PRECIS, //quality
    DEFAULT_PITCH, //pitch and family
    "" //font name
  };
  strcpy_s(desc.FaceName, name.c_str());
  D3DXCreateFontIndirect(d3ddev, &desc, &font);
  return font;
}

void FontPrint(LPD3DXFONT font, int x, int y, string text, 
  D3DCOLOR color) {
    //figure out the text boundary
    RECT rect = {x, y, 0, 0};
    font->DrawText(NULL, text.c_str(), text.length(), &rect, 
      DT_CALCRECT, color);

    //print the text
    font->DrawText(fontspriteobj, text.c_str(), text.length(), &rect, 
      DT_LEFT, color);
}

void Direct3DFont_Shutdown() {
  if (fontArial24) fontArial24->Release();
  if (fontspriteobj) fontspriteobj->Release();
}

/* DirectInput initialization */
bool DirectInput_Init(HWND hwnd) {
  //initialize DirectInput object
  HRESULT result = DirectInput8Create(
    GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, 
    (void**)&dinput, NULL);

  //initialize the keyboard
  dinput->CreateDevice(GUID_SysKeyboard, &dikeyboard, NULL);
  dikeyboard->SetDataFormat(&c_dfDIKeyboard);
  dikeyboard->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE);// | DISCL_FOREGROUND);
  dikeyboard->Acquire();

  //initialize the mouse
  /*dinput->CreateDevice(GUID_SysMouse, &dimouse, NULL);
  dimouse->SetDataFormat(&c_dfDIMouse);
  dimouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
  dimouse->Acquire();
  d3ddev->ShowCursor(true);*/

  return true;
}

/* DirectInput update */
void DirectInput_Update() {
  //update keyboard
  dikeyboard->GetDeviceState(sizeof(keys), (LPVOID)&keys);

  //update mouse
  //dimouse->GetDeviceState(sizeof(mouse_state), (LPVOID)&mouse_state);

  //update controllers
  //for (int i=0; i<4; i++) {
  //  ZeroMemory(&controllers[i], sizeof(XINPUT_STATE));

  //  //get the state of the controller
  //  XINPUT_STATE state;
  //  DWORD result = XInputGetState(i, &state);

  //  //store state in global controllers array
  //  if (!result) controllers[i] = state.Gamepad;
  //}
}

///* Return mouse x movement */
//int Mouse_X() {
//  return mouse_state.lX;
//}
//
///* Return mouse y movement */
//int Mouse_Y() {
//  return mouse_state.lY;
//}
//
///* Return mouse button state */
//int Mouse_Button(int button) {
//  return mouse_state.rgbButtons[button] & 0x80;
//}

/* Return key press state */
int Key_Down(int key) {
  return (keys[key] & 0x80);
}

/* DirectInput shutdown */
void DirectInput_Shutdown() {
  if (dikeyboard) {
    dikeyboard->Unacquire();
    dikeyboard->Release();
    dikeyboard = NULL;
  }
  /*if (dimouse) {
    dimouse->Unacquire();
    dimouse->Release();
    dimouse = NULL;
  }*/
}


///* Returns true if controller is plugged in */
//bool XInput_Controller_Found() {
//  XINPUT_CAPABILITIES caps;
//  ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
//  XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps);
//  if (caps.Type!=0) return false;
//
//  return true;
//}

///* Vibrates the controller */
//void XInput_Vibrate(int contNum, int amount) {
//  XINPUT_VIBRATION vibration;
//  ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
//  vibration.wLeftMotorSpeed = amount;
//  vibration.wRightMotorSpeed = amount;
//  XInputSetState(contNum, &vibration);
//}