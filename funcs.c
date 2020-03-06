#include <stdio.h>
#include <math.h>

#define conversionCycles 22

#define adcAddress 0x12000000
#define adcDataAddress 0x13000000

#define configReg0 0x40
#define configReg1 0x41
#define configReg2 0x42

//Address of ADC: 0x12000000 (it's like a pointer to a pointer)
//     ^ it lets you choose what you want to change about the ADC
//ADC Data: 0x13000000 (this is what you're writing to that pointer)


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

int readData(int address) {
    int *hi = adcAddress;
    int *hi2 = adcDataAddress;
    *hi2 = address;
    return *hi;
    
    
}

int writeData(int address) {
    
}

void writeDclkDivider(int dclkDivider) {
    int* address = adcAddress;
    int* dataAddress = adcDataAddress;
    
    *adcAddress = configRed2;
    *dataAddress = dclkDivider << 8;
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
