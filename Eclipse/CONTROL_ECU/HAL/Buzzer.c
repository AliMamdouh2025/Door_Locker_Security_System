/*
 * Buzzer.c
 *
 *  Created on: Nov 4, 2022
 *      Author: Ali
 */


#include "Buzzer.h"
#include "gpio.h"

/*
 * Description :
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 *  Turn off the buzzer through the GPIO.
 */
void Buzzer_init(void)
{
	GPIO_setupPinDirection(Buzzer_Port,Buzzer_Pin,PIN_OUTPUT);
	GPIO_writePin(Buzzer_Port,Buzzer_Pin,0);
}

/*
 * Description :
 * Function to enable the Buzzer through the GPIO
 */
void Buzzer_on(void)
{
	GPIO_writePin(Buzzer_Port,Buzzer_Pin,1);
}

/*
 * Description :
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_off(void)
{
	GPIO_writePin(Buzzer_Port,Buzzer_Pin,0);
}

