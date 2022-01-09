#define _LEGACY_HEADERS
#include <htc.h>

/*  cADC - Display the PICkit Pot Input Value on the built in LEDs

This program samples the voltage on RA0 using the ADC and Displays the 
  value on the 8 LEDs using "cLEDDisp 2" as a base.  

myke predko
04.10.03

UPDATED 9/1/2022 D Halsall
changed incude to htc.h

Running on MPLAB IDE 8.92 with 
compiler: HI-TECH C pic 10/12/16 (lite mode) v 9.81


*/

__CONFIG(INTIO & WDTDIS & PWRTEN & MCLRDIS & UNPROTECT \
  & UNPROTECT & BORDIS & IESODIS & FCMDIS);


int i, j;
int ADCState = 0;               //  Keep Track of ADC Operation
int ADCValue = 0;
int Dlay = 63;                  //  LED Time on Delay Variable
const char PORTAValue[8] = {0b010000, 0b100000, 0b010000, 0b000100, 
                            0b100000, 0b000100, 0b000100, 0b000010};
const char TRISAValue[8] = {0b001111, 0b001111, 0b101011, 0b101011, 
                            0b011011, 0b011011, 0b111001, 0b111001};
const char NOTPORTA[8] = {0, 0, 0, 0, 0, 0, 0, 0};

main()
{

    PORTA = 0;
    CMCON0 = 7;                 //  Turn off Comparators
    ANSEL = 1;                  //  Just RA0 is an Analog Input

    ADCON0 = 0b00000001;        //  Turn on the ADC
                                //   Bit 7 - Left Justified Sample
                                //   Bit 6 - Use VDD
                                //   Bit 4:2 - Channel 0
                                //   Bit 1 - Do not Start
                                //   Bit 0 - Turn on ADC
    ADCON1 = 0b00010000;        //  Selemct the Clock as Fosc/8

    while(1 == 1)               //  Loop Forever
    {
        for (i = 0; i < 8; i++ )
        {                       //  Loop through Each of the 8 LEDS
            for (j = 0; j < Dlay; j++);  //  Display "On" Delay Loop
            if ((ADCValue & (1 << i)) == 0)
                PORTA = NOTPORTA[i];
            else
                PORTA = PORTAValue[i];
            TRISA = TRISAValue[i];
        }  //  rof
        switch (ADCState)       //  ADC State Machine
        {
            case 0:             //  Finished, Start Next Sample
                GODONE = 1;
                ADCState++;
                break;
            case 1:             //  Wait for ADC to complete
                if (!GODONE)
                    ADCState++; //  Sample Finished
                break;
            case 2:             //  Save Sample Value in "ADCValue" 
                ADCValue = ADRESH;
                ADCState = 0;
                break;
        }  // hctiws
    }  //  elihw
}  //  End cADC
