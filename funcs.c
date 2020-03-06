#include <stdio.h>
#include <math.h>

#define conversionCycles 22

#define adcAddress 0x12000000
#define adcDataAddress 0x13000000

#define configReg0 0x40
#define configReg1 0x41
#define configReg2 0x42

#define DENDWEaddress 0x14000000


int main()
{
    
    int dclkFreq;
    int acqTime;
    int conversionRate;
    
    printf("Enter DCLK Frequency (MHz)\n");
    scanf("%d", &dclkFreq);
    printf("Enter acquisition time (cycles)\n");
    scanf("%d", &acqTime);
    printf("Enter ADC conversion rate (KSPS)\n");
    scanf("%d", &conversionRate);
    
    printf("\nYour clock divider: %d", 
        getDclkDivider(dclkFreq, acqTime, conversionRate));

    return 0;
}

void setDENandDWE() {
    int* denAndDwe = DENDWEaddress;
    *denAndDwe = 0x3;
}

/* writeDclkDivider
 * Input:  dclkDivider
 * Output: none */
void writeDclkDivider(int dclkDivider) {
    int* address = adcAddress;
    int* dataAddress = adcDataAddress;
    
    //set the address we are writing to to configReg2
    *adcAddress = configReg2;
    
    //write the clock divider to that spot, bits shifted by 8 (other 8 unused)
    *dataAddress = dclkDivider << 8;
    
    //set DEN and DWE high so the ADC will actually read what we wrote
    setDENandDWE();
}


/* getClkDivider
 * Inputs: dclkFrequency (MHz)
 *         acqTime (clock cycles)
 *         conversionRate (KSPS)
 * Output: clock divider value, rounded to nearest whole number */
int getDclkDivider(int dclkFrequency, int acqTime, int conversionRate) {
    // dclkFrequency inputted in MHz
    // acqTime in clock cycles
    // conversionRate inputted in KSPS
    
    // NOTE: multiplied by 1000 to account for dclkFrequency being in MHz.
    return round(1000 * ((double)dclkFrequency / 
        (conversionRate * (acqTime + conversionCycles))));
}

// 4 cycle acquisition ( can be extended to 10 by setting ACQ bit )
// conversion is 22 cycles
