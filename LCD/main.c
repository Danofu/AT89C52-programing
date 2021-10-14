#include <REGX52.H>
#define ESC 0x1B
#define DELAY_VALUE 40
#define LCD_CLEAR 0x01

extern void LcdInit();
extern void Lcd_Cursor(char row, char column);
extern void Lcd_DisplayCharacter(unsigned char a_char);
extern void Lcd_DisplayString(char row, char column, char *string);
extern void Lcd_DisplayScreen(char *ptr);
extern void Lcd_WriteControl(unsigned char LcdCommand);
extern void Lcd_WriteData(unsigned char LcdData);
extern void AddCustomCharacter(unsigned char Character[], unsigned char CGRAMAddress);

unsigned char data Var1, Var2, Var3;
unsigned char code WelcomeScreen[] = "Sest. Wbudowane "
                                     "----------------"
                                     " Daniil Borysov "
                                     "----------------";
unsigned char code InfoScreen[]    = "  Informacja o  "
                                     "    Autorze:    "
                                     "Nr.Albumu: 35623"
                                     "Grupa:      ID-1";

unsigned char code BackSlash[] = { 0x00, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00 };
unsigned char code I[]         = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00 };
unsigned char code II[]        = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 };
unsigned char code III[]       = { 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00 };
unsigned char code IIII[]      = { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x00 };

unsigned char data Revolutions;
unsigned char data j, k;

void delay(void)
{
  unsigned char data x, y;
  for (x = 0; x < DELAY_VALUE; x++) { for (y = 0; y < DELAY_VALUE; y++) {;} }
}

void ISR_Serial(void) interrupt 4
{
  if(RI == 1)
  {
    if (SBUF == ESC) { Lcd_DisplayScreen(WelcomeScreen); }
    if (SBUF == '1') { Lcd_DisplayScreen(InfoScreen); }
    
    if (SBUF == '2')
    {
      Lcd_WriteControl(LCD_CLEAR);
      Lcd_WriteControl(0x0C);
      
      Lcd_DisplayString(1, 0, " Poczekaj       ");
      Revolutions = 0x00;
      while (Revolutions < 8)
      {
        Lcd_DisplayString(1, 0, " Poczekaj.      ");
        Lcd_Cursor(1, 13);
        Lcd_DisplayCharacter('|');
        delay();
        
        Lcd_DisplayString(1, 0, " Poczekaj..     ");
        Lcd_Cursor(1, 13);
        Lcd_DisplayCharacter('/');
        delay();
        
        Lcd_DisplayString(1, 0, " Poczekaj...    ");
        Lcd_Cursor(1, 13);
        Lcd_DisplayCharacter('-');
        delay();
        
        Lcd_DisplayString(1, 0, " Poczekaj       ");
                Lcd_Cursor(1, 13);
        Lcd_DisplayCharacter(0x00); // back slash
        delay();
        
        Revolutions++;
      }
      
      Lcd_WriteControl(LCD_CLEAR);
      Lcd_WriteControl(0x0C);
      Lcd_DisplayString(1, 4, "Gotowe !");
      Lcd_Cursor(2, 7);
      Lcd_DisplayCharacter(0xBC);
    }
    
    if (SBUF == '3')
    {
      Lcd_WriteControl(LCD_CLEAR);
      Lcd_WriteControl(0x0C);
      
      Lcd_DisplayString(1, 3, "Odbieram:");
      
      for (j = 0; j < 16; j++)
      {
        for (k = 0; k < 5; k++)
        {
          Lcd_Cursor(2, j);
          
          if (k != 4) { Lcd_DisplayCharacter(k + 1); }
          else { Lcd_DisplayCharacter(0xFF); }
          
          delay();
        }
      }
      
      Lcd_DisplayString(1, 3, "Odebrano !");
    }
    
    RI = 0;
  }
}

void Init(void)
{
  // Listen to Serial Channel
  P3_4 = 0;
  // Timer 2
  T2CON = 0x20; // Read Only
  TL2 = RCAP2L = 0xDC;
  TH2 = RCAP2H = 0xFF;
  // Serial Channel
  SCON = 0x50;
  // Interrupt 4 | Serial Rcv./Xmit.
  ES  = 1;
  // Enable IS
  EA  = 1;
  
  TR2 = 1; // Enable
}

void InitCustomCharacters(void)
{
  AddCustomCharacter(BackSlash, 0x40);
  AddCustomCharacter(I,    0x48);
  AddCustomCharacter(II,   0x50);
  AddCustomCharacter(III,  0x58);
  AddCustomCharacter(IIII, 0x60);
}

void main(void)
{
  Init();
  LcdInit();
  InitCustomCharacters();
  Lcd_DisplayScreen(WelcomeScreen);
  while(1) {;}
}