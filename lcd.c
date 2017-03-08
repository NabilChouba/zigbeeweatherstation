//lcd_nokia.h
#include "lcd.h"
#include "msp430x22x4.h"

//static int   LcdCacheIdx;
//static int   LoWaterMark;
//static int   HiWaterMark;




void nokia_gotoxy(unsigned char xnokia, unsigned char ynokia)      // Nokia LCD 3310 Position cursor
   {
     // y can be 0 .. 5 (step by 8bit block)
     // x can be 0 ..83 
      LCDSend(0x40|(ynokia&0x07),0);   // Y axe initialisation: 0100 0yyy
      LCDSend(0x80|(xnokia&0x7f),0);   // X axe initialisation: 1xxx xxxx
   }

// represent lcd matrix
unsigned char  LcdCache [ LCD_CACHE_SIZE ];

void LCD_IMG( int c) {
  int i;
  nokia_gotoxy(0,0);
  for (i=0;i<504;i++){
    if (c == IMG_ALL )
      LCDSend(ImageAll[i],1);
   // if (c == IMG_Humidity )
    //  LCDSend(ImageH[i],1);
   // if (c == IMG_temperature )
    //  LCDSend(ImageT[i],1);
    if (c == IMG_Luminosity )
      LCDSend(ImageLux[i],1);
  }
    
}

void LCD_Clear(int d)
	{
		int i;
		 for (i=0;i<504;i++)
                  LCDSend(0x00,1);                  
                  
		//delay_ms(d);
	}

//draw graf
int tabGraf[9]={0,BIT7,BIT7|BIT6,BIT7|BIT6|BIT5,BIT7|BIT6|BIT5|BIT4,BIT7|BIT6|BIT5|BIT4|BIT3,BIT7|BIT6|BIT5|BIT4|BIT3|BIT2,BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1,BIT7|BIT6|BIT5|BIT4|BIT3|BIT2|BIT1|BIT0};

//allow you to draw a simple line
//int tabGraf[9]={0,BIT7,BIT6,BIT5,BIT4,BIT3,BIT2,BIT1,BIT0};

//from colPixel we will draw the graf :
//go by block and draw the colone if needed
int translate(char colPixel,char y){
  int n = colPixel - (4-y) *8;
  if ( n<0 )
    return (0x00);
  if (n>8)
    return (0xFF);
   //return (0x00);//allow you to draw a simple line
  else 
    return tabGraf[n];
}


void LCD_DrowGraf(char d[],char lengh)
	{
		int i,j;
                for (j=0;j<5;j++){
                // graf is composed from 60 points  
		nokia_gotoxy(0,j);
                for (i=0;i<lengh;i++)
                  LCDSend(translate(d[i]/2,j),1); // (/2) 0-100 0-50 we have only 48 pixel
                
                }
		//delay_ms(d);
	}


void table_to_nokialcd(char charsel)               // extract ascii from tables & write to LCD
   {
     int charpos;
     char chardata;
     
      if (charsel<0x20)return;            // 5 bytes
      if (charsel>0x7f)return;
      for (int char_row=0;char_row<5;char_row++)
         {
            if (charsel<0x50)
               {
                  charpos=(((charsel&0xff)-0x20)*5);
                  chardata=TABLE5[(charpos+char_row)];
               }                           // use TABLE5
            else if (charsel>0x4f)
               {
                  charpos=(((charsel&0xff)-0x50)*5);
                  chardata=TABLE6[(charpos+char_row)];
               }                           // use TABLE6
            LCDSend(chardata,1);    // send data to nokia
           }
      LCDSend(0X00,1);               // 1 byte (always blank)
   }

void LCD_TXT(char a[30],unsigned char x,unsigned char y,int d)
	{
		nokia_gotoxy(x,y);
                for(int i = 0 ;i<d ;i++)
		  table_to_nokialcd(a[i]);
		//delay_ms(d);
	}



   
// simple delay
void Delay(unsigned long a) { 
  for(int j = 0; j < 0x01; j++)
  for(int i = a; i!=0; i--);
//  while (--a!=0); 
  
}

/****************************************************************************/
/*  Set LCD Contrast                                                        */
/*  Function : LcdContrast                                                  */
/*      Parameters                                                          */
/*          Input   :  contrast                                             */
/*          Output  :  Nothing                                              */
/****************************************************************************/
/*void LCDChr(unsigned char size, unsigned char ch ) {

  unsigned char i, c;
    unsigned char b1, b2;
    int  tmpIdx;

    if ( LcdCacheIdx < LoWaterMark )
    {
        //  Update low marker.
        LoWaterMark = LcdCacheIdx;
    }

    // if size 1X
    if ( size == FONT_1X )
    {
        for ( i = 0; i < 5; i++ )
        {
            LcdCache[LcdCacheIdx++] = FontLookup[ch - 32][i] << 1;
        }
    }
    // if size 2X
    else if ( size == FONT_2X )
    {
        tmpIdx = LcdCacheIdx - 84;

        if ( tmpIdx < LoWaterMark )
        {
            LoWaterMark = tmpIdx;
        }

        if ( tmpIdx < 0 ) return;

        for ( i = 0; i < 5; i++ )
        {
            c = FontLookup[ch - 32][i] << 1;
            b1 =  (c & 0x01) * 3;
            b1 |= (c & 0x02) * 6;
            b1 |= (c & 0x04) * 12;
            b1 |= (c & 0x08) * 24;

            c >>= 4;
            b2 =  (c & 0x01) * 3;
            b2 |= (c & 0x02) * 6;
            b2 |= (c & 0x04) * 12;
            b2 |= (c & 0x08) * 24;

            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx++] = b1;
            LcdCache[tmpIdx + 82] = b2;
            LcdCache[tmpIdx + 83] = b2;
        }

        //  Update x cursor position.
        LcdCacheIdx += 11;
    }


    if ( LcdCacheIdx > HiWaterMark )
    {
        //  Update high marker.
        HiWaterMark = LcdCacheIdx;
    }

    //  Horizontal gap between characters.
    LcdCache[LcdCacheIdx++] = 0x00;
}
*/
/****************************************************************************/
/*  Clear LCD                                                               */
/*  Function : LCDClear                                                     */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/****************************************************************************/
/*void LCDClear(void) {

    int i;

    // loop all cashe array
    for ( i = 0; i < LCD_CACHE_SIZE; i++ )
    {
        LcdCache[i] = 0x00;
    }

    //  Reset watermark pointers.
    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;
}
*/
/****************************************************************************/
/*  Set LCD Contrast                                                        */
/*  Function : LcdContrast                                                  */
/*      Parameters                                                          */
/*          Input   :  contrast                                             */
/*          Output  :  Nothing                                              */
/****************************************************************************/
/*void LCDUpdate ( void )
{
    int i;

    if ( LoWaterMark < 0 )
        LoWaterMark = 0;
    else if ( LoWaterMark >= LCD_CACHE_SIZE )
        LoWaterMark = LCD_CACHE_SIZE - 1;

    if ( HiWaterMark < 0 )
        HiWaterMark = 0;
    else if ( HiWaterMark >= LCD_CACHE_SIZE )
        HiWaterMark = LCD_CACHE_SIZE - 1;

    //  Set base address according to LoWaterMark.
    LCDSend((0x80 | (LoWaterMark % LCD_X_RES)), SEND_CMD );
    LCDSend((0x40 | (LoWaterMark / LCD_X_RES)), SEND_CMD );

    //  Serialize the video buffer.
    for ( i = LoWaterMark; i <= HiWaterMark; i++ )
    {
      LCDSend( LcdCache[i], SEND_CHR );
    }

    //  Reset watermark pointers.
    LoWaterMark = 0;
    HiWaterMark = LCD_CACHE_SIZE - 1;

}*/

/****************************************************************************/
/*  Send to LCD                                                             */
/*  Function : LCDSend                                                      */
/*      Parameters                                                          */
/*          Input   :  data and  SEND_CHR or SEND_CMD                       */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDSend(unsigned char data, unsigned char cd) {

  
  // Enable display controller (active low).
  P4OUT &= ~ BIT4 ; // STE0 (CE)

  // command or data
  if(cd == SEND_CHR) {
      P4OUT |= BIT6 ;  // SOMI0 (D/S)
  }
  else {
     P4OUT &= ~BIT6 ;  // SOMI0 (D/S)
  }

  ///// SEND SPI /////

  //send data
  //U0TXBUF = data;
  UCB0TXBUF = data;

  //Wait for ready U0TXBUF
  //while((U0TCTL & TXEPT) == 0);
  while (!(IFG2 & UCB0TXIFG));  

  // Disable display controller.
  P4OUT |= BIT4 ; // STE0 (CE)
 Delay(3);
}

/****************************************************************************/
/*  Init LCD Controler                                                      */
/*  Function : LCDInit                                                      */
/*      Parameters                                                          */
/*          Input   :  Nothing                                              */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDInit(void)
{

  //  Pull-up on reset psin.
  P4OUT |= BIT5;

  // Pin configuration - all as output
  P4DIR |= BIT4 ; // STE0
  P3DIR |= BIT1 ; // SIMO0
  P4DIR |= BIT6 ; // SOMI0 (D/S)
  P3DIR |= BIT3 ; // ULCK0
  P4DIR |= BIT5 ; // RES

  // Pin select function
  P4SEL &= ~ BIT4 ; // STE0
  P3SEL |= BIT1 ; // SIMO0
  P4SEL &= ~ BIT6 ; // SOMI0 (D/S)
  P3SEL |= BIT3 ; // ULCK0
  P4SEL &= ~ BIT5 ; // RES
  


  //  Toggle display reset pin.
  P4OUT &=~ BIT5;
  Delay(10000);
  P4OUT |= BIT5;

  // Init SPI 
  /*
  U0CTL   = 0x16;   // SPI Mode, 8bit, Master mode
  U0TCTL  = 0xB2;   // 3pin Mode, clock->SMCLK, no CKPL (poliarity), no CKPH (phase)
  U0BR0   = 0x15;     // 38000 -> 38000 = ~800Khz/21
  U0BR1   = 0x00;
  UMCTL0  = 0x00;   // in spi mode don't used
  ME1     = 0x40;   // Enable SPI0
  */

  // Init SPI
  UCB0CTL1 = UCSWRST;                       // **Put state machine in reset**
  //  UCB0CTL0 = UCCKPL +UCCKPH+ UCMSB + UCMST + UCSYNC;       // 3-pin, 8-bit SPI mstr, MSB 1st
  UCB0CTL0 = UCCKPL+ UCMSB + UCMST + UCSYNC;       // 3-pin, 8-bit SPI mstr, MSB 1st
  UCB0CTL1 = UCSSEL_2;                     // SMCLK
  UCB0BR0 = 0X02;
  UCB0BR1 = 0X00;
  
    
  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**

  
  // Disable display controller.
  P4OUT |= BIT4 ; // STE0

  Delay(100);

  // Send sequence of command
  LCDSend( 0x21, SEND_CMD );  // LCD Extended Commands.
  LCDSend( 0xC8, SEND_CMD );  // Set LCD Vop (Contrast).
  LCDSend( 0x06, SEND_CMD );  // Set Temp coefficent.
  LCDSend( 0x13, SEND_CMD );  // LCD bias mode 1:48.
  LCDSend( 0x20, SEND_CMD );  // LCD Standard Commands, Horizontal addressing mode.
  LCDSend( 0x08, SEND_CMD );  // LCD blank
  LCD_Clear(5);
  LCDSend( 0x0C, SEND_CMD );  // LCD in normal mode.

  /*
  //  Reset watermark pointers.
  LoWaterMark = 0;
  HiWaterMark = LCD_CACHE_SIZE - 1;

  // Clear and Update
  LCDClear();
  LCDUpdate();
*/
}

/****************************************************************************/
/*  Change LCD Pixel mode                                                   */
/*  Function : LcdContrast                                                  */
/*      Parameters                                                          */
/*          Input   :  contrast                                             */
/*          Output  :  Nothing                                              */
/****************************************************************************/
/*void LCDPixel (unsigned char x, unsigned char y, unsigned char mode )
{
    unsigned int    index   = 0;
    unsigned char   offset  = 0;
    unsigned char   data    = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

    index = ((y / 8) * 84) + x;
    offset  = y - ((y / 8) * 8);

    data = LcdCache[index];

    if ( mode == PIXEL_OFF )
    {
        data &= (~(0x01 << offset));
    }
    else if ( mode == PIXEL_ON )
    {
        data |= (0x01 << offset);
    }
    else if ( mode  == PIXEL_XOR )
    {
        data ^= (0x01 << offset);
    }

    LcdCache[index] = data;

    if ( index < LoWaterMark )
    {
        //  Update low marker.
        LoWaterMark = index;
    }

    if ( index > HiWaterMark )
    {
        //  Update high marker.
        HiWaterMark = index;
    }

}*/

/****************************************************************************/
/*  Change LCD Pixel mode                                                   */
/*  Function : LcdContrast                                                  */
/*      Parameters                                                          */
/*          Input   :  contrast                                             */
/*          Output  :  Nothing                                              */
/****************************************************************************/
/*void LCDChrXY (unsigned char x, unsigned char y, unsigned char ch )
{
    unsigned int    index   = 0;
    unsigned int    offset  = 0;
    unsigned int    i       = 0;

    // check for out off range
    if ( x > LCD_X_RES ) return;
    if ( y > LCD_Y_RES ) return;

    //index = ((y / 8) * 84) + x;
    //offset  = y - ((y / 8) * 8);

    //index = y*84+x;
    index = (x*48+y*84)/8;



    for ( i = 0; i < 5; i++ )
    {
      LcdCache[index] = FontLookup[ch - 32][i] << 1;
      index++;
    }

}*/

/****************************************************************************/
/*  Set LCD Contrast                                                        */
/*  Function : LcdContrast                                                  */
/*      Parameters                                                          */
/*          Input   :  contrast                                             */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDContrast(unsigned char contrast) {

    //  LCD Extended Commands.
    LCDSend( 0x21, SEND_CMD );

    // Set LCD Vop (Contrast).
    LCDSend( 0x80 | contrast, SEND_CMD );

    //  LCD Standard Commands, horizontal addressing mode.
    LCDSend( 0x20, SEND_CMD );
}


/****************************************************************************/
/*  Send string to LCD                                                      */
/*  Function : LCDStr                                                       */
/*      Parameters                                                          */
/*          Input   :  size text, text                                      */
/*          Output  :  Nothing                                              */
/****************************************************************************/
void LCDStr(unsigned char size, unsigned char *dataPtr ) {

  // loop to the and of string
  while ( *dataPtr ) {
    LCDChr( size, *dataPtr++ );
  }
}



