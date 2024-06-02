 /******************************************************************************
 *
 * Module: Motor
 *
 * File Name: motor.h
 *
 * Description: header file for the motor driver
 *
 * Author: Ali Mamdouh
 *
 *******************************************************************************/


#include "gpio.h"
#include "Common_Macros.h"
#include "DC_Motor.h"
#include "pwm.h"

/*
 * Description : Function to initialize the Motor driver
 * 1- Set the two pins of motor which are responsible for ClockWise,AntiClockWise,Stop the motor
 */
void DcMotor_Init(void)
{
	GPIO_setupPinDirection(PORT_MOTOR,PIN0_MOTOR,PIN_OUTPUT);
	GPIO_setupPinDirection(PORT_MOTOR,PIN1_MOTOR,PIN_OUTPUT);
	GPIO_writePin(PORT_MOTOR,PIN0_MOTOR,0); //to initially stop the motor
	GPIO_writePin(PORT_MOTOR,PIN1_MOTOR,0); //to initially stop the motor
}

/*
 * Description : Function to control the speed and rotation direction of the motor
 * 1-speed is controlled by PWM driver which sends signals to enable pin of motor
 * 2-rotation is controlled by the two pins of H-Bridge of the motor
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	if(state == CW)
	{
		GPIO_writePin(PORT_MOTOR,PIN1_MOTOR,0);
		GPIO_writePin(PORT_MOTOR,PIN0_MOTOR,1);
		PWM_Timer0_Start(speed*2.55); //equation to control motor from 0% to 100% speed i.e:100(max speed)*2.55 = 255(max duty)
	}
	else if(state == A_CW)
	{
		GPIO_writePin(PORT_MOTOR,PIN1_MOTOR,1);
		GPIO_writePin(PORT_MOTOR,PIN0_MOTOR,0);
		PWM_Timer0_Start(speed*2.55); //equation to control motor from 0% to 100% speed i.e:100(max speed)*2.55 = 255(max duty)
	}
	else if(state == STOP)
	{
		GPIO_writePin(PORT_MOTOR,PIN1_MOTOR,0);
		GPIO_writePin(PORT_MOTOR,PIN0_MOTOR,0);
	}


}
