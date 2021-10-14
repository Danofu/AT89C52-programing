// BCD
#include <REGX52.H>
#define ESC 0x1B
#define ADD '+'

unsigned char xdata Left  _at_ 0xFE00;
unsigned char xdata Right _at_ 0xFD00;
unsigned char code Coder[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40 };

volatile unsigned char data iLeft, iRight;

void Send(unsigned char Value)
{
  P3_4 = 1;
    TI = 0;
      SBUF = Value;
      while (TI == 0) {;}
    TI = 0;
  P3_4 = 0;
}

void ISR_Button(void) interrupt 0
{
  Send(ESC);
  // Resets display
  iLeft = iRight = 0x0A;
}

void ISR_Timer1(void) interrupt 3
{
  Left  = Coder[iLeft];
  Right = Coder[iRight];
}

void ISR_Serial(void) interrupt 4
{
  if (RI == 1)
  {
    if (SBUF == '0') { iRight = iLeft = 0x00; }
    if (SBUF == ADD)
    {
      if (iRight < 0x09) { iRight++; }
      else
      {
        iRight = 0x00;
        if (iLeft < 0x09) { iLeft++; } else { iLeft = 0x00; }
      }
    }

    RI = 0;
  }
}

void Init(void)
{
  iLeft = iRight = 0x0A;
  // Listen to Serial Channel
  P3_4 = 0;
  // Timer 1
  TMOD = TMOD & 0x0F;
  TMOD = TMOD | 0x20;
  TR1  = 1; // Enable
  TH1 = TL1 = 0x00;
  // Timer 2
  T2CON = 0x30; // Reade/Write
  TL2 = RCAP2L = 0xDC;
  TH2 = RCAP2H = 0xFF;
  TR2 = 1; // Enable
  // Serial Channel
  SCON = 0x50;
  // Interrupt 0 | Int0
  IT0 = 1; // zbocze opadajace
  EX0 = 1;
  // Interrupt 3 | Timer 1
  ET1 = 1;
  // Interrupt 4 | Serial Rcv./Xmit.
  ES  = 1;
  // Enable IS
  EA  = 1;
}

void main(void)
{
  Init();
  for (;;) {;}
}