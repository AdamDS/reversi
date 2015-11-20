#include "Board.h"

Board::Board() {
  MOVE = 0; //track number of moves
  PASS = 0; //last pass move played
  //initialize board spaces
  //board fills count from top left to bottom right, just like screen pixels
  init_board();
  POSY = 0;
  POSX = 0;
}

Board::Board(int posy, int posx) {
  //for screen positioning at (posx,posy)
  MOVE = 0; //track number of moves
  PASS = 0;
  //initialize board spaces
  //board fills count from top left to bottom right, just like screen pixels
  init_board();
  POSY = posy;
  POSX = posx;
}

void Board::init_board() {
  /*const int BOARDH = 8;
  const int BOARDW = 8;*/
  MOVE = 0;
  PASS = 0;
  for (int i=0; i<BOARDH; i++) { 
    for (int j=0; j<BOARDW; j++) { 
      fills[i][j] = 0;
      //set starting pieces at center         //j ->
      if (i==3) {                             //0  1  2  3  4  5  6  7
        if (j==3) fills[i][j] = 1;            //0  0  0  0  0  0  0  0,  0  i
        if (j==4) fills[i][j] = 2;            //0  0  0  0  0  0  0  0,  1  |
        if (j==2 || j==5) fills[i][j] = -1;   //0  0 -1 -1 -1 -1  0  0,  2  V
      } else if (i==4) {                      //0  0 -1  1  2 -1  0  0,  3
        if (j==3) fills[i][j] = 2;            //0  0 -1  2  1 -1  0  0,  4
        if (j==4) fills[i][j] = 1;            //0  0 -1 -1 -1 -1  0  0,  5
        if (j==2 || j==5) fills[i][j] = -1;   //0  0  0  0  0  0  0  0,  6
      } else if (i==2 || i==5) {              //0  0  0  0  0  0  0  0,  7
        if (j>1 && j<6) fills[i][j] = -1;
      } /*else {
        fills[i][j] = 0;
      }*/
    } 
  }
}

void Board::board_reset() {
  init_board();
}

const int Board::get_BOARDH() { return BOARDH; }
const int Board::get_BOARDW() { return BOARDW; }
int Board::get_space(int y, int x) { return fills[y][x]; }
int Board::get_MOVE() { return MOVE; }

int Board::sum_type(int type) {
  int sum = 0;
  int thisSpace;
  for (int i=0; i<get_BOARDH(); i++) 
    for (int j=0; j<get_BOARDW(); j++) {
      thisSpace = get_space(i, j);
      if (thisSpace==type) sum++;
    }
  return sum;
}

int Board::get_search_spaces(int y, int x, int order) {
  //(x,y) is space coordinate to search about
  //order is minx(0), maxx(1), miny(2), or maxy(3)
  int a, b, c, d;
  //set ranges of spaces to check for valid edges
  if (y==0) { //along top
    if (x==0) { //top left
      a = x;  b = x+1;  c = y;  d = y+1;
    } else if (x==8) { //top right
      a = x-1;  b = x;  c = y; d = y+1;
    } else { //top not a corner
      a = x-1;  b = x+1;  c = y;  d = y+1;
    }
  } else if (y==8) { //along bottom
    if (x==0) { //bottom left
      a = x;  b = x+1;  c = y-1;  d = y;
    } else if (x==8) { //bottom right
      a = x-1;  b = x;  c = y-1;  d = y;
    } else { //bottom not a corner
      a = x-1;  b = x+1;  c = y-1;  d = y;
    }
  } else if (y>0 && y<8) { //not along top or bottom
    if (x==0) { //left not a corner
      a = x;  b = x+1;  c = y-1;  d = y+1;
    } else if (x==8) { //right not a corner
      a = x-1;  b = x;  c = y-1;  d = y+1;
    } else { //somewhere in middle
      a = x-1;  b = x+1;  c = y-1;  d = y+1;
    }
  }
  
  if (order==0) return a;
  else if (order==1) return b;
  else if (order==2) return c;
  else if (order==3) return d;
  else return 0;
}

int Board::get_lookfor(int type) {
  int lookfor;
  //set what to look for
  if (type==1) { //if light piece
    return lookfor = 2; //look for dark piece
  } else { //if dark piece
    return lookfor = 1; //look for light piece
  }
}

int Board::get_disc_type() { 
  int type = MOVE%2;
  if (type==0) type = 2;
  else type = 1;
  return type; 
}

bool Board::check_for_valid_move(int type) {
  //searches whole board for a valid play
  bool valid = false; //return value for move
  int ss[4]; //search space ranges
  if (sum_type(0)>0) { //if any edges
    //check if move with disc type at (x,y) is valid, don't actually make move
    for (int i=0; i<BOARDH; i++) {
      for (int j=0; j<BOARDW; j++) {
        if (!valid) { //search board until valid possible move found
          if (fills[i][j]==-1) { //only search about edges
            for (int k = 0; k<4; k++)  ss[k] = get_search_spaces(i, j, k);
            valid = check_move(ss, i, j, type); 
          }
        }
      }
    }
  }

  return valid;
}

bool Board::edge_exist() {
  bool valid = false; //assume no moves
  for (int i=0; i<BOARDH; i++) {
    for (int j=0; j<BOARDW; j++) {
       if (fills[i][j]==-1) valid = true;
    }
  }
  return valid;
}

bool Board::check_move(int *ss, int y, int x, int type) {
  //searches in ss range about (x,y) for opposite type to check move

  bool valid = false; //assume play is invalid
  bool search; //for while, keep searching?
  int minx = ss[0]; int maxx = ss[1]; int miny = ss[2]; int maxy = ss[3];
  int dy, dx; //direction to check
  int lookfor = get_lookfor(type); //what piece to look for
  bool possible = false; //good search start?, assume not

  for (int i=miny; i<=maxy; i++) { //cycle rows
    for (int j=minx; j<=maxx; j++) { //cycle columns
      if (!valid) { //keep cycling if no valid move
        if (i==y && j==x) { //skip space of new piece
        } else {
          if (fills[i][j]==lookfor) { //found an opposing piece
            dy = i-y; dx = j-x; //set direction to find end of set
            int m = i;  int n = j; //start search from current opposing piece
            search = true; //ok to start search
            /* search in line along direction (dx,dy) */
            while (search) { //find end of set to flip
              if (m<0 || m>BOARDH || n<0 || n>BOARDW) { //if off board
                search = false; //stop search, off board
                valid = false; //not valid, never found like piece
              } else if (fills[m][n]==lookfor) { //found another opposing piece
                m = m+dy; n = n+dx; //increment one space along search direction
                possible = true;
              } else if (fills[m][n]==type && possible) { //end (always false on first step)
                search = false; //stop searching in this direction
                valid = true; //move is valid
              } else if (fills[m][n]==0 || fills[m][n]==-1) { //empty or edge
                search = false;
              }
            }
          }
        }
      }
    }
  }
  return valid;
}

bool Board::make_move(int y, int x) {
  bool valid = false; //assume play not allowed
  int ss[4];
  for (int i = 0; i<4; i++)  ss[i] = get_search_spaces(y, x, i); //search range
  int type = get_disc_type(); //type of piece being played
  bool search; //for while, keep searching?
  int dy, dx; //direction to check
  int lookfor = get_lookfor(type); //what piece to look for
  bool possible = false;
  int q, p; //move along flip line
  int flip; //how many to flip

  if (fills[y][x]==0 || fills[y][x]==-1) {
    for (int i=ss[2]; i<=ss[3]; i++) { //cycle rows
      for (int j=ss[0]; j<=ss[1]; j++) { //cycle columns
        if (i==y && j==x) { //skip space of new piece
        } else { //potential play space
          if (fills[i][j]==lookfor) { //found an opposing piece
            dy = i-y; dx = j-x; //set direction to find end of set
            int m = i;  int n = j; //start search from current opposing piece
            search = true; //ok to start search
            flip = 0;
            /* search in line along direction (dx,dy) */
            while (search) { //find end of set to flip
              if (m<0 || m>=BOARDH || n<0 || n>=BOARDW) { //if off board
                search = false; //stop search, off board
                //valid = false; //not valid, never found like piece
              } else if (fills[m][n]==lookfor) { //found another opposing piece
                m = m+dy; n = n+dx; //increment one space along search direction
                flip++; //increase flip counter
                possible = true;
              } else if (fills[m][n]==type && possible) { //end (always false on first step)
                p = m;  q = n; //start from end piece
                flip++; //flip this many & set new piece
                for (int d=0; d<flip; d++) { 
                  p = p-dy; q = q-dx; //backtrack along flip line
                  update_board(p, q, type);
                }
                search = false; //stop searching in this direction
                valid = true; //move is valid
              } else if (fills[m][n]==0 || fills[m][n]==-1) { //empty or edge
                flip = 0;
                search = false;
              } 
            } //while (search)
          } //if (fills[i][j]==lookfor)
        } //else potential play space
      } //for j
    } //for i
  } //if a space or edge

  if (valid) MOVE++; 
  return valid;
}

void Board::update_board(int y, int x, int type) {
  //Make move
  fills[y][x] = type;
}

bool Board::pass_play() {
  bool stop_game = false; //return this, used as gameover in Reversi.cpp
  int previous = MOVE-1; //previous move count
  int type = get_disc_type(); //disc type to be played
  bool can_move = false;

  //search board for valid move
  for (int i=0; i<BOARDH; i++) {
    for (int j=0; j<BOARDW; j++) {
      if (!can_move) { //if can't move, keep checking
        can_move = check_for_valid_move(type); //see if you can
      } //if can move, then no further need to check
    }
  }

  //if can't move & previous move was a pass, then send to toggle gameover
  if (!can_move && previous==PASS) {
    stop_game = true;
  } else if (!can_move) { 
    PASS = MOVE;
    MOVE++;
  } //if can move, then do nothing
  
  return stop_game;
}

Board::~Board() {
//  for (int i=0; i<BOARDH; i++) delete[] fills[i];
//  delete[] fills;
}