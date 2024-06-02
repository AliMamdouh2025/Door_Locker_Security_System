/*
 * timer1.c
 *
 *  Created on: Oct 29, 2022
 *      Author: Ali
 */

#include "timer1.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /* To use ICU/Timer1 Registers */
#include <avr/interrupt.h> /* For ICU ISR */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
#if(USED_MODE == COMPARE_MODE)
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

#elif(USED_MODE == NORMAL_MODE)
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
#endif

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to initialize the ICU driver
 * 	1. Set the required clock.
 * 	2. Set the required edge detection.
 * 	3. Enable the Input Capture Interrupt.
 * 	4. Initialize Timer1 Registers
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	TCNT0 = Config_Ptr->initial_value;

	TCCR1A = (1<<FOC1A) | (1<<FOC1B); /*FOC1A , FOC1B are 1 to cancel PWM mode
	 *Disconnect OC1A and OC1B  COM1A1=0 COM1A0=0 COM1B0=0 COM1B1=0
	 *WGM10, WGM11 are zero to choose only between normal and compare mode
	 */
	TCCR1B = (Config_Ptr->prescaler) | ((Config_Ptr->mode)<<WGM12); /*Prescaler by the first three bits(CS10,CS12,CS11)
	 *WGM13 is zero to choose only between normal and compare mode by making WGM12 1(for compare) or 0(for normal)
	 */
#if(USED_MODE == COMPARE_MODE)
	OCR1A = Config_Ptr->compare_value;
	TIMSK |= (1<<OCIE1A);// Enable Timer1 Compare Interrupt///////////////////////////////////////////////////////////////////////////
#elif(USED_MODE == NORMAL_MODE)
	TIMSK |= (1<<TOIE1);// Enable Timer1 Overflow Interrupt///////////////////////////////////////////////////////////////////////////
#endif
}


/*
 * Description : Function to disable the Timer1.
 *
 */
void Timer1_deInit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
#if(USED_MODE == COMPARE_MODE)
	TIMSK &= ~(1<<OCIE1A); // Disable Timer1 Compare Interrupt///////////////////////////////////////////////////////////////////////////
#elif(USED_MODE == NORMAL_MODE)
	TIMSK &= ~(1<<TOIE1); // Disable Timer1 Overflow Interrupt
#endif
}

/*
 * Description : Function to set the Call Back function address.
 *
 */
void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}
