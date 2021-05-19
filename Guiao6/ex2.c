# include <detpic32.h>

void delay(int ms);
void send2displays(unsigned char value);
unsigned char toBcd (unsigned char value);
volatile unsigned char voltage = 0; // Global variable

int main(void)
{
	// Configure all (digital I/O, analog input, A/D module, interrupts)
	// Configure the A/D module and port RB4 as analog input

    TRISBbits.TRISB4 = 1; // RBx digital output disconnected
    AD1PCFGbits.PCFG4= 0; // RBx configured as analog input (AN4)
    AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
    // mode an internal counter ends sampling and
    // starts conversion
    AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
    // interrupt is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON2bits.SMPI = 8-1; // Interrupt is generated after XX samples
    // (replace XX by the desired number of
    // consecutive samples)
    AD1CHSbits.CH0SA = 4; // replace x by the desired input
    // analog channel (0 to 15)
    AD1CON1bits.ON = 1; // Enable A/D converter
    // This must the last command of the A/D
    // configuration sequence

	// digital I/0
	TRISBbits.TRISB6 = 0; // RB6 as output

	// display configure
	TRISB = TRISB & 0x80FF; // outputs
	LATDbits.LATD5 = 0; // low
	LATDbits.LATD6 = 0; // high

	// config interrupts
	IPC6bits.AD1IP = 2; // configure priority of A/D interrupts
	IEC1bits.AD1IE = 1; // enable A/D interrupts
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
	EnableInterrupts(); // Global Interrupt Enable

	unsigned int cnt = 0;
	while(1)
	{
		
		if(cnt % 25 == 0) // 250 ms (4 samples/second)
		{
			AD1CON1bits.ASAM = 1;// Start A/D conversion
			
		}
		send2displays(toBcd(voltage));// Send "voltage" variable to displays
        cnt++; 
        delay(10);// Wait 10 ms using the core timer
	}	
    return 0;
}
void _int_(27) isr_adc(void)
{
	int val_ad = 0;
	int j;
	int *p = (int *)(&ADC1BUF0);
	for(j = 0; j<8; j++){
		val_ad = val_ad + p[j*4]; // Calculate buffer average (8 samples)
        printInt(p[4*j],16); // Print ADC1BUF0 value // Hexadecimal (3 digits format)        
	}
    printStr("\n");
	val_ad = val_ad / 8; // Calculate voltage amplitude
	voltage = (val_ad * 33 + 511) / 1023; // Convert voltage amplitude to decimal. Assign it to "voltage"
	
	IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

void delay(int ms) {
  for(; ms > 0; ms--) {
    resetCoreTimer();
    while(readCoreTimer() < 20000);
  }
}

void send2displays(unsigned char value) {

  static const char display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
  static char displayFlag   = 0;

  // Configure RB8 to RB15 as outputs
  TRISB = (TRISB & 0x00FF);

  // Configure RD5 and RD6 as outputs
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;

  // Determine digits
  int digit_low = value & 0x0F;     
  int digit_high = value >> 4;      

  // Send digit_low to display low
  if (displayFlag == 0) {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_low] << 8);
    LATDbits.LATD5 = 1;
    LATDbits.LATD6 = 0;
  }

  // Send digit_high to display high
  else {
    LATB = (LATB & 0x00FF) | (display7Scodes[digit_high] << 8);
    LATDbits.LATD5 = 0;
    LATDbits.LATD6 = 1;
  }
  displayFlag = !displayFlag;

}

  unsigned char toBcd(unsigned char value){
	return ((value/10)<<4) + (value%10);
  }  
