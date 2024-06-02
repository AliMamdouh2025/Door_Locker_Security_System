/*
 * Buzzer.h
 *
 *  Created on: Nov 4, 2022
 *      Author: Ali
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define Buzzer_Port PORTC_ID
#define Buzzer_Pin PIN2_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 *  Turn off the buzzer through the GPIO.
 */
void Buzzer_init(void);

/*
 * Description :
 * Function to enable the Buzzer through the GPIO
 */
void Buzzer_on(void);

/*
 * Description :
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
