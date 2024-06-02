/*
 * timer1.h
 *
 *  Created on: Oct 29, 2022
 *      Author: Ali
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define NORMAL_MODE 0x00
#define COMPARE_MODE 0x01
#define USED_MODE COMPARE_MODE

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024,External_clock_falling, External_clock_rising //External clock source on T1 pin
}Timer1_Prescaler;

typedef enum
{
	normal,compare
}Timer1_Mode;

typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer driver
 *
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description : Function to disable the Timer1.
 *
 */
void Timer1_deInit(void);

/*
 * Description : Function to set the Call Back function address.
 *
 */
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
