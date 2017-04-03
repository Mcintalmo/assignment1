//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include "msp432.h"
#include "portfunc.h"
#include "clock.h"

uint8_t manual = 1; 	// 0: Automatic
							// 1: Manual

uint8_t color[] = {
		0,  					//Off
		 BIT0,  				//Red
				BIT1,  			//Green
		(		BIT1 | BIT2),  	//Yellow
					   BIT2,  	//Blue
		(BIT0 | 	   BIT2),	//Violet
		(       BIT1 | BIT2),  	//Teal
		(BIT0 | BIT1 | BIT2)	//White
};

uint8_t color_index = 0;
void CycleLED(void){
	if(color_index > 6){ // Roll over
		color_index = 0;
	}
	else{
	    color_index++; // Next color
	}
	P2OUT = color[color_index]; // Update color
}

void InitializeLEDs(void){
    //P1DIR |= BIT0; //Port 1.0 configured as output.
    //SelectPortFunction(1, 0, 0, 0);
    //P1OUT &= ~BIT0; //Turn off red LED

    P2DIR |= (BIT0|BIT1|BIT2);
    SelectPortFunction(2, 0, 0, 0);	// General purpose I/0, Red LED
    SelectPortFunction(2, 1, 0, 0);	// General purpose I/0, Green LED
    SelectPortFunction(2, 2, 0, 0);	// General purpose I/0, Blue LED
}

void InitializePushButton(uint8_t x){
    P1DIR &= ~BIT(x); //Resets port 1.x to input.
    P1REN |= BIT(x); //Pull-up or pull-down enabled
    P1OUT |= BIT(x); //Pull-up selected
    SelectPortFunction(1, x, 0, 0); // General purpose I/0
}

void main (void){
    WDTCTL = WDTPW | WDTHOLD; // Stop watch-dog timer
    InitializeLEDs();
    InitializePushButton(1);
    InitializePushButton(4);
    for(;;){
        unsigned char portIn = P1IN;
        if(!(portIn & BIT1)) { // Button 1 is pressed
            manual = !manual; // Toggle mode
        }
        if(!manual){ // Automatic
        	CycleLED();
        }
        else if(manual && !(portIn & BIT4)){// Button 1 is pressed (manual mode)
        	CycleLED();
        }
        volatile uint16_t k = 0; //Force delay
        for(k=0; k<40000; ++k);
    }
}
