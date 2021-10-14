// Keyboard
#include <REGX52.H>
#define DELAY_VALUE 35
#define ESC 0x1B

unsigned char bdata Key;
sbit K1 = Key^1;
sbit K2 = Key^2;
sbit K3 = Key^3;

void delay(void)
{
  unsigned char data x, y;
  for (x = 0; x < DELAY_VALUE; x++) { for (y = 0; y < DELAY_VALUE; y++) {;} }
}

void ISR_Serial(void) interrupt 4
{
  if (RI == 1)
  {
    if (SBUF == ESC)
    {
      P0_0 = 0;
      delay();
      P0_0 = 1;

      RI = 0;
    }
  }
}

unsigned char Find(unsigned char Value)
{
  unsigned char code ScanCode[] = { 0x7B, 0xE7, 0xEB, 0xED, 0xD7, 0xDB, 0xDD, 0xB7, 0xBB, 0xBD, 0x77, 0x7D };
  unsigned char data ReturnValue = 0x00;
  unsigned char data i = 0x00;
  
  while (Value != ScanCode[i]) { i++; }
  ReturnValue = '0' + i;
  if (i == 0x0A) { ReturnValue = '*'; }
  if (i == 0x0B) { ReturnValue = '#'; }
  
  return ReturnValue;
}

void Send(unsigned char Value)
{
  P3_4 = 1;
    TI = 0;
      SBUF = Value;
      while (TI == 0) {;}
    TI = 0;
  P3_4 = 0;
}

void Init(void)
{
  // Listen to Serial Channel
  P3_4 = 0;
  // Timer 2
  T2CON = 0x30; // Read/Write
  TL2 = RCAP2L = 0xDC;
  TH2 = RCAP2H = 0xFF;
  TR2 = 1; // Enable
  // Serial Channel
  SCON = 0x50;
  // Interrupt 4 | Serial Rcv./Xmit.
  ES  = 1;
  // Enable IS
  EA  = 1;
}

void main(void)
{
  unsigned char code RunningZero[] = { 0x7F, 0xBF, 0xDF, 0xEF };
  unsigned char data i = 0x00;
  bit Enable = 1;
  
  Init();
  for (;;)
  {
    P2 = RunningZero[i];
    Key = P2;
    
    if ((K1 & K2 & K3) == 0)
    {
      if (Enable == 1)
      {
        P0_0 = 0;
        Send(Find(Key));
        delay();
        P0_0 = 1;
        Enable = 0;
      }
    }
    else
    {
      if (i < 3) { i++; } else { i = 0; }
      Enable = 1;
    }
  }
}