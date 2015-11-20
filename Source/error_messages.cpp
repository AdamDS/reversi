#include "error_messages.h"

bool fail_Initialize_DTD(LPDIRECT3D9 d3d, HWND hwnd) {
  if (!d3d) { //MESSAGE: fail_initialize_DTD
    MessageBox(hwnd, "Error initializing Direct3D", "Error", MB_OK);
    return false;
  }
  return true;
}


bool fail_Create_DTDD(LPDIRECT3DDEVICE9 d3ddev, HWND hwnd) {
  if (!d3ddev) { //MESSAGE: fail_Create_DTDD
    MessageBox(hwnd, "Error creating Direct3D device", "Error", MB_OK);
    return false;
  }
  return true;
}

bool fail_Load_board_surf(LPDIRECT3DSURFACE9 board_surf, HWND hwnd) {
  if (!board_surf) {
    MessageBox(hwnd, "Error loading board.png", "Error", MB_OK);
    return false;
  }
  return true;
}

bool fail_Load_Ldisc_surf(LPDIRECT3DSURFACE9 Ldisc_surf, HWND hwnd) {
  if (!Ldisc_surf) {
    MessageBox(hwnd, "Error loading Ldisc.png", "Error", MB_OK);
    return false;
  }
  return true;
}

bool fail_Load_Ddisc_surf(LPDIRECT3DSURFACE9 Ddisc_surf, HWND hwnd) {
  if (!Ddisc_surf) {
    MessageBox(hwnd, "Error loading Ddisc.png", "Error", MB_OK);
    return false;
  }
  return true;
}

bool fail_Load_highlight_surf(LPDIRECT3DSURFACE9 highlight_surf, HWND hwnd) {
  if (!highlight_surf) {
    MessageBox(hwnd, "Error loading highlight.png", "Error", MB_OK);
    return false;
  }
  return true;
}

bool fail_Load_space_surf(LPDIRECT3DSURFACE9 space_surf, HWND hwnd) {
  if (!space_surf) {
    MessageBox(hwnd, "Error loading space.png", "Error", MB_OK);
    return false;
  }
  return true;
}

bool fail_Load_hlLdisc_surf(LPDIRECT3DSURFACE9 hlLdisc_surf, HWND hwnd) {
  if (!hlLdisc_surf) {
    MessageBox(hwnd, "Error loading hlLdisc.png", "Error", MB_OK);
    return false;
  }
  return true;
}

bool fail_Load_hlDdisc_surf(LPDIRECT3DSURFACE9 hlDdisc_surf, HWND hwnd) {
  if (!hlDdisc_surf) {
    MessageBox(hwnd, "Error loading hlDdisc.png", "Error", MB_OK);
    return false;
  }
  return true;
}