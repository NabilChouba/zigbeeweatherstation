
#include "msp430x22x4.h"
#include "lcd.h"
#include "capteur.h"


void capteur (char *Temperature, char *Humidity, char *Luminosity){

 /// light musure [0 to 1024]
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_12;
  ADC10AE0 = 0x00;                         // P2.0 ADC option select
  ADC10AE1 = BIT4;                         // P2.0 ADC option select

  P4DIR |= BIT5;
  P4OUT |= BIT5; // Unused pins are output driven low
 
  int accLight =0;
  for (  int  m=0;m<Lmax_sample;m++)
  {
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit

    accLight+=ADC10MEM;
    
  }
  // the avreage from Lmax_sample 0 to 100 (11 is to translate from 0-1024 to 0-100)
  *Luminosity = accLight/(11*Lmax_sample);
 
  /// temperature musure 
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_4;
  ADC10AE0 |= BIT4;                         // P2.0 ADC option select
 
  P4DIR |= BIT5;
  P4OUT |= BIT5; // Unused pins are output driven low
 
  int accTemp=0;
  
  for (  char  m=0;m<Tmax_sample;m++)
  {
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit

    accTemp+=ADC10MEM;

  }
  // diveder resistonce : R=3.3K 
  // thermistor : B = 4400 / (T0= 298 , R0= 10K )
  // Vdd  = R.i + Rt.i (1)
  // Vadc = R.i        (2)
  // (1)/(2) Vdd/Vadc => R/(R+Rt)
  // 1/T =  1/T0 + 1/B log(Rt/R0)
  //
  // the avreage from Tmax_sample 0° to 50°
  *Temperature= (accTemp/1024*Tmax_sample);  
   
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  OA0CTL0 = OAPM_3 + OAADC1;                // OAPM_3 : Fast rate,
                                            // "+" connected to OA1I0 (default),
                                            // OAADC1 :Output connected to A1/OA0O (this is P2.1)
  OA0CTL1 = OAFBR_2 + OAFC_3 + OARRIP;      // OAFBR_4 : "-" connected to AVcc * 4/16, =0.25
                                            // OAFC_3  : Comparator mode,
                                            // OARRIP  : Reverse resistor connection

  ADC10AE0 = BIT1;                          // P2.1/0 analog function select  
  
//  P2OUT = 0; // Unused pins are output driven low
  P2DIR |= BIT2 ; // (Comparator_A+ overrides this)
  P2SEL |= BIT2 | BIT3 ; // Timer_A ch 0 output to resistor OUT1 P2.2
                // P2.3 output OA to CCI1B

// Timer_A: SMCLK , no division , 
//continuous (Continuous mode: the timer counts up to 0FFFFh.) ,
// no need to clear , no ints
TACTL = TASSEL_2 | ID_0 | MC_2;
  
int ChargeTime , DischargeTime; // Measured transient times
DischargeTime = 1000; // Dummy value for first delay
ChargeTime= 1000; // Dummy value for first delay
     
     
  int accHumdity=0;
     // __________________________________________________________
  for (  char z=0;z<Hmax_sample;z++) {
    volatile int ii;
    for(ii = 0; ii < 0xFFFF; ii++){}
    // Charging transient: discharge C fully , Set output , wait for Comp_A+
    //TACCR0 = TAR + 25*DischargeTime; // Delay for full discharge
    TACCR0 = TAR + 1000; // Delay for full discharge
    TACCTL0 = OUTMOD_1; // Set on compare to start charge
    // TA1 captures , comparator (CCI1B), synchronized , interrupt , disabled
    //#define COMP_CAP (CCIS_1 | SCS | CAP | CCIE)
    TACCTL1 = COMP_CAP | CM_1; // Capture comparator rising
    __low_power_mode_0 (); // Wait for timer and comparator
    ChargeTime = TACCR1 - TACCR0; // Duration of charge
    // Discharging transient: charge C fully , Reset output , wait for Comp_A+
    for(ii = 0; ii < 0xFFFF; ii++) {}
    //TACCR0 = TAR + 25*ChargeTime; // Delay to allow full charge
    TACCR0 = TAR + 1000; // Delay to allow full charge
    TACCTL0 = OUTMOD_5; // Reset output to start discharge
    TACCTL1 = COMP_CAP | CM_2; // Capture comparator falling
    __low_power_mode_0 (); // Wait for timer and comparator
    DischargeTime = TACCR1 - TACCR0; // Duration of discharge     
      accHumdity += (DischargeTime + ChargeTime);
    }


 
  //the capa value is x10 
  //    capa = -offset + accHumdity / (2 * Hz *R(M om)* Ln(0.5));
  //int capa = -177    + accHumdity / (2 * 16 *1.012* 0.69);
  int capa =    accHumdity / (2 * FREC_value *1.012* 0.69);
  // i will find the offset and the sensibility ! i only need to know that this is liniare
  // i need 2 sample 
  // capa = 355.8 h=53%
 
  //from humidity courve see HCH-1000.pdf 
  //               c(100%/frego) -capa / Sensitivity
//  Humidity = 100 - (3544 - capa) /5.5;
    *Humidity = 100 - (4000 - capa) /5.5;
}
