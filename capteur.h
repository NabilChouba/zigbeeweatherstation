#include "msp430x22x4.h"

// TA1 captures , comparator (CCI1B), synchronized , interrupt , disabled
#define COMP_CAP (CCIS_1 | SCS | CAP  |CCIE)

#define VDD_value                       3.6     //vloatage
#define FREC_value                       1      //Mhz

#define Tmax_sample                       10   // number of sample to make avarage
#define Hmax_sample                       10   // number of sample to make avarage
#define Lmax_sample                       10   // number of sample to make avarage

void capteur (char *Temperature, char *Humidity, char *Luminosity);