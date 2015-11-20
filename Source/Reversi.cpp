#include "Game_DirectX.h"
#include "Game_Window.h"
#include "Board.h"
#include "error_messages.h"
using namespace std;

//intialize game variables
Board board; 

//initialize screen stuff
const string APPTITLE = "ADS: Reversi";
int SCREENW = 480;
int SCREENH = 480;
int LEFTMARGIN = 15;
int TOPMARGIN = 15;
int SCOREW = 100;
int SPACEW = (SCREENW-SCOREW)/(board.get_BOARDW()); //fit 8 spaces in 480px
int SPACEH = (SCREENH/(board.get_BOARDH())); //fit 8 spaces in 480px
int SCORECOL = SPACEW*8+3*LEFTMARGIN;
int SCOREH = SCREENH;
int LSCOREY = SPACEH*3+TOPMARGIN;
int DSCOREY = SPACEH*4+TOPMARGIN;
int FONTH = 25;

//reversi game data
int selectedx = 0;
int selectedy = 0;
int Lscore, Dscore, empties, edges;
int highlightx = 0;
int highlighty = 0;
double dt;
clock_t current, last_update;
double WAIT_TIME = 150; //


//initialize surfaces for drawing
LPDIRECT3DSURFACE9 Ldisc_surf = NULL;
LPDIRECT3DSURFACE9 Ddisc_surf = NULL;
LPDIRECT3DSURFACE9 highlight_surf = NULL;
LPDIRECT3DSURFACE9 space_surf = NULL;
LPDIRECT3DSURFACE9 hlLdisc_surf = NULL;
LPDIRECT3DSURFACE9 hlDdisc_surf = NULL;

//LPD3DXSPRITE spriteobj = NULL;
//LPD3DXFONT fontArial24 = NULL;
//p149-?

/* Game initialization function */
bool Game_Init(HWND hwnd) {
  //initialize Direct3D, DirectInput, & font
  Direct3D_Init(hwnd, SCREENW, SCREENH, false);
  DirectInput_Init(hwnd);
  //fontArial24 = MakeFont("Arial", 24);

  //initialize game variables
  Lscore = board.sum_type(1);
  Dscore = board.sum_type(2);
  empties = board.sum_type(0);
  edges = board.sum_type(-1);
  last_update = clock();

  //load game surfaces
  /*board_surf = LoadSurface("board.png");
  fail_Load_board_surf(board_surf, hwnd);*/
  space_surf = LoadSurface("space.png");
  fail_Load_space_surf(space_surf, hwnd);
  Ldisc_surf = LoadSurface("Ldisc.png");
  fail_Load_Ldisc_surf(Ldisc_surf, hwnd);
  Ddisc_surf = LoadSurface("Ddisc.png");
  fail_Load_Ddisc_surf(Ddisc_surf, hwnd);
  highlight_surf = LoadSurface("highlight.png");
  fail_Load_highlight_surf(highlight_surf, hwnd);
  hlLdisc_surf = LoadSurface("hlLdisc.png");
  fail_Load_hlLdisc_surf(hlLdisc_surf, hwnd);
  hlDdisc_surf = LoadSurface("hlDdisc.png");
  fail_Load_hlDdisc_surf(hlDdisc_surf, hwnd);
  
  //get the back buffer surface
  d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

  //position the board
  //board.POSX = SCREENW/2;
  //board.POSY = SCREENH/2;

  return true;
}

/* Game update function */
void Game_Run(HWND hwnd) {
  //make sure the Direct3D device is valid
  if (!d3ddev) return;

  //update input devices
  DirectInput_Update();

  //check for game end
  edges = board.sum_type(-1);
  empties = board.sum_type(0);
  if (!empties) gameover = true; //if no edges
  if (!gameover) {
    current = clock();
    //change selected space with keyboard
    dt = (double)(current -last_update);
    if (dt>WAIT_TIME) { //wait for 0.5s after a move before looking for updates
      //move selected space
      if (Key_Down(DIK_LEFT)) {
        selectedx = (selectedx-1)%board.get_BOARDW();
        last_update = clock();
      }
      if (Key_Down(DIK_RIGHT)) {
        selectedx = (selectedx+1)%board.get_BOARDW();
        last_update = clock();
      }
      if (Key_Down(DIK_DOWN)) {
        selectedy = (selectedy+1)%board.get_BOARDH();
        last_update = clock();
      }
      if (Key_Down(DIK_UP)) {
        selectedy = (selectedy-1)%board.get_BOARDH();
        last_update = clock();
      }

      //right shift to pass
      if (Key_Down(DIK_RSHIFT)) {
        gameover = board.pass_play();
        last_update = clock();
      }
  
      //enter to place piece, but check if valid move
      if (Key_Down(DIK_RCONTROL)) {
        int type = board.get_disc_type();
        bool success = board.make_move(selectedy, selectedx);
        //if (success) { //if valid move
        //  MessageBox(hwnd, "Nice move!", "Good choice", MB_OK);
        //} else if (!success) {
        //  MessageBox(hwnd, "Not a valid move", "Bad choice", MB_OK);
        //}
        last_update = clock();
      }

      //reset
      if (Key_Down(DIK_BACK)) { board.board_reset(); }

      //escape to exit
      if (Key_Down(DIK_ESCAPE)) { gameover = true; }
    }//if (dt>WAIT_TIME)
  } //if !gameover

  //clear the backbuffer to black
  d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

  //start rendering
  if (d3ddev->BeginScene()) {
    //update highlighted space
    highlightx = (selectedx*SPACEW)+LEFTMARGIN;
    highlighty = (selectedy*SPACEH)+TOPMARGIN; 
    //draw green board
    d3ddev->ColorFill(backbuffer, NULL, D3DCOLOR_XRGB(0,0,0));
    //DrawSpaceSurface(backbuffer, highlightx, highlighty, highlight_surf);
    int type, y, x;
    for (int i=0; i<board.get_BOARDH(); i++) {
      for (int j=0; j<board.get_BOARDW(); j++) {
        type = board.get_space(i, j);
        y = i*SPACEH+TOPMARGIN;
        x = j*SPACEW+LEFTMARGIN;
        if (highlighty==y && highlightx==x) { 
          if (type==0 || type==-1) {
            DrawSpaceSurface(backbuffer, 
              highlighty, highlightx, highlight_surf);
          } else if (type==1) {
            DrawSpaceSurface(backbuffer, 
              highlighty, highlightx, hlLdisc_surf);
          } else if (type==2) {
            DrawSpaceSurface(backbuffer, 
              highlighty, highlightx, hlDdisc_surf);
          }
        } else {
          if (type==1) DrawSpaceSurface(backbuffer, 
            y, x, Ldisc_surf);
          if (type==2) DrawSpaceSurface(backbuffer, 
            y, x, Ddisc_surf);
          if (type==0 || type==-1) DrawSpaceSurface(backbuffer, 
            y, x, space_surf);
        } //each space
      } //for j
    } //for i
    int turn_indicator;
    if (board.get_disc_type()==1) { //if L
      turn_indicator = LSCOREY;
    } else { //if D
      turn_indicator = DSCOREY;
    }
    DrawSpaceSurface(backbuffer, turn_indicator, SCORECOL, 
        highlight_surf);

    //score
    fontspriteobj->Begin(D3DXSPRITE_ALPHABLEND);
    stringstream L, D;
    Lscore = board.sum_type(1);
    L << Lscore;
    FontPrint(fontArial24, SCORECOL, LSCOREY, L.str(), 
      D3DCOLOR_XRGB(0,0,255));
    Dscore = board.sum_type(2);
    D << Dscore;
    FontPrint(fontArial24, SCORECOL, DSCOREY, D.str(), 
      D3DCOLOR_XRGB(255,0,0));
    fontspriteobj->End();

    //stop rendering
    d3ddev->EndScene();
    //copy back buffer on the screen
    d3ddev->Present(NULL, NULL, NULL, NULL);
  }

  //check for escape key to exit program
  if (Key_Down(DIK_ESCAPE))
    gameover = true;
  if (gameover) {
    Lscore = board.sum_type(1);
    Dscore = board.sum_type(2);
    string result;
    if (Lscore>Dscore) {
      result = "Blue wins!";
    } else if (Lscore<Dscore) {
      result = "Red wins!";
    } else {
      result = "Draw";
    }
    MessageBox(hwnd, result.c_str(), "Final Score", MB_OK);
    board.board_reset();
    gameover = false;
  }
}

void Game_End() {
  if (fontArial24) fontArial24->Release();
  DirectInput_Shutdown();
  Direct3D_Shutdown();
}