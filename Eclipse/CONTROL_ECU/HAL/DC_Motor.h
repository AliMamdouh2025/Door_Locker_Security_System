 /******************************************************************************
 *
 * Module: Motor
 *
 * File Name: DC_Motor
 *
 * Description: header file for the motor driver
 *
 * Author: Ali Mamdouh
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define PORT_MOTOR PORTB_ID
#define PIN0_MOTOR PIN0_ID
#define PIN1_MOTOR PIN1_ID

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	CW, A_CW, STOP
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : Function to initialize the Motor driver
 * 1- Set the two pins of motor which are responsible for ClockWise,AntiClockWise,Stop the motor
 */
void DcMotor_Init(void);

/*
 * Description : Function to control the speed and rotation direction of the motor
 * 1-speed is controlled by PWM driver which sends signals to the enable pin of motor
 * 2-rotation is controlled by the two pins of the H-Bridge of the motor
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* MOTOR_H_ */
