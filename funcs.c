#include <stdio.h>
#include <math.h>

#define conversionCycles 22

#define adcAddress 0x12000000
#define adcDataAddress 0x13000000

#define configReg0 0x40
#define configReg1 0x41
#define configReg2 0x42

#define DENDWEaddress 0x14000000

#define EOCaddress 0x15000000
#define sampleAddress 0x16000000

// DO NOT CHANGE ANYTHING ABOVE THIS LINE //

// defines below this line can be changed //

#define reg0Default 0x0003 //0000000000000011
#define reg1Default 0x30F0 //0011000011110000
#define reg2Default 0x0000 //0000000000000000

#define dclkFreq 50 //in MHz
#define acqTime 4 //in cycles. can only be 4 or 10
#define convRate 50 //in KSPS

/* setDENandDWE
 * sets the bit located at the DENDWEaddress to high
 * Input:  none
 * Output: none */
void setDENandDWE() {
    int* denAndDwe = DENDWEaddress;
    *denAndDwe = 0x1;
    // Hardware will (should) reset this value to 0.
}

/* writeDclkDivider
 * Input:  dclkDivider
 *         reg2 (pointer to change it in main when modified)
 * Output: none */
void writeDclkDivider(int dclkDivider, int* reg2) {
    int* address = adcAddress;
    int* dataAddress = adcDataAddress;
    
    int shiftedDivider = dclkDivider << 8;
    
    *reg2 &= 0x00FF; //clear out the old clock divider
    *reg2 |= shiftedDivider; //and set the inputted one
    
    //set the address we are writing to be configReg2
    *address = configReg2;
    
    //write the clock divider to that spot, bits shifted by 8 (other 8 unused)
    *dataAddress = *reg2;
    
    //set DEN and DWE high so the ADC will actually read what we wrote (done in hardware)
    setDENandDWE();
}


/* getDclkDivider
 * Inputs: dclkFrequency (MHz)
 *         acqTime (clock cycles)
 *         conversionRate (KSPS)
 * Output: clock divider value, rounded to nearest whole number */
int getDclkDivider(int dclkFrequency, int acquireTime, int conversionRate) {    
    // NOTE: multiplied by 1000 to account for dclkFrequency being in MHz.
    return round(1000 * ((double)dclkFrequency / 
        (conversionRate * (acquireTime + conversionCycles))));
}

/* getSamples
 * populates the inputted int array with samples from the ADC
 * Input: adcSamples
 * Output: none */
void getSamples(int* adcSamples) {
    int i = 0;
    int* EOC = EOCaddress;
    int* sampleAddr = sampleAddress;
    while(i < 5000) {
        // do not collect samples until EOC goes high
        while(!(*EOC)) {
            ;
        }
        // once EOC goes high, fill up our array.
        adcSamples[i++] = *sampleAddr;
    }
    return;
}

/* getPeakToPeak
 * finds the min and max in the adcSamples list, and outputs their difference
 * Inputs: adcSamples (int array)
 *         N (size of adcSamples)
 * Output: peak to peak value */
int getPeakToPeak(int* adcSamples, int N) {
    int max = adcSamples[0];
    int min = adcSamples[0];
    int i;
    for(i = 0; i < N; i++) {
        if (adcSamples[i] < min) 
            min = adcSamples[i];
        if (adcSamples[i] > max)
            max = adcSamples[i];
    }
    return (max - min);
}

/* getPeakToPeak
 * finds the average value in the adcSamples list
 * Inputs: adcSamples (int array)
 *         N (size of adcSamples)
 * Output: average value */
int getAverage(int* adcSamples, int N) {
    int sum = 0;
    int i;
    for(i = 0; i < N; i++) {
        sum += adcSamples[i];
    }
    return (sum / N);
}


int main()
{
    int reg0 = reg0Default;
    int reg1 = reg1Default;
    int reg2 = reg2Default;

    int dclkDivider;
    
    int peakToPeak;
    int average;
    
    int adcSamples[5000];
    
    // set the clock rate from user-specified parameters, from the #defines
    dclkDivider = getDclkDivider(dclkFreq, acqTime, convRate);
    writeDclkDivider(dclkDivider, &reg2);
    
    // run indefinitely (until shutdown)
    while(1){
        // populate the samples array
        getSamples(adcSamples);
        // get the peak to peak of this sample
        peakToPeak = getPeakToPeak(adcSamples, 5000);
        // get the average of this sample
        average = getAverage(adcSamples, 5000);
    }

    return 0;
}
