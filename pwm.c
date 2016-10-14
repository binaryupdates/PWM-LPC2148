/************************************************************/
/* PROJECT NAME: PWM (Pulse Width Modulation)	            	*/
/* Device:       LPC2148                										*/
/* Filename:     pwm.c                                   		*/
/* Language:     C                      	                	*/
/* Compiler:     Keil ARM				                						*/
/* For more detail visit www.binaryupdates.com		        	*/
/************************************************************/

#include <lpc214x.h>

void initPWM(void);						// Initialize PWM
void initClocks(void); 	      // Setup PLL and Clock Frequency

int main(void)
{
	initClocks(); //Initialize CPU and Peripheral Clocks @ 60Mhz
	initPWM(); //Initialize PWM

  //IO1DIR = 0x1; This is not needed!
	//Also by default all pins are configured as Inputs after MCU Reset.
   
    while(1)
    {
		if( !((IO1PIN) & (1<<16)) ) // Check P1.16
		{
			PWMMR4 = 2500; //T-ON=25% , Hence 25% Bright
			PWMLER = (1<<4); //Update Latch Enable bit for PWMMR4
		}
		else if( !((IO1PIN) & (1<<17)) ) // Check P1.17
		{
			PWMMR4 = 5000; //50% Bright
			PWMLER = (1<<4);
		}
		else if( !((IO1PIN) & (1<<18)) ) // Check P1.18
		{
			PWMMR4 = 7500; //75% Bright
			PWMLER = (1<<4);
		}
		else if( !((IO1PIN) & (1<<19)) ) // Check P1.19
		{
			PWMMR4 = 10000; //100% Bright
			PWMLER = (1<<4);
		}
    }
    //return 0; //normally this wont execute ever
}

void initPWM(void)
{
	PINSEL0 = (PINSEL0 & ~(1 << 16)) | (1 << 17); // Select PWM4 output for Pin0.8
	PWMPCR = 0x0; //Select Single Edge PWM - by default its single Edged so this line can be removed
	PWMPR = 60-1; // 1 micro-second resolution
	PWMMR0 = 10000; // 10ms period duration
	PWMMR4 = 500; // 0.5ms - pulse duration i.e width (Brigtness level)
	PWMMCR = (1<<1); // Reset PWMTC on PWMMR0 match
	PWMLER = (1<<0)|(1<<4); // update MR0 and MR4
	PWMPCR = (1<<12); // enable PWM output
	PWMTCR = (1<<1) ; //Reset PWM TC & PR

	//Now , the final moment - enable everything
	PWMTCR = (1<<0) | (1<<3); // enable counters and PWM Mode

	//PWM Generation goes active now - LED must be 25% Bright after boot!!
	//Now you can get the PWM output at Pin P0.8!
}

void initClocks(void)
{
	PLL0CON = 0x01;   //Enable PLL
	PLL0CFG = 0x24;   //Multiplier and divider setup
	PLL0FEED = 0xAA;  //Feed sequence
	PLL0FEED = 0x55;
	
	while(!(PLL0STAT & 0x00000400)); //is locked?
		
	PLL0CON = 0x03;   //Connect PLL after PLL is locked
	PLL0FEED = 0xAA;  //Feed sequence
	PLL0FEED = 0x55;
	VPBDIV = 0x01;    // PCLK is same as CCLK i.e.60 MHz
}
