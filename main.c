
#include "capteur.h"
#include "lcd.h"
//#include "msp430x42x.h"
#include "msp430x22x4.h"
#include "math.h"
int main( void )
{
  int i;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  {
  // delay loop to ensure proper startup before SimpliciTI increases DCO
  // This is typically tailored to the power supply used, and in this case
  // is overkill for safety due to wide distribution.
    for(i = 0; i < 0xFFFF; i++){}
  }

  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO after random function
  DCOCTL = CALDCO_1MHZ;
  

  //all as input
  P4DIR = 0x0;
  P3DIR = 0x0;
  
  P4SEL = 0x0;
  P3SEL = 0x0;

  // RF csn active
   P3DIR |= BIT0 ; 
   P3OUT &= ~ BIT0 ; 
  
   //led active 
   //P1DIR |= BIT0;
   //P1SEL &= ~BIT0;
   
  //LCD init
  LCDInit();
  //Contrast init
  LCDContrast(0x25);
  // clear LCD
  LCD_Clear(5);
  
  
  char dT[60];
  char dL[60];
  char dH[60];
  
  while(1){
    for (char j=0;j<60;j++) {
      dT[j]=0; dH[j]=0; dL[j]=0; 
    }
    for (char j=0;j<60;j++){
   
   capteur (&dT[j], &dH[j], &dL[j]);
   
   char m[13];
   m[0] = ' ' ;
   m[1] = '0' + dT[j] / 10 ;
   m[2] = '0' + dT[j] % 10 ;
   m[3] = 'C' ;
   m[4] = ' ' ;
   m[5] = '0' + dL[j] / 10 ;
   m[6] = '0' + dL[j] % 10 ;
   m[7] = '%' ;
   m[8] = ' ' ;
   m[9] = ' ' ;
   m[10] = '0' + dH[j] / 10 ;
   m[11] = '0' + dH[j] % 10 ;
   m[12] = '%' ;

   
   // display all data: Humidity / temperature / Luminosity

   P1OUT |= BIT0; //led On
  
   LCD_IMG( IMG_ALL);   
   //LCD_TXT(" 23C 50%  77%",0,4,13);
   LCD_TXT(m,0,4,13);
   for(i = 0; i < 0xFFFF; i++)for(int k = 0; k < 0x9; k++);//delay
   
   LCD_Clear(5); 

   
   LCD_IMG( IMG_Humidity); //Humidity
   LCD_TXT("H24h 12h  0h",0,5,12);
   LCD_DrowGraf(dH,j);
   for(i = 0; i < 0xFFFF; i++)for(int k = 0; k < 0x9; k++);//delay
  
   P1OUT |= BIT0;
  
   LCD_IMG( IMG_temperature);//temperature
   LCD_TXT("T24h 12h  0h",0,5,12);
   LCD_DrowGraf(dT,j);
   for(i = 0; i < 0xFFFF; i++)for(int k = 0; k < 0x9; k++);//delay

   LCD_IMG( IMG_Luminosity );//Luminosity
   LCD_TXT("L24h 12h  0h",0,5,12);
   LCD_DrowGraf(dL,j);
   for(i = 0; i < 0xFFFF; i++)for(int k = 0; k < 0x9; k++);//delay
 
   
   
   
   
   P1OUT &= ~BIT0;//led Off
 }
  }  
  return 0;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

// ----------------------------------------------------------------------
// ISR for CCIFG1: disable further captures and return to active mode
// ----------------------------------------------------------------------
#pragma vector = TIMERA1_VECTOR
__interrupt void TIMERA1_ISR (void) // Shared ISR for CCIFG1 and TAIFG
{
  TACCTL1 = COMP_CAP | CM_0; // Disable further caps , clear flags
__low_power_mode_off_on_exit(); // Return to active mode on exit
}