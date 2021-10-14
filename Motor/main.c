// Motor
#include <REGX52.H>
#define ESC 0x1B
#define ADD '+'
#define IN1 P2_1
#define IN2 P2_2
#define IN3 P2_5
#define IN4 P2_6
#define EN1 P2_3
#define EN2 P2_4
#define MAX 0xFF
#define REVOLUTIONS 0x0A // 10

volatile unsigned char Slider, Counter, Setting;

void Send(unsigned char Value)
{
  P3_4 = 1;
    TI = 0;
      SBUF = Value;
      while (TI == 0) {;}
    TI = 0;
  P3_4 = 0;
}

void ISR_Motor(void) interrupt 0
{
  if (Counter < REVOLUTIONS)
  {
    Counter++;
    Send(ADD);
  }
  else
  {
    Counter = 0x00;
    Setting = 0x00;
    EX0 = 0;
  }
}

void ISR_Timer1(void) interrupt 3
{
  if (Setting != 0x00)
  {
    if (Setting != MAX)
    {
      if (Slider < Setting) { EN1 = EN2 = 1; } else { EN1 = EN2 = 0; }
      if (Slider < MAX) { Slider++; } else { Slider = 0x00; }
    }
    else { EN1 = EN2 = 1; }
  }
  else { EN1 = EN2 = 0; }
}

void ISR_Serial(void) interrupt 4
{
  if (RI == 1)
  {
    // Resets motors
    if (SBUF == ESC)
    {
      EX0 = 0;
      Counter = 0x00;
      Setting = 0x00;
      IN1 = IN3 = 1;
      IN2 = IN4 = 0;
    }
    
    // Changing speed of motors
    if (SBUF == '4') { Setting = 0x00; } // motors are stopped
    if (SBUF == '5') { Setting = 0x33; } // 20% of max speed
    if (SBUF == '6') { Setting = 0x66; } // 40% of max speed
    if (SBUF == '7') { Setting = 0x99; } // 60% of max speed
    if (SBUF == '8') { Setting = 0xCC; } // 80% of max speed
    if (SBUF == '9') { Setting = MAX ; } // 100% speed
    
    // Changing the rotation direct of the top motor
    if (SBUF == '*')
    {
      if (IN1 == 1)
      {
        IN1 = 0;
        IN2 = 1;
      }
      else
      {
        IN1 = 1;
        IN2 = 0;
      }
    }
    
    // Changing the rotation direct of the bottom motor
    if (SBUF == '#')
    {
      if (IN3 == 1)
      {
        IN3 = 0;
        IN4 = 1;
      }
      else
      {
        IN3 = 1;
        IN4 = 0;
      }
    }
    
    // Enable Interrupt 0
    // Countdown timer mod
    // Motors stop, when reach #REVOLUTIONS revolutions
    if (SBUF == '0') { EX0 = 1; }
    
    RI = 0;
  }
}

void Init(void)
{
  Counter = 0x00;
  Slider  = 0x00;
  Setting = 0x00;
  // Top Motor
  IN1 = 1; // Turning right
  IN2 = 0;
  EN1 = 0; // Disabled
  // Bottom Motor
  IN3 = 1; // Turning right
  IN4 = 0;
  EN2 = 0; // Disabled
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
  IT0 = 1;
  EX0 = 0;
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