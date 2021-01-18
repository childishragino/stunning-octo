#include <MKL25Z4.H>
#include "switches.h"
#include "LEDs.h"

volatile unsigned count_l=0; // can be changed by invisible code/external thread/hardwhere at anytime.
volatile unsigned count_ir=0; // can be changed by invisible code/external thread/hardwhere at anytime.

void init_switch(void) {
	SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK; /* enable clock for port D */
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; /* enable clock for port A */
	
	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	PORTD->PCR[SW_POS_IR] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a); 
	PORTA->PCR[SW_POS_L] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a); //added
		
	/* Set port D switch bit to inputs */
	PTD->PDDR &= ~MASK(SW_POS_IR); //
	PTA->PDDR &= ~MASK(SW_POS_L); //added


	/* Enable Interrupts */
	NVIC_SetPriority(PORTD_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTD_IRQn); 
	NVIC_EnableIRQ(PORTD_IRQn);
	
	/* Enable Interrupts */
	NVIC_SetPriority(PORTA_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTA_IRQn); 
	NVIC_EnableIRQ(PORTA_IRQn);
}


void PORTD_IRQHandler(void) {  
	
	//DEBUG_PORT->PSOR = MASK(DBG_ISR_POS);
	// clear pending interrupts
	NVIC_ClearPendingIRQ(PORTD_IRQn);
	//if (!(MASK(SW_POS))){
		if (PORTD->ISFR & MASK(SW_POS_IR)) {
		count_ir++;
	}
//}
	// clear status flags 
	PORTD->ISFR = 0xffffffff;
	//DEBUG_PORT->PCOR = MASK(DBG_ISR_POS);
}


void PORTA_IRQHandler(void) {  
	
	//DEBUG_PORT->PSOR = MASK(DBG_ISR_POS);
	// clear pending interrupts
	NVIC_ClearPendingIRQ(PORTA_IRQn);
	//if (!(MASK(SW_POS))){
		if (PORTA->ISFR & MASK(SW_POS_L)) {
		count_l++;
	}
//}
	// clear status flags 
	PORTA->ISFR = 0xffffffff;
	//DEBUG_PORT->PCOR = MASK(DBG_ISR_POS);
}

// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
