#ifndef BOARD_H
#define BOARD_H
#pragma once

#include <cmath>
using namespace std;

#define BOARDH 8
#define BOARDW 8

class Board {
public:
  Board();
  Board(int posy, int posx);
  ~Board();
  //accessors
  const int get_BOARDH();
  const int get_BOARDW();
  int get_space(int y, int x);
  int get_MOVE();
  int get_lookfor(int type);
  int get_search_spaces(int y, int x, int order);
  int sum_type(int type);
  int get_disc_type();
  //mutators
  void board_reset();
  bool check_for_valid_move(int type);
  bool check_move(int*ss, int y, int x, int type);
  bool make_move(int y, int x);
  void update_board(int y, int x, int type);
  bool pass_play();

private: 
  //int **make_temp();
  void init_board();
  bool edge_exist();
  
  /*int BOARDH;
  int BOARDW;*/
  int fills[8][8];
  int MOVE;
  int POSX;
  int POSY;
  int PASS;
};

#endif