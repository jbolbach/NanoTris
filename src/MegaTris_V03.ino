#include  <Adafruit_GFX.h>
#include <SWTFT.h>
#include <Wire.h>
#include <DS3231.h>

SWTFT tft;
DS3231 Clock;

#include "GameBoard.h"
#include "SNES.h"
#include "Font.h"
#include "Tetris.h"

#define TimeSet_Active_Color   0b111111101
#define TimeSet_InActive_Color CI_Border

void DrawClock(byte H, byte M, byte S,byte HC,byte MC,byte SC);
void DoTimeSet();
void DoGame();
bool PieceComplete();

#define InputCheck() SNES_Read()

#define STATE_TimeSet_H 0
#define STATE_TimeSet_M 1
#define STATE_TimeSet_S 2

void setup() {
  Serial.begin(115200);
  Serial.println("MegaTris 0.9");

  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
  Wire.begin();
  
  SNES_Init();
}

#define IDLE_ANI_DELAY 120
#define IDLE_STATE_StartUp  0
#define IDLE_STATE_NewPiece 1
#define IDLE_STATE_FadeIn   2
#define IDLE_STATE_FadeOut  3
#define IDLE_Piece_Count 8
#define IDLE_NextPiece_Intensity (32/IDLE_Piece_Count)

#define MOVE_SPEED 50

void DoGame() {
  randomSeed(millis());
  unsigned long DropMillis=millis();
  unsigned long RepeatMillis=millis();
  unsigned RepeatSpeed=90;
  unsigned int DropSpeed=0;
  unsigned LastInput=0;
  bool PieceComplete=false;
  byte NextPiece=random(7);
   
  NewPiece(random(7));
  DropSpeed=1000;
  ClearGameBoard();

  DrawBorder(BORDER_B|BORDER_L|BORDER_R);
  DrawPiece(NextPiece,0,7,0,0xff);
  DrawGameBoard();

  while(1) {
    unsigned Input=InputCheck();
    if(Input&SNES_ST && Input&SNES_SL) { break; }

    if(PieceComplete) {
      ErasePiece(NextPiece,0,7,0);
      DropSpeed-=(ClearComplete()*20);
      PieceComplete=false;
      if(!NewPiece(NextPiece)) break;
      NextPiece=random(7);
      DrawPiece(NextPiece,0,7,0,0xff);
    }
    
    if(Input&SNES_D) {
     DropMillis=millis(); 
    }else if(millis()-DropSpeed>=DropMillis) {      
      if(!MovePiece(MOVE_DN)) { PieceComplete=true; continue; }
      DropMillis=millis();
    }
    
    DrawGameBoard();

    if(Input && Input==LastInput) {
      if(millis()<RepeatMillis) { continue; }
      if(Input&SNES_Y || Input&SNES_B || Input&SNES_X || Input&SNES_CL || Input&SNES_CR) continue;
      LastInput=0;
    }else {
      LastInput=Input;
    }

    if(Input&SNES_L) { MovePiece(MOVE_LT); }
    if(Input&SNES_R) { MovePiece(MOVE_RT); }
    if(Input&SNES_D) { if(!MovePiece(MOVE_DN)) { PieceComplete=true; } }
    if(Input&SNES_Y || Input&SNES_CL) { RotatePiece(ROTATE_CCW); }
    if(Input&SNES_B || Input&SNES_CR) { RotatePiece(ROTATE_CW); }
    if(Input&SNES_X) { while(MovePiece(MOVE_DN)); PieceComplete=true; } 
    RepeatMillis=millis()+RepeatSpeed;
  }
  while(InputCheck()&SNES_ST || InputCheck()&SNES_SL);
}

void loop() {
  byte Idle_Piece[IDLE_Piece_Count];
  byte Idle_State[IDLE_Piece_Count];
  byte Idle_Intensity[IDLE_Piece_Count];
  byte Idle_Row[IDLE_Piece_Count];
  byte Idle_Col[IDLE_Piece_Count];
  byte Idle_Rot[IDLE_Piece_Count];
  byte H,M,S;
  int HMS=-1;
  bool Temp;
  unsigned long LastMillis=0;
  
  DrawClock(0,0,0,0,0,0);
  ClearGameBoard();
  DrawGameBoard();

  H=Clock.getHour(Temp,Temp);
  M=Clock.getMinute();
  S=Clock.getSecond();
  DrawClock(Clock.getHour(Temp,Temp),Clock.getMinute(),Clock.getSecond(),CI_Border,CI_Border,CI_Border);
  HMS=H+M+S;

  for(byte p=0;p<IDLE_Piece_Count;p++) {
    Idle_Intensity[p]=random(31); Idle_State[p]=2+random(1);
    while(1) {
      Idle_Rot[p]=random(4);
      Idle_Piece[p]=random(7);
      Idle_Row[p]=random(BOARD_ROWS-2+2);
      Idle_Col[p]=random(BOARD_COLS-2+2);
      if(!CollisionCheck(Idle_Piece[p], Idle_Row[p], Idle_Col[p], Idle_Rot[p])) break;
      }
    }

  while(1) {
    byte Input=InputCheck();
    
    if(Input&SNES_SL) { DoTimeSet(); break; }
    if(Input&SNES_ST) { DoGame(); break; }

    H=Clock.getHour(Temp,Temp);
    M=Clock.getMinute();
    S=Clock.getSecond();
    if(((int)H+(int)M+(int)S)!=HMS) {
      DrawClock(Clock.getHour(Temp,Temp),Clock.getMinute(),Clock.getSecond(),CI_Border,CI_Border,CI_Border);
      HMS=H+M+S;
    }
  
    if(millis()-LastMillis<IDLE_ANI_DELAY) continue;
    LastMillis=millis();

    for(byte p=0;p<IDLE_Piece_Count;p++) {
      switch(Idle_State[p]) {
        case(IDLE_STATE_NewPiece):
          while(1) {
            Idle_Rot[p]=random(4);
            Idle_Piece[p]=random(7);
            Idle_Row[p]=random(BOARD_ROWS-2+2);
            Idle_Col[p]=random(BOARD_COLS-2+2);
            if(!CollisionCheck(Idle_Piece[p], Idle_Row[p], Idle_Col[p], Idle_Rot[p])) break;
          }
          Idle_Intensity[p]=0;
          Idle_State[p]=IDLE_STATE_FadeIn;
        break;
        case(IDLE_STATE_FadeIn):
          Idle_Intensity[p]+=2;
          if(Idle_Intensity[p]>=32) {
            Idle_State[p]=IDLE_STATE_FadeOut;
            Idle_Intensity[p]-=2;
          }
        break;
        case(IDLE_STATE_FadeOut):
            Idle_Intensity[p]-=2;
            if(Idle_Intensity[p]>200 || Idle_Intensity==0) {
                ErasePiece(Idle_Piece[p],Idle_Row[p],Idle_Col[p],Idle_Rot[p]);
                Idle_State[p]=IDLE_STATE_NewPiece;
              }
        break;
        default:
        Idle_State[p]=IDLE_STATE_NewPiece;
      }
    if(Idle_State[p]!=IDLE_STATE_NewPiece) DrawPiece(Idle_Piece[p],Idle_Row[p],Idle_Col[p],Idle_Rot[p],Idle_Intensity[p]);
   }
   DrawGameBoard();
  }
}

void DrawClock(byte H, byte M, byte S,byte HC,byte MC,byte SC) {
  static byte DrawClock_LastH,DrawClock_LastM,DrawClock_LastS;
  DrawChar((DrawClock_LastH/10), 2,2,0x00);
  DrawChar((DrawClock_LastH%10), 2,7,0x00);
  
  DrawChar((DrawClock_LastM/10), 8,2,0x00);
  DrawChar((DrawClock_LastM%10), 8,7,0x00);

  DrawChar((DrawClock_LastS/10),14,2,0x00);
  DrawChar((DrawClock_LastS%10),14,7,0x00);

  DrawClock_LastH=H; DrawClock_LastM=M; DrawClock_LastS=S;

  DrawChar((H/10), 2,2,HC);
  DrawChar((H%10), 2,7,HC);
  
  DrawChar((M/10), 8,2,MC);
  DrawChar((M%10), 8,7,MC);

  DrawChar((S/10),14,2,SC);
  DrawChar((S%10),14,7,SC);
}

void DoTimeSet() {
  byte TimeSet_State=STATE_TimeSet_H;
  byte NewH=12,NewM=0,NewS=0;
  byte LastInput=0;
  bool Temp;
  
  NewH=Clock.getHour(Temp,Temp);
  NewM=Clock.getMinute();
  NewS=Clock.getSecond();

  ClearGameBoard();
  DrawGameBoard();

  while(1) {
    byte Input=InputCheck();
    if(Input && Input==LastInput) {
      while((millis()%400)<200);
      LastInput=0;
    }else {
      LastInput=Input;
    }
    if(Input&SNES_L) { TimeSet_State--; if(TimeSet_State>2) TimeSet_State=2; }
    if(Input&SNES_R) { TimeSet_State++; if(TimeSet_State>2) TimeSet_State=0; }
    if(Input&SNES_U) {
      switch(TimeSet_State) {
        case(STATE_TimeSet_H):
          NewH++;
          if(NewH>23) NewH=0;
        break;
        case(STATE_TimeSet_M):
          NewM++;
          if(NewM>59) NewM=0;
        break;
        case(STATE_TimeSet_S):
          NewS++;
          if(NewS>59) NewS=0;
        break;
      }
    }
    if(Input&SNES_D) {
      switch(TimeSet_State) {
        case(STATE_TimeSet_H):
          NewH--;
          if(NewH>23) NewH=23;
        break;
          case(STATE_TimeSet_M):
          NewM--;
          if(NewM>59) NewM=59;
        break;
        case(STATE_TimeSet_S):
          NewS--;
          if(NewS>59) NewS=59;
        break;
      }
    }
    if(Input&SNES_ST) {
      Clock.setHour(NewH);
      Clock.setMinute(NewM);
      Clock.setSecond(NewS);
      break;
    }
    if(millis()%1000>500) { DrawClock(NewH,NewM,NewS,TimeSet_State==STATE_TimeSet_H?TimeSet_Active_Color:TimeSet_InActive_Color,TimeSet_State==STATE_TimeSet_M?TimeSet_Active_Color:TimeSet_InActive_Color,TimeSet_State==STATE_TimeSet_S?TimeSet_Active_Color:TimeSet_InActive_Color); }
    else { DrawClock(NewH,NewM,NewS,TimeSet_InActive_Color,TimeSet_InActive_Color,TimeSet_InActive_Color); }
    DrawGameBoard();
  }  
}
