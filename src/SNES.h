#define SNES_CLK_PIN 45
#define SNES_LAT_PIN 47
#define SNES_DAT_PIN 49
#define SNES_DELAY   28

#define SNES_B  0x0001
#define SNES_Y  0x0002
#define SNES_SL 0x0004
#define SNES_ST 0x0008

#define SNES_U  0x0010
#define SNES_D  0x0020
#define SNES_L  0x0040
#define SNES_R  0x0080

#define SNES_A  0x0100
#define SNES_X  0x0200
#define SNES_CL 0x0400
#define SNES_CR 0x0800

void SNES_Init() {
 digitalWrite(SNES_LAT_PIN,LOW);
 digitalWrite(SNES_CLK_PIN,LOW);
 digitalWrite(SNES_DAT_PIN,LOW);

 pinMode(SNES_LAT_PIN,OUTPUT);
 pinMode(SNES_CLK_PIN,OUTPUT);
 pinMode(SNES_DAT_PIN,INPUT);
}

unsigned int SNES_Read() {
  digitalWrite(SNES_CLK_PIN,HIGH);
  digitalWrite(SNES_LAT_PIN,HIGH);
  delayMicroseconds(SNES_DELAY);
  digitalWrite(SNES_LAT_PIN,LOW);
  
  unsigned int Result=0x0000;
  
  for(byte i=0;i<16;i++) {
   digitalWrite(SNES_CLK_PIN,HIGH);
   delayMicroseconds(SNES_DELAY/2);
   Result>>=1; Result|=((!digitalRead(SNES_DAT_PIN))<<15); 
   digitalWrite(SNES_CLK_PIN,LOW);
   delayMicroseconds(SNES_DELAY/2);
  }

 return Result;
}
