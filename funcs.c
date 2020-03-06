#include <stdio.h>
#include <math.h>

#define conversionCycles 22

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
