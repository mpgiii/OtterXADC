#include <math.h>

#define conversionCycles 22

int main(int argc, char* argv[]) {
    
}



int getDclkDivider(int dclkFrequency, int acqTime, int conversionRate) {
    // dclkFrequency inputted in MHz
    // acqTime in clock cycles
    // conversionRate inputted in KSPS
    
    // NOTE: multiplied by 1000 to account for dclkFrequency being in MHz.
    return round(1000 * 
        (dclkFrequency / (conversionRate * (acqTime + conversionCycles))));
}

// 4 cycle acquisition ( can be extended to 10 by setting ACQ bit )
// conversion is 22 cycles