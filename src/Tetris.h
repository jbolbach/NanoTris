#include "TetrisPieces.h"

byte Piece=0xff;
int  Piece_Row=0,Piece_Col=0;
byte Piece_Rot=0;
byte Game_State=0;

#define TETRIS_ROWS 24
#define TETRIS_COLS 10
#define TETRIS_START_COL ((BOARD_COLS-TETRIS_COLS)/2)
#define TETRIS_PIECE_START_COL ((BOARD_COLS/2)-(TETRIS_COLS/2))

void DrawPiece(byte Piece, int R, int C, byte Rot,byte Intensity) {
  for (int PC = 0; PC < 4; PC++) {
    for (int PR = 0; PR < 4; PR++) {
      byte Val = pgm_read_byte(&(Pieces[Piece][Rot][(PR << 2) + PC]));
      if(Val) Val|=Intensity<<3;
      if (!Val) continue;
      if ((PR + R) >= BOARD_ROWS) break;
      if ((PC + C) >= BOARD_COLS) break;
      if ((PR + R) < 0) break;
      if ((PC + C) < 0) break;
      SetBoardPixel(R+PR,C+PC,Val); 
    }
  }
}

void ErasePiece(byte Piece, int R, int C, byte Rot) {
  for (int PC = 0; PC < 4; PC++) {
    for (int PR = 0; PR < 4; PR++) {
      if ((PR + R) >= BOARD_ROWS) break;
      if ((PC + C) >= BOARD_COLS) break;
      if ((PR + R) < 0) break;
      if ((PC + C) < 0) break;
      byte Val = pgm_read_byte(&(Pieces[Piece][Rot][(PR << 2) + PC]));
      if (Val > 0) SetBoardPixel(R+PR,C+PC,0x00); 
    }
  }
}

#define DebugOut(s,b) //{ Serial.print(s); Serial.println(b); }

byte CollisionCheck(byte Piece, int R, int C, byte Rot) {
 DebugOut("CollisionCheck:Start",0);
  byte Result = 0;
  for (int PR = 0; PR < 4 && !Result; PR++) {
    for (int PC = 0; PC < 4 && !Result; PC++) {
      byte Val = pgm_read_byte(&(Pieces[Piece][Rot][(PR << 2) + PC]));
      if (!Val) continue;
      if ((PR + R) >= BOARD_ROWS) { DebugOut("R>BR",0); Result = 1; break; }
      if ((PC + C) >= BOARD_COLS) { DebugOut("C>BC",0); Result = 1; break; }
      if ((PR + R) < 1) { DebugOut("R<0",0); Result = 1; break; }
      if ((PC + C) < 1) { DebugOut("C<0",0); Result = 1; break; }
      if(GameBoard[R + PR][C + PC]) { DebugOut("OCC",0); Result = 1; break; }
    }
  }
  DebugOut("CollisionCheck:End",0);
  return Result;
}

byte RowComplete(int R) {
 byte Complete=1;
 for(int i=0;i<TETRIS_COLS && Complete;i++) if(!GameBoard[R][i+TETRIS_START_COL]) Complete=0; 
 return Complete;
}

#define ROTATE_CW  1
#define ROTATE_CCW 0
#define TestRot(R,X,Y) (!CollisionCheck(Piece,Piece_Row-(Y), Piece_Col+(X), (R)))

#define ROT_01 0
#define ROT_10 1
#define ROT_12 2
#define ROT_21 3
#define ROT_23 4
#define ROT_32 5
#define ROT_30 6
#define ROT_03 7

const byte KickTable_JLSTZ[8][5] PROGMEM = {
  {0x22,0x12,0x13,0x20,0x10},
  {0x22,0x32,0x31,0x24,0x34},
  {0x22,0x32,0x31,0x24,0x34},
  {0x22,0x12,0x13,0x20,0x10},
  {0x22,0x32,0x33,0x20,0x30},
  {0x22,0x12,0x11,0x24,0x14},
  {0x22,0x12,0x11,0x24,0x14},
  {0x22,0x32,0x33,0x20,0x30}
};

const byte KickTable_I[8][5] PROGMEM = {
  {0x22,0x02,0x32,0x01,0x34},
  {0x22,0x32,0x12,0x43,0x10},
  {0x22,0x12,0x42,0x14,0x41},
  {0x22,0x32,0x02,0x30,0x03},
  {0x22,0x42,0x12,0x43,0x10},
  {0x22,0x02,0x32,0x01,0x34},
  {0x22,0x32,0x02,0x30,0x03},
  {0x22,0x12,0x42,0x14,0x41}
};

void RotatePiece(byte Direction) { 
  if(Piece==PIECE_O) return;
  
  ErasePiece(Piece,Piece_Row,Piece_Col,Piece_Rot);

  if(Piece>6) return;
  byte New_Rot=Piece_Rot;
  byte DirIndex=0,TestIndex=0;
  if(Direction==ROTATE_CW)  {
    switch(Piece_Rot) {
      case(0): New_Rot=1; DirIndex=ROT_01; break;
      case(1): New_Rot=2; DirIndex=ROT_12; break;
      case(2): New_Rot=3; DirIndex=ROT_23; break;
      case(3): New_Rot=0; DirIndex=ROT_30; break;
    }
  }
  if(Direction==ROTATE_CCW)  {
    switch(Piece_Rot) {
      case(0): New_Rot=3; DirIndex=ROT_03; break;
      case(1): New_Rot=0; DirIndex=ROT_10; break;
      case(2): New_Rot=1; DirIndex=ROT_21; break;
      case(3): New_Rot=2; DirIndex=ROT_32; break;
    }
  }
  
  byte Temp;
  int X,Y;
  for(TestIndex=0;TestIndex<5;TestIndex++) {
   if(Piece==PIECE_I) Temp=pgm_read_byte(&(KickTable_I[DirIndex][TestIndex]));
   else Temp=pgm_read_byte(&(KickTable_JLSTZ[DirIndex][TestIndex]));
  
   X=-2; X+=(int)(Temp>>4);
   Y=-2; Y+=(int)(Temp&0x0f);
   if(!CollisionCheck(Piece,Piece_Row+Y, Piece_Col+X, New_Rot)) {
    Piece_Rot=New_Rot; 
    Piece_Row+=Y;
    Piece_Col+=X;
    break;
   }
  }
 DrawPiece(Piece,Piece_Row,Piece_Col,Piece_Rot,0xff);
}

#define MOVE_UP 0
#define MOVE_DN 1
#define MOVE_LT 2
#define MOVE_RT 3

bool MovePiece(byte Direction) {
  bool Result=true;
  ErasePiece(Piece,Piece_Row,Piece_Col,Piece_Rot);
  switch(Direction) {
    case(MOVE_UP):
      if(!CollisionCheck(Piece,Piece_Row-1, Piece_Col, Piece_Rot)) Piece_Row-=1;
      else Result=false;
    break;
    case(MOVE_DN):
      if(!CollisionCheck(Piece,Piece_Row+1, Piece_Col, Piece_Rot)) Piece_Row+=1;
      else Result=false;
    break;
    case(MOVE_LT):
      if(!CollisionCheck(Piece,Piece_Row, Piece_Col-1, Piece_Rot)) Piece_Col-=1;
      else Result=false;
    break;
    case(MOVE_RT):
      if(!CollisionCheck(Piece,Piece_Row, Piece_Col+1, Piece_Rot)) Piece_Col+=1;
      else Result=false;
    break;
  }
  DrawPiece(Piece,Piece_Row,Piece_Col,Piece_Rot,0xff);
  return Result;
}

bool NewPiece(byte P=0xff) {
 Piece=P;
 if(P>6) Piece=random(7);
 Piece_Row=3;
 Piece_Col=4;
 Piece_Rot=0;
 bool Result = CollisionCheck(Piece,Piece_Row,Piece_Col,Piece_Rot);
 DrawPiece(Piece,Piece_Row,Piece_Col,Piece_Rot,0xff);
 return !Result;
}

byte RowsComplete[BOARD_ROWS];

#define CLEAR_FADE_DELAY  25
#define CLEAR_SLIDE_DELAY 25
#define CLEAR_WIPE_DELAY  50

void ClearComplete_Fade()
{
 for(int i=30;i>=0;i-=2) {
  for(byte r=0;r<BOARD_ROWS;r++) {
    if(!RowsComplete[r]) continue;
    for(byte c=0;c<TETRIS_COLS;c++) {
     if(i>0) { SetBoardPixel(r,c+TETRIS_PIECE_START_COL,(GameBoard[r][c+TETRIS_PIECE_START_COL]&0x07)|(i<<3)); } //GameBoard[r][c+TETRIS_PIECE_START_COL]=(GameBoard[r][c+TETRIS_PIECE_START_COL]&0x07)|(i<<3);
    }
  }
  DrawGameBoard();
  delay(CLEAR_FADE_DELAY);
 }
}

void ClearComplete_Slide()
{
 for(byte i=0;i<TETRIS_COLS;i++) {
    byte Count=0;
    for(byte r=0;r<BOARD_ROWS;r++) {
      if(!RowsComplete[r]) continue;
      Count++;
      for(byte c=0;c<TETRIS_COLS;c++) {
        if(Count&0x01) { SetBoardPixel(r,TETRIS_PIECE_START_COL+c,GameBoard[r][TETRIS_PIECE_START_COL+c+1]); }
        else SetBoardPixel(r,TETRIS_PIECE_START_COL+TETRIS_COLS-1-c,GameBoard[r][TETRIS_PIECE_START_COL+TETRIS_COLS-2-c]);
        }
      if(Count&0x01) { SetBoardPixel(r,TETRIS_COLS+TETRIS_PIECE_START_COL-1,0); }
      else SetBoardPixel(r,TETRIS_PIECE_START_COL,0);
      }
     DrawGameBoard();
     delay(CLEAR_SLIDE_DELAY);
    }
}

void ClearComplete_Wipe()
{
 for(byte i=0;i<TETRIS_COLS/2;i++) {
    for(byte r=0;r<BOARD_ROWS;r++) {
      if(!RowsComplete[r]) continue;
      for(byte c=0;c<TETRIS_COLS/2;c++) {
         SetBoardPixel(r,TETRIS_PIECE_START_COL+c,GameBoard[r][TETRIS_PIECE_START_COL+c+1]);
         SetBoardPixel(r,TETRIS_PIECE_START_COL+TETRIS_COLS-1-c,GameBoard[r][TETRIS_PIECE_START_COL+TETRIS_COLS-2-c]);
        }
       SetBoardPixel(r,TETRIS_PIECE_START_COL+(TETRIS_COLS/2)-1,0);
       SetBoardPixel(r,TETRIS_PIECE_START_COL+(TETRIS_COLS/2)-0,0);
      }
     DrawGameBoard();
     delay(CLEAR_WIPE_DELAY);
    }
}

#define COLLAPSE_DELAY 0

void CollapseComplete()
{
 for(byte i=1;i<BOARD_ROWS-1;i++) {
  if(RowsComplete[i]) {
    for(byte r=i;r>1;r--)  {
      for(byte c=0;c<TETRIS_COLS;c++) {
        SetBoardPixel(r,TETRIS_START_COL+c,GameBoard[r-1][TETRIS_START_COL+c]);
      }
    }
   DrawGameBoard();
   delay(COLLAPSE_DELAY);
  }
 } 
}

byte ClearComplete()
{
 byte CompleteCount=0;
 for(byte r=0;r<BOARD_ROWS-1;r++) {
  if(RowComplete(r)) { 
    CompleteCount++;
    RowsComplete[r]=1;
  }else {
    RowsComplete[r]=0;
  }
 }
  if(CompleteCount) {
    switch(random(3)) {
      case(0): ClearComplete_Fade(); break;
      case(1): ClearComplete_Slide(); break;
      case(2): ClearComplete_Wipe(); break;
    }
    CollapseComplete();
  }
  return CompleteCount;
}
