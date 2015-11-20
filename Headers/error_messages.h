#include <Windows.h>
#include <d3d9.h>

bool fail_Initialize_DTD(LPDIRECT3D9 d3d, HWND hwnd);
bool fail_Create_DTDD(LPDIRECT3DDEVICE9 d3ddev, HWND hwnd);
bool fail_Load_board_surf(LPDIRECT3DSURFACE9 board_surf, HWND hwnd);
bool fail_Load_Ldisc_surf(LPDIRECT3DSURFACE9 Ldisc_surf, HWND hwnd);
bool fail_Load_Ddisc_surf(LPDIRECT3DSURFACE9 Ddisc_surf, HWND hwnd);
bool fail_Load_highlight_surf(LPDIRECT3DSURFACE9 highlight_surf, HWND hwnd);
bool fail_Load_space_surf(LPDIRECT3DSURFACE9 space_surf, HWND hwnd);
bool fail_Load_hlLdisc_surf(LPDIRECT3DSURFACE9 hlLdisc_surf, HWND hwnd);
bool fail_Load_hlDdisc_surf(LPDIRECT3DSURFACE9 hlDdisc_surf, HWND hwnd);