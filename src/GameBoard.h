#define BOARD_PAD  0
#define BOARD_ROWS 23
#define BOARD_COLS 12


const byte ColorMap[][3] PROGMEM = {
  {0x3f, 0x3f, 0x3f}, // 
  {0x00, 0x3f, 0x3f}, // I Cyan
  {0x3f, 0x3f, 0x00}, // O Yellow
  {0x3f, 0x00, 0x3f}, // T Purple
  {0x00, 0x3f, 0x00}, // S Green
  {0x3f, 0x00, 0x00}, // Z Red
  {0x00, 0x00, 0x3f}, // J Blue
  {0x3f, 0x1f, 0x00}, // L Orange
  {0x1f, 0x1f, 0x1f}  // Border Grey
};

#define CI_Border 0b11111000

byte GameBoard[BOARD_ROWS][BOARD_COLS];
byte GameBoardDiff[BOARD_ROWS][BOARD_COLS];

#define SetBoardPixel(R,C,Val) { GameBoard[R][C] = Val; GameBoardDiff[R][C] = 1; }

#define BORDER_T 1
#define BORDER_B 2
#define BORDER_L 4
#define BORDER_R 8

void DrawBorder(byte Side) {
 for(byte r=4;r<BOARD_ROWS;r++) {
  if(Side&BORDER_L) SetBoardPixel(r,0,CI_Border); 
  if(Side&BORDER_R) SetBoardPixel(r,BOARD_COLS-1,CI_Border);
  }
 for(byte c=0;c<BOARD_COLS;c++) {
  if(Side&BORDER_T) SetBoardPixel(0,c,CI_Border); 
  if(Side&BORDER_B) SetBoardPixel(BOARD_ROWS-1,c,CI_Border);
  }
}

char Hex[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
#define BLOCK_PIXEL_SIZE 14
void DrawBlockPixel(uint16_t R,uint16_t C,uint16_t V) {
 uint16_t Color=0;
 uint16_t Scale=V>>4;
 uint16_t Index=V&0x07;

 uint16_t Red=pgm_read_byte(&(ColorMap[Index][0]))&0x3f;
 uint16_t Grn=pgm_read_byte(&(ColorMap[Index][1]))&0x3f;
 uint16_t Blu=pgm_read_byte(&(ColorMap[Index][2]))&0x3f;

 Red=(Red*Scale)>>5;
 Grn=(Grn*Scale)>>5;
 Blu=(Blu*Scale)>>5;

 Color =Red<<11;
 Color|=Grn<<5;
 Color|=Blu<<0;

 tft.fillRect(47+C*BLOCK_PIXEL_SIZE,R*BLOCK_PIXEL_SIZE,BLOCK_PIXEL_SIZE,BLOCK_PIXEL_SIZE,Color);
}

void DrawGameBoard() {
  for (byte r = 0; r < BOARD_ROWS; r++) {
   for (byte c = 0; c < BOARD_COLS; c++) {
     if(GameBoardDiff[r][c]) {
       DrawBlockPixel(r,c,GameBoard[r][c]);
       GameBoardDiff[r][c]=0;
     }
    }
   }    
/*
  SPI.begin();
  for (int i = 0; i < 4; i++) SPI.transfer(0x00);
  for (byte c = 0; c < BOARD_COLS / 2; c++) {
    for (int i = 0; i < BOARD_PAD; i++) {
      SPI.transfer(0xff);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
    }


    for (byte r = 0; r < BOARD_ROWS; r++) {
      //if(c==11 && r<11) continue;
      byte Temp = GameBoard[(BOARD_ROWS-1)-r][(c * 2)];
      SPI.transfer(0b11100000|(Temp>>5));
      Temp&=0x07;
      SPI.transfer(pgm_read_byte(&(ColorMap[Temp][2])));
      SPI.transfer(pgm_read_byte(&(ColorMap[Temp][1])));
      SPI.transfer(pgm_read_byte(&(ColorMap[Temp][0])));
    }

    for (byte r = 0; r < BOARD_ROWS; r++) {
      byte Temp = GameBoard[(r)][(c * 2) + 1];
      //if(c==11 && r<11) continue;
      SPI.transfer(0b11100000|(Temp>>5));
      Temp&=0x07;
      SPI.transfer(pgm_read_byte(&(ColorMap[Temp][2])));
      SPI.transfer(pgm_read_byte(&(ColorMap[Temp][1])));
      SPI.transfer(pgm_read_byte(&(ColorMap[Temp][0])));
    }

  
    for (int i = 0; i < BOARD_PAD; i++) {
      SPI.transfer(0xff);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
    }
  }
  for (int i = 0; i < 10; i++) SPI.transfer(0xff);
  SPI.end();
  return;

  byte Val;
  Serial.write(0x00);
  for(byte r=0;r<BOARD_ROWS;r++) {
    for(byte c=0;c<BOARD_COLS;c++) {
      Val=GameBoard[r][c];
      if(Val) Serial.write(Val);
      else Serial.write(0b00001000);
     }
    }
    */
}

void ClearGameBoard() {
  for (byte r = 0; r < BOARD_ROWS; r++) {
    for (byte c = 0; c < BOARD_COLS; c++) {
      GameBoard[r][c] = 0;
      GameBoardDiff[r][c] = 0;
    }
  }
  tft.fillRect(0,0,240,320,0x0);
  DrawGameBoard();
}
