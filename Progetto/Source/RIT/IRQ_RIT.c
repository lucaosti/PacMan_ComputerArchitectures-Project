/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "funzioni.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down = 0;
extern int seed;
extern int direzione;

void RIT_IRQHandler (void)
{
	seed++;
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
		direzione = 1;
	}
	else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		direzione = 2;
	}
	else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		direzione = 4;
	}
	else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		direzione = 3;
	}

	/* button management */
	if(down != 0){
		if((LPC_GPIO2->FIOPIN & (1 << 10)) == 0){
			down++;
			switch(down){
				case 2:
					pausa();
					break;
				default:
					break;
			}
		}
		else {
			down = 0;
			NVIC_EnableIRQ(EINT0_IRQn);
			LPC_PINCON->PINSEL4 |= (1 << 20);
		}
	}
	
	
	
	reset_RIT();
	
	LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
