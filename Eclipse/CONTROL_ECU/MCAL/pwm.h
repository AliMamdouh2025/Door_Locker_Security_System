 /******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: header file for the ATmega32 PWM driver
 *
 * Author: Ali Mamdouh
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for sending PWM with required frequency.
 * Timer0 will be used with required pre-scaler
 * Duty Cycle can be changed by updating the value in The Compare Register
 */
void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* PWM_H_ */
