const unsigned char font[96][3] PROGMEM = {
  {0x3e,0x22,0x3e}, // 0
  {0x24,0x3e,0x20}, // 1
  {0x3a,0x2a,0x2e}, // 2
  {0x22,0x2a,0x3e}, // 3
  {0x0e,0x08,0x3e}, // 4
  {0x2e,0x2a,0x3a}, // 5
  {0x3e,0x2a,0x3a}, // 6
  {0x02,0x3a,0x06}, // 7
  {0x3e,0x2a,0x3e}, // 8
  {0x2e,0x2a,0x3e}  // 9
};

void DrawChar(byte CharIndex,int R,int C,byte Color) {
  for (int PC = 0; PC < 3; PC++) {
    byte BitMap=pgm_read_byte(&(font[CharIndex][PC]));
    for (int PR = 0; PR < 7; PR++) {
      if ((PR + R) >= BOARD_ROWS) break;
      if ((PC + C) >= BOARD_COLS) break;
      if ((PR + R) < 0) break;
      if ((PC + C) < 0) break;
      if(BitMap&0x01){ SetBoardPixel(R+PR,C+PC,Color); }
      BitMap>>=1;
    }
  }
}
