In order to utilize the hardware changes made in the OTTER microprocessor to support the Basys 3's built-in XADC, software is needed to drive values, such as the clock divider.

In the code provided below, there are many macros (seen in the #define section at the top of funcs.c) which can be modified to meet a user's needs. reg0Default, reg1Default, and reg2Default which we have chosen to make 0x0003, 0x30F0, and 0x0000 respectively, can be changed to match what the user wants to initialize these registers to be in hardware. This should match the default hardware values, in order to maintain other bits when you change parts of each register. What each registers' bits represent can be found on page 43 of the Artix 7 XADC data sheet, which can be found here: https://www.xilinx.com/support/documentation/user_guides/ug480_7Series_XADC.pdf

The other provided macros which may be changed by the user are dclkFreq (which represents the desired frequency for the module to run at, in MHz), acqTime* (which represents the amount of cycles desired to acquire a sample, in clock cycles), and convRate (which represents the conversion rate, in KSPS).
*note: acqTime can only be 4 or 10 cycles, according to the XADC datasheet.

Above these are macros which should not be modified. These are constants that have been renamed for ease of readability of the remaining code. 

The functions included in this sample code which are significant and not helper functions are writeDclkDivider(), getSamples(), getPeakToPeak(), and getAverage(). The function of each should be self-explanatory in the code, and are all carefully commented for ease of the user.

The most significant of these functions is definitely writeDclkDivider(). This is the code that could be used as a basis to write other values to the XADC, making this code a great starting point to make the XADC completely configurable from the OTTER in software. Notice what is done in this function:
- First, we create a pointer to represent the addresses for the ADC and its data
- Then, we take the inputted dclkDivider that we would like to write to certain bits, and bit-shift it so it will be put into the right spot in reg2 (the register which contains the bits we would like to replace)
- On reg2, we AND the bits to clear out the upper eight, as these are what we wish to rewrite
- On reg2, we then OR the bits with our calculated value from step 2
- Note that the pointer to reg2 was passed in. This is so the main function can see how this value has been changed in this function, making sure we don't accidentally overwrite the value in a later function with the unchanged register value
- We then set the register we'd like to write to by setting address to be configReg2 (a value defined in a macro)
- We also set the data we'd like to write by setting dataAddress to be the final value we just calculated, stored in *reg2
- To end, we run the setDENandDWE() function, which tells the hardware that we have modified something and that we would like to write it to hardware.

This functionality described above can easily be modified in order to repeat this procedure for other XADC values the user would like to change in software.

The provided sample code can be found at https://github.com/mpgiii/OtterXADC, and questions about it can be directed to me at georgariou3@gmail.com
