/*
 * HMI_ECU.h
 *
 *  Created on: Nov 5, 2022
 *      Author: Ali
 */

#ifndef HMI_ECU_H_
#define HMI_ECU_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define MATCHED_PASS 0x01
#define UNMATCHED_PASS 0x00
#define CHECK_PASSWORD 0x02
#define ALERT 0x03
#define MOTOR_UNLOCKING 0x04
#define MOTOR_LOCKING 0x05
#define MOTOR_STOP 0x06
#define CHECK_PASSWORDS_MATCHING 0x07

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 Pass[5];
uint32 g_tick;
uint8 Flag_Of_locking;
uint8 Check_Matching;
uint8 num;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : takes password from user
 */
void Enter_Password(uint8* pass);
/*
 * Description : it check if the two passwords are matched or not
 *if the two passwords are matched the function sent to MC2 to store the password in the eeprom
 */
uint8 Check_Passwords_Matching(void);
/*
 * Description : it alert if the entered password doesnt match the password saved in eeprom for three times
 */
uint8 Alert(void);

void init(void);
void EnterPasswordAgain(void);
uint8 EnterAndCheckPassword(void);
void Main_Options(void);
void Timer_On(void(*a_ptr)(void));
void Timer_Door_locking_unlocking(void);
void Door_unlocking(void);
void Door_locking(void);
void Door_Open(void);

#endif /* HMI_ECU_H_ */
